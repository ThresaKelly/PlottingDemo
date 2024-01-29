#include "timeseriesexample.h"
#include "ui_timeseriesexample.h"

TimeSeriesExample::TimeSeriesExample(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimeSeriesExample)
{
    ui->setupUi(this);

    SetupPlot();
    SetupAxisOptions();

    ui->stop->setVisible(false);
    ui->start->setEnabled(false);
    ui->horizontalPlotScroll->setEnabled(false);
    ui->horizontalPlotScroll->setMinimum(0);
    ui->horizontalPlotScroll->setMaximum(1000);
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(OnXAxisChanged(QCPRange)));
}

TimeSeriesExample::~TimeSeriesExample()
{
    delete ui;
}


void TimeSeriesExample::DelayedRealtimeDataSlot()
{
    // end timer
    delayTimer.stop();
    // mark flag
    plotDelay = true;
    // add annotation
    if(ui->axisType->currentIndex() == 2){
        double epoch = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        double dt = ui->cosDelaySec->value();
        AddAnnotation(epoch-dt,epoch,"Delay period.");
    }
    else{
        AddAnnotation(0.,timePass.elapsed()/1000.0,"Delay period.");
    }
}

void TimeSeriesExample::OnMousePress(QMouseEvent *event)
{
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(OnMouseMove(QMouseEvent*)));
    OnMouseMove(event);
}

void TimeSeriesExample::OnMouseMove(QMouseEvent *event)
{
    amplitude = abs(YFromMouse(event));
    SetAmplitudeLine(amplitude);
}

void TimeSeriesExample::OnMouseRelease(QMouseEvent *event)
{
    disconnect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)),0,0);
}

void TimeSeriesExample::on_axisType_currentIndexChanged(int index)
{
    ui->start->setEnabled(true);
    switch(index){
        case 0: {
            ui->plot->xAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
            ui->plot->xAxis->setLabel("Seconds");
            break;
        }
        case 1: {
            QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
            timeTicker->setTimeFormat("%m:%s:%z");
            ui->plot->xAxis->setTicker(timeTicker);
            ui->plot->xAxis->setLabel("Min:Sec:Nano");
            break;
        }
        case 2: {
            // This QCPAxisTicker subclass generates ticks that correspond to real calendar
            // dates and times. The plot axis coordinate is interpreted as Unix Time, so
            // seconds since Epoch (January 1, 1970, 00:00 UTC).
            QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
            dateTimeTicker->setDateTimeFormat("d. MMM yyyy\n hh:mm:ss AP");
            ui->plot->xAxis->setTicker(dateTimeTicker);
            ui->plot->xAxis->setLabel("Day. Month Year Hr:Min:Sec");
            break;
        }
    }
}

void TimeSeriesExample::on_horizontalPlotScroll_valueChanged(int value)
{
    if(!lockScroll){
        ui->plot->xAxis->setRange(
            (tStart + (ScrollStep() * value)),
            ui->plot->xAxis->range().size(),
            Qt::AlignCenter
            );
        ui->plot->replot();
    }
}

void TimeSeriesExample::OnXAxisChanged(QCPRange range)
{
    // adjust position of scroll bar slider
    ui->horizontalPlotScroll->setValue(   qRound( (range.center() - tStart) / ScrollStep() ));
    // adjust size of scroll bar slider
    ui->horizontalPlotScroll->setPageStep(qRound( range.size() ));
}

void TimeSeriesExample::SetupAxisOptions()
{
    ui->axisType->addItem("Time in seconds");
    ui->axisType->addItem("Minutes:Seconds:Nanoseconds");
    ui->axisType->addItem("Date/time");
}

void TimeSeriesExample::SetupPlot()
{
    // axis
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->yAxis->setRange(-1.5,1.5);
    // legend
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->plot->legend->setFont(legendFont);
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->yAxis2, SLOT(setRange(QCPRange)));
}

void TimeSeriesExample::SetupAmpLine()
{
    // init item
    amplitudeLine1 = new QCPItemStraightLine(ui->plot);
    amplitudeLine2 = new QCPItemStraightLine(ui->plot);
    amplitudeLine1->setPen(QPen(Qt::black,3,Qt::DotLine));
    amplitudeLine2->setPen(QPen(Qt::black,3,Qt::DotLine));
    SetAmplitudeLine(amplitude);
    amplitudeLine1->setVisible(true);
    amplitudeLine2->setVisible(true);
}

