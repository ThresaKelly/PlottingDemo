#include "mycoolplot.h"
#include "ui_mycoolplot.h"

MyCoolPlot::MyCoolPlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyCoolPlot)
{
    ui->setupUi(this);
    SetupSelectPlotComboBox();
}

MyCoolPlot::~MyCoolPlot()
{
    delete ui;
}

void MyCoolPlot::on_selectPlot_comboBox_currentIndexChanged(int index)
{
    // reset plot
    CleanPlot();
    // draw new plot
    switch (index) {
    case  1: BasicParabola();    break;
    case  2: BasicDecayPlot();   break;
    case  3: BusyPlot();         break;
    case  4: DateTimePlot();     break;
    case  5: BarChart();         break;
    case  6: TextItemPlot();     break;
    case  7: Curve();            break;
    case  8: RangeSelectorPlot();break;
    case  9: PokePlot();         break;
    // case  _: RealTimeExample();  break;
    }
    ui->plot->replot();
}

void MyCoolPlot::SetupSelectPlotComboBox()
{
    ui->selectPlot_comboBox->addItem("Empty");
    ui->selectPlot_comboBox->addItem("Parabola");
    ui->selectPlot_comboBox->addItem("Decay");
    ui->selectPlot_comboBox->addItem("Busy");
    ui->selectPlot_comboBox->addItem("Dates");
    ui->selectPlot_comboBox->addItem("Bar");
    ui->selectPlot_comboBox->addItem("Text");
    ui->selectPlot_comboBox->addItem("Curve");
    ui->selectPlot_comboBox->addItem("Range Select");
    ui->selectPlot_comboBox->addItem("Poke");
    // ui->selectPlot_comboBox->addItem("Real Time");
}

double MyCoolPlot::XFromMouse(QMouseEvent *event)
{
    return( ui->plot->xAxis->pixelToCoord( event->position().x() ) );
}

double MyCoolPlot::YFromMouse(QMouseEvent *event)
{
    return( ui->plot->yAxis->pixelToCoord( event->position().y() ) );
}

void MyCoolPlot::CleanPlot()
{
    // end timer
    if (dataTimer.isActive())
        dataTimer.stop();
    dataTimer.disconnect();
    // clear
    ui->plot->clearGraphs();
    ui->plot->clearItems();
    ui->plot->clearPlottables();
    // fix axis and range
    ui->plot->yAxis->setRange(0,5);
    ui->plot->xAxis->setRange(0,5);
    ui->plot->xAxis2->setRange(0,5);
    ui->plot->yAxis2->setRange(0,5);
    ui->plot->xAxis2->setVisible(false);
    ui->plot->yAxis2->setVisible(false);
    ui->plot->xAxis2->setTickLabels(false);
    ui->plot->yAxis2->setTickLabels(false);
    ui->plot->xAxis->setTickLabels(true);
    ui->plot->yAxis->setTickLabels(true);
    ui->plot->xAxis->setLabel("");
    ui->plot->yAxis->setLabel("");
    ui->plot->xAxis2->setLabel("");
    ui->plot->yAxis2->setLabel("");
    ui->plot->xAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
    ui->plot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
    ui->plot->xAxis2->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
    ui->plot->yAxis2->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
    // clear extra layout stuff
    if(ui->plot->plotLayout()->elementCount() > 1){
        ui->plot->plotLayout()->removeAt(0);
        ui->plot->plotLayout()->simplify();
    }
    ui->plot->legend->setVisible(false);
    // remove interactions
    ui->plot->setInteraction(QCP::iRangeDrag, false);
    ui->plot->setInteraction(QCP::iRangeZoom, false);
    ui->plot->setInteraction(QCP::iSelectPlottables, false);
    disconnect(ui->plot, SIGNAL(mousePress(QMouseEvent*)),0,0);
    disconnect(ui->plot, SIGNAL(mouseRelease(QMouseEvent*)),0,0);
    disconnect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)),0,0);
}

void MyCoolPlot::BasicParabola()
{
    /* Source: https://www.qcustomplot.com/index.php/tutorials/basicplotting */
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 - 1; // x goes from -1 to 1
        y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->plot->xAxis->setRange(-1, 1);
    ui->plot->yAxis->setRange(0, 1);
}

void MyCoolPlot::BasicDecayPlot()
{
    QCustomPlot * customPlot = ui->plot;
    /* Source: https://www.qcustomplot.com/index.php/tutorials/basicplotting */
    // add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), y0(251), y1(251);
    for (int i=0; i<251; ++i)
    {
        x[i] = i + 5;
        y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
        y1[i] = qExp(-i/150.0);              // exponential envelope
    }
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:
    customPlot->graph(0)->setData(x, y0);
    customPlot->graph(1)->setData(x, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    customPlot->graph(1)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MyCoolPlot::BusyPlot()
{
    QCustomPlot * customPlot = ui->plot;
    /* Source: https://www.qcustomplot.com/index.php/tutorials/basicplotting */

    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    customPlot->legend->setFont(legendFont);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    // setup for graph 0: key axis left, value axis bottom
    // will contain left maxwell-like function
    customPlot->addGraph(customPlot->yAxis, customPlot->xAxis);
    customPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    // customPlot->graph(0)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // fill with texture of specified image
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    customPlot->graph(0)->setName("Left maxwell function");

    // setup for graph 1: key axis bottom, value axis left (those are the default axes)
    // will contain bottom maxwell-like function with error bars
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    // customPlot->graph(1)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // same fill as we used for graph 0
    customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    customPlot->graph(1)->setName("Bottom maxwell function");
    QCPErrorBars *errorBars = new QCPErrorBars(customPlot->xAxis, customPlot->yAxis);
    errorBars->removeFromLegend();
    errorBars->setDataPlottable(customPlot->graph(1));

    // setup for graph 2: key axis top, value axis right
    // will contain high frequency sine with low frequency beating:
    customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    customPlot->graph(2)->setPen(QPen(Qt::blue));
    customPlot->graph(2)->setName("High frequency sine");

    // setup for graph 3: same axes as graph 2
    // will contain low frequency beating envelope of graph 2
    customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidthF(4);
    customPlot->graph(3)->setPen(blueDotPen);
    customPlot->graph(3)->setName("Sine envelope");

    // setup for graph 4: key axis right, value axis top
    // will contain parabolically distributed data points with some random perturbance
    customPlot->addGraph(customPlot->yAxis2, customPlot->xAxis2);
    customPlot->graph(4)->setPen(QColor(50, 50, 50, 255));
    customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(4)->setName("Some random data around\na quadratic function");

    // generate data, just playing with numbers, not much to learn here:
    QVector<double> x0(25), y0(25);
    QVector<double> x1(15), y1(15), y1err(15);
    QVector<double> x2(250), y2(250);
    QVector<double> x3(250), y3(250);
    QVector<double> x4(250), y4(250);
    for (int i=0; i<25; ++i) // data for graph 0
    {
        x0[i] = 3*i/25.0;
        y0[i] = qExp(-x0[i]*x0[i]*0.8)*(x0[i]*x0[i]+x0[i]);
    }
    for (int i=0; i<15; ++i) // data for graph 1
    {
        x1[i] = 3*i/15.0;;
        y1[i] = qExp(-x1[i]*x1[i])*(x1[i]*x1[i])*2.6;
        y1err[i] = y1[i]*0.25;
    }
    for (int i=0; i<250; ++i) // data for graphs 2, 3 and 4
    {
        x2[i] = i/250.0*3*M_PI;
        x3[i] = x2[i];
        x4[i] = i/250.0*100-50;
        y2[i] = qSin(x2[i]*12)*qCos(x2[i])*10;
        y3[i] = qCos(x3[i])*10;
        y4[i] = 0.01*x4[i]*x4[i] + 1.5*(rand()/(double)RAND_MAX-0.5) + 1.5*M_PI;
    }

    // pass data points to graphs:
    customPlot->graph(0)->setData(x0, y0);
    customPlot->graph(1)->setData(x1, y1);
    errorBars->setData(y1err);
    customPlot->graph(2)->setData(x2, y2);
    customPlot->graph(3)->setData(x3, y3);
    customPlot->graph(4)->setData(x4, y4);
    // activate top and right axes, which are invisible by default:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    // set ranges appropriate to show data:
    customPlot->xAxis->setRange(0, 2.7);
    customPlot->yAxis->setRange(0, 2.6);
    customPlot->xAxis2->setRange(0, 3.0*M_PI);
    customPlot->yAxis2->setRange(-70, 35);
    // set pi ticks on top axis:
    customPlot->xAxis2->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));
    // add title layout element:
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Way too many graphs in one plot", QFont("sans", 12, QFont::Bold)));
    // set labels:
    customPlot->xAxis->setLabel("Bottom axis with outward ticks");
    customPlot->yAxis->setLabel("Left axis label");
    customPlot->xAxis2->setLabel("Top axis label");
    customPlot->yAxis2->setLabel("Right axis label");
    // make ticks on bottom axis go outward:
    customPlot->xAxis->setTickLength(0, 5);
    customPlot->xAxis->setSubTickLength(0, 3);
    // make ticks on right axis go inward and outward:
    customPlot->yAxis2->setTickLength(3, 3);
    customPlot->yAxis2->setSubTickLength(1, 1);
}