void TimeSeriesExample::SetAmplitudeLine(double a)
{
    // draw horizontal line at y=a
    amplitudeLine1->position("point1")->setCoords(0, a);
    amplitudeLine1->position("point2")->setCoords(1, a);
    amplitudeLine2->position("point1")->setCoords(0,-a);
    amplitudeLine2->position("point2")->setCoords(1,-a);
}

void TimeSeriesExample::ToggleUIStartStop(bool state)
{
    // toggle UI elements
    ui->stop->setVisible(                  state);
    ui->start->setVisible(               ! state);
    ui->axisType->setEnabled(            ! state);
    ui->cosDelaySec->setEnabled(         ! state);
    ui->plot->legend->setVisible(          state);
    ui->horizontalPlotScroll->setEnabled(! state);
    if(amplitudeLine1 != nullptr) amplitudeLine1->setVisible( state);
    if(amplitudeLine2 != nullptr) amplitudeLine2->setVisible( state);
    lockScroll = state;
}

void TimeSeriesExample::StartTimer(int nsec)
{
    // setup a timer that repeatedly calls realtimeDataSlot
    connect(&dataTimer,  SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
    connect(&delayTimer, SIGNAL(timeout()), this, SLOT(DelayedRealtimeDataSlot()));
    dataTimer.start(nsec);
    delayTimer.start(ui->cosDelaySec->value() * 1000.);
    // start elapsed timer
    timePass.start();
}

void TimeSeriesExample::StopTimer()
{
    // stop timers
    if (dataTimer.isActive()) dataTimer.stop();
    if (delayTimer.isActive()) delayTimer.stop();
    dataTimer.disconnect();
    delayTimer.disconnect();
    plotDelay = false;
}

void TimeSeriesExample::RealtimeDataSlot()
{
    // check time elapsed since start of demo, in seconds
    double t_elapsed = timePass.elapsed()/1000.0;
    // get x
    double t = t_elapsed;
    if(ui->axisType->currentIndex() == 2) // if using date/time axis
        t = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    // get y
    double yNoiseSin = amplitude * ( qSin(2.0*t) + QRandomGenerator::global()->bounded(-100,100)/500.0 * qSin(t/0.3843) );
    double yCleanSin = amplitude *   qSin(2.0*t);
    // plot sin
    ui->plot->graph(0)->addData(t,yNoiseSin);
    ui->plot->graph(1)->addData(t,yCleanSin);
    // plot cos
    if(plotDelay){
        // get y
        double yNoiseCos = amplitude * ( qCos(2.0*t) + QRandomGenerator::global()->bounded(-100,100)/500.0 * qCos(t/0.3843) );
        double yCleanCos = amplitude *   qCos(2.0*t);
        // plot sin
        ui->plot->graph(2)->addData(t,yNoiseCos);
        ui->plot->graph(3)->addData(t,yCleanCos);
    }

    // random annotation
    if(timePass.elapsed()/1000.0 > ui->cosDelaySec->value() + 1 ){ // must be beyond cosine delay
        if(QRandomGenerator::global()->bounded(0,100) < 1) { // 1% chance of adding an annotation
            double x1 = t ;
            if(QRandomGenerator::global()->bounded(0,100) < 25){ // 25% change of annotation having different start/end times
                x1 -= QRandomGenerator::global()->bounded(0,10)/5. ;
            }
            AddAnnotation(x1, t, FunnyFace() );
        }
    }

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot->xAxis->setRange(t, 8, Qt::AlignRight);
    ui->plot->replot();
    // update tracker
    tStop = t;
}

void TimeSeriesExample::on_start_clicked()
{
    ResetPlot();
    StartGraphs();
    StartTimer(1);
    ToggleUIStartStop(true);

    // reset scroll
    tStart = 0.;
    if(ui->axisType->currentIndex() == 2) // if using date/time axis
        tStart = QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

void TimeSeriesExample::on_stop_clicked()
{
    ToggleUIStartStop(false);
    StopTimer();
    StopGraphs();
}

void TimeSeriesExample::ResetPlot()
{
    // reset plot
    ui->plot->setInteraction(QCP::iRangeDrag, false);
    ui->plot->setInteraction(QCP::iRangeZoom, false);
    ui->plot->clearGraphs();
    ui->plot->clearItems(); // this removes amplitude lines and annotations
    ui->plot->yAxis->setRange(-1.5,1.5);
    // reset items
    SetupAmpLine();
    annRectList.clear();
    annTextList.clear();
}

void TimeSeriesExample::StartGraphs()
{
    // allow movable amplitude
    SetAmplitudeLine(amplitude);
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(OnMousePress(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(OnMouseRelease(QMouseEvent*)));
    // make graphs
    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->graph(0)->setName("Noisy Sine");
    ui->plot->graph(1)->setName("Smooth Sine");
    ui->plot->graph(2)->setName("Noisy Cosine");
    ui->plot->graph(3)->setName("Smooth Cosine");

    ui->plot->graph(0)->setPen(QPen(
        QColor(QRandomGenerator::global()->bounded(0,225),
               QRandomGenerator::global()->bounded(0,225),
               QRandomGenerator::global()->bounded(0,225) ),
        2));

    ui->plot->graph(1)->setPen(QPen(
        QColor(0,0,0,100), // transparent black
        3 ));

    ui->plot->graph(2)->setPen(QPen(
        QColor(QRandomGenerator::global()->bounded(0,225),
               QRandomGenerator::global()->bounded(0,225),
               QRandomGenerator::global()->bounded(0,225) ),
        2));

    ui->plot->graph(3)->setPen(QPen(
        QColor(0,0,0,150), // transparent black
        3 ));
}

void TimeSeriesExample::StopGraphs()
{
    // disconect all interactables
    disconnect(ui->plot, SIGNAL(mousePress(QMouseEvent*)),0,0);
    disconnect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)),0,0);
    disconnect(ui->plot, SIGNAL(mouseRelease(QMouseEvent*)),0,0);

    // plot
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // allow drag ny clicking plot and zooming using the mouse scroll wheel
    ui->plot->replot();
}