void MyCoolPlot::DateTimePlot()
{
    QCustomPlot * customPlot = ui->plot;
    /* Source: https://www.qcustomplot.com/index.php/tutorials/basicplotting */

    // set locale to english, so we get english month names:
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    // seconds of current time, we'll use it as starting point in time for data:
    double now = QDateTime::currentDateTime().toSecsSinceEpoch(); //.toTime_t(); depreciated
    srand(8); // set the random seed, so we always get the same random data
    // create multiple graphs:
    for (int gi=0; gi<5; ++gi)
    {
        customPlot->addGraph();
        QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(QPen(color.lighter(200)));
        customPlot->graph()->setBrush(QBrush(color));
        // generate random walk data:
        QVector<QCPGraphData> timeData(250);
        for (int i=0; i<250; ++i)
        {
            timeData[i].key = now + 24*3600*i;
            if (i == 0)
                timeData[i].value = (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
            else
                timeData[i].value = qFabs(timeData[i-1].value)*(1+0.02/4.0*(4-gi)) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
        }
        customPlot->graph()->data()->set(timeData);
    }
    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    customPlot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(10, "a bit\nlow");
    textTicker->addTick(50, "quite\nhigh");
    customPlot->yAxis->setTicker(textTicker);
    // set a more compact font size for bottom and left axis tick labels:
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    customPlot->xAxis->setLabel("Date");
    customPlot->yAxis->setLabel("Random wobbly lines value");
    // make top and right axes visible but without ticks and labels:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    customPlot->xAxis->setRange(now, now+24*3600*249);
    customPlot->yAxis->setRange(0, 60);
    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));
}

void MyCoolPlot::BarChart()
{
    QCustomPlot * customPlot = ui->plot;
    /* Source: https://www.qcustomplot.com/index.php/tutorials/basicplotting */

    QCPBars *myBars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    // now we can modify properties of myBars:
    myBars->setName("Bars Series 1");
    QVector<double> keyData;
    QVector<double> valueData;
    keyData << 1 << 2 << 3;
    valueData << 2 << 4 << 8;
    myBars->setData(keyData, valueData);
    customPlot->rescaleAxes();
    customPlot->replot();
}

void MyCoolPlot::TextItemPlot()
{
    QCustomPlot * customPlot = ui->plot;
    /* Source: https://www.qcustomplot.com/index.php/tutorials/items */

    // add the text label at the top:
    QCPItemText *textLabel = new QCPItemText(customPlot);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    textLabel->setText("Text Item Demo");
    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    textLabel->setPen(QPen(Qt::black)); // show black border around text

    // add the arrow:
    QCPItemLine *arrow = new QCPItemLine(customPlot);
    arrow->start->setParentAnchor(textLabel->bottom);
    arrow->end->setCoords(4, 1.6); // point to (4, 1.6) in x-y-plot coordinates
    arrow->setHead(QCPLineEnding::esSpikeArrow);
}

//////////////////////////////////////////////////////////////////////////

void MyCoolPlot::RangeSelectorPlot()
{
    ui->plot->setMouseTracking(true);
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)),   this, SLOT(OnMousePress_Range(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(OnMouseRelease_Range(QMouseEvent*)));
    ui->plot->xAxis->setRange(-1, 1);

    QVector<double> x;
    QVector<double> y;
    x << -0.5 << -0.5 << 0.5 << 0.5;
    y << 0 << 5 << 5 << 0;

    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::black));
    ui->plot->graph(0)->setBrush(QBrush(QColor(0, 0, 0, 20)));
    ui->plot->graph(0)->addData(x, y);
    ui->plot->replot();
}

void MyCoolPlot::OnMousePress_Range(QMouseEvent *event)
{
    // allow mouse move action
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(OnMouseMove_Range(QMouseEvent*)));
    // read coordinate
    range_x1 = XFromMouse(event);
    // data
    QVector<double> x;
    QVector<double> y;
    x << range_x1 << range_x1;
    y << 0 << 5;
    // start plot
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::black));
    ui->plot->graph(0)->setBrush(QBrush(QColor(0, 0, 0, 20))); // transparent area under grah
    ui->plot->graph(0)->addData(x, y);
    ui->plot->replot();
}

void MyCoolPlot::OnMouseMove_Range(QMouseEvent *event)
{
    // read mouse coord
    range_x2 = XFromMouse(event);
    // order smallest x on left largest x on right
    QVector<double> x;
    if(range_x1 < range_x2){
        x << range_x1 << range_x1 << range_x2 << range_x2;
    }
    else{
        x << range_x2 << range_x2 << range_x1 << range_x1;
    }
    QVector<double> y;
    y << 0 << 5 << 5 << 0;
    // plot range
    ui->plot->graph(0)->setData(x, y);
    ui->plot->replot();
}

void MyCoolPlot::OnMouseRelease_Range(QMouseEvent *event)
{
    // draw final range
    OnMouseMove_Range(event);
    // show final range
    qDebug() << "Range: ("+QString::number(range_x1) +", " + QString::number(range_x2)+") -- "+QString::number(abs(range_x2-range_x1));
    // finish drawing range
    disconnect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)),0,0);
}

//////////////////////////////////////////////////////////////////////////
void MyCoolPlot::OnMousePress_Poke(QMouseEvent *event)
{
    // setup data
    QVector<double> x(1);
    QVector<double> y(1);
    x[0] = XFromMouse(event);
    y[0] = YFromMouse(event);
    // plot
    ui->plot->graph(0)->addData(x, y);
    ui->plot->replot();
}


void MyCoolPlot::PokePlot()
{
    // setup plot
    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::magenta));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
    // draw point when clicking plot
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(OnMousePress_Poke(QMouseEvent*)));
}
//////////////////////////////////////////////////////////////////////////


void MyCoolPlot::RealtimeDataSlot()
{
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
        ui->plot->graph(0)->addData(key, qSin(key)+QRandomGenerator::global()->bounded(-10,10)/20.0 *qSin(key/0.3843));
        ui->plot->graph(1)->addData(key, qCos(key));
        // rescale value (vertical) axis to fit the current data:
        ui->plot->graph(0)->rescaleValueAxis();
        ui->plot->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        qDebug() << QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                        .arg(ui->plot->graph(0)->data()->size() + ui->plot->graph(1)->data()->size());
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MyCoolPlot::RealTimeExample()
{
    QCustomPlot * customPlot = ui->plot;
    /* Source: https://www.qcustomplot.com/index.php/demos/realtimedatademo */

    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
    time.start();
    dataTimer.start(0);
}

void MyCoolPlot::Curve()
{
    QCPCurve * deltoidRadial = new QCPCurve(ui->plot->xAxis, ui->plot->yAxis);

    // generate the curve data points:
    const int pointCount = 500;
    QVector<QCPCurveData> dataDeltoid(pointCount);
    for (int i=0; i<pointCount; ++i)
    {
        double theta = i/(double)(pointCount-1)*2*M_PI;
        dataDeltoid[i] = QCPCurveData(i, 2*qCos(2*theta)+qCos(1*theta)+2*qSin(theta), 2*qSin(2*theta)-qSin(1*theta));
    }
    // pass the data to the curves; we know t (i in loop above) is ascending, so set alreadySorted=true (saves an extra internal sort):
    deltoidRadial->data()->set(dataDeltoid, true);
    // color the curves:
    QRadialGradient radialGrad(QPointF(310, 180), 200);
    radialGrad.setColorAt(0, QColor(170, 20, 240, 100));
    radialGrad.setColorAt(0.5, QColor(20, 10, 255, 40));
    radialGrad.setColorAt(1,QColor(120, 20, 240, 10));
    deltoidRadial->setPen(QPen(QColor(170, 20, 240)));
    deltoidRadial->setBrush(QBrush(radialGrad));
    // set some basic customPlot config:
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->rescaleAxes();
}