void TimeSeriesExample::AddAnnotation(double x1, double x2, QString text)
{
    // create new rectangle
    QCPItemRect * newAnnRect = new QCPItemRect(ui->plot);
    newAnnRect->position("topLeft")->setCoords(     x1,  2);
    newAnnRect->position("bottomRight")->setCoords( x2, -2);
    newAnnRect->setPen(  QPen(  QColor(200, 0, 0)));
    newAnnRect->setBrush(QBrush(QColor(200, 0, 0, 100))); // first graph will be filled with translucent blue
    // create text
    QCPItemText * newAnnText = new QCPItemText(ui->plot);
    newAnnText->position->setCoords( x1+0.05, 1.4);
    newAnnText->setPositionAlignment(Qt::AlignLeft);
    newAnnText->setColor(QColor(100, 0, 0));
    newAnnText->setText(text);
    // add to member list
    annRectList.append(newAnnRect);
    annTextList.append(newAnnText);
}

QString TimeSeriesExample::FunnyFace()
{
    QVector<QString> eyes;
    QVector<QString> mouth;
    eyes << ":" << ";" << "=" << ">:" << ">;" << "8";
    mouth << ")" << "(" << "P" << "p" << "0" << "D" << "/" << ">" << "3";
    return (eyes[ QRandomGenerator::global()->bounded(0,eyes.count()) ] + mouth[ QRandomGenerator::global()->bounded(0,mouth.count()) ]) ;
}

double TimeSeriesExample::XFromMouse(QMouseEvent *event)
{
    return( ui->plot->xAxis->pixelToCoord( event->position().x() ) );
}

double TimeSeriesExample::YFromMouse(QMouseEvent *event)
{
    return( ui->plot->yAxis->pixelToCoord( event->position().y() ) );
}

double TimeSeriesExample::ScrollStep()
{
    // scrollbar goes from 0..1000 in increments of 1
    // data goes from tStart..tStop
    return ( (tStop - tStart) / 1000.);
}

