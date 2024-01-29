#include "heatmapexample.h"
#include "ui_heatmapexample.h"

HeatmapExample::HeatmapExample(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HeatmapExample)
{
    ui->setupUi(this);

    SetupPlot();

    ui->pushButton_stop->setVisible(false);
}

HeatmapExample::~HeatmapExample()
{
    delete ui;
}

void HeatmapExample::SetupPlot()
{
    QCustomPlot * customPlot = ui->plot;

    // configure axis rect:
    customPlot->axisRect()->setupFullAxesBox(true);

    // set up the QCPColorMap:
    colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    PlotData();

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorScale->axis()->setLabel("Magnetic Field Strength");

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    customPlot->rescaleAxes();
    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // select UI elements
    ui->comboBox_cmap->setCurrentIndex(8);
    ui->checkBox_interp->setChecked(true); // toggle to trigger signal
    ui->checkBox_interp->setChecked(false);
}

void HeatmapExample::PlotData()
{
    double s = ui->slider_pixels->value();
    colorMap->data()->setSize(s, s);

    double x, y, z;
    for (int xIndex=0; xIndex<s; ++xIndex)
    {
        for (int yIndex=0; yIndex<s; ++yIndex)
        {
            colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            double r = scale*qSqrt(x*x+y*y)+1e-2;
            z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
            colorMap->data()->setCell(xIndex, yIndex, z);
        }
    }
}

void HeatmapExample::on_slider_pixels_sliderMoved(int position)
{
    PlotData();
    ui->plot->replot();
}

void HeatmapExample::on_comboBox_cmap_currentIndexChanged(int index)
{
    // set the color gradient of the color map to one of the presets:
    switch(index){
    case  0 : colorMap->setGradient(QCPColorGradient::gpGrayscale   ); break;
    case  1 : colorMap->setGradient(QCPColorGradient::gpHot         ); break;
    case  2 : colorMap->setGradient(QCPColorGradient::gpCold        ); break;
    case  3 : colorMap->setGradient(QCPColorGradient::gpNight       ); break;
    case  4 : colorMap->setGradient(QCPColorGradient::gpCandy       ); break;
    case  5 : colorMap->setGradient(QCPColorGradient::gpGeography   ); break;
    case  6 : colorMap->setGradient(QCPColorGradient::gpIon         ); break;
    case  7 : colorMap->setGradient(QCPColorGradient::gpThermal     ); break;
    case  8 : colorMap->setGradient(QCPColorGradient::gpPolar       ); break;
    case  9 : colorMap->setGradient(QCPColorGradient::gpSpectrum    ); break;
    case 10 : colorMap->setGradient(QCPColorGradient::gpJet         ); break;
    case 11 : colorMap->setGradient(QCPColorGradient::gpHues        ); break;
    }
    // redraw plot
    ui->plot->replot();
}

void HeatmapExample::on_checkBox_interp_toggled(bool checked)
{
    // grid when false, smooth gradient when true
    colorMap->setInterpolate(checked);
    ui->plot->replot();
}


void HeatmapExample::on_pushButton_start_clicked()
{
    // toggle buttons
    ui->pushButton_start->setVisible(false);
    ui->pushButton_stop->setVisible(true);

    // setup a timer that repeatedly calls realtimeDataSlot
    connect(&dataTimer,  SIGNAL(timeout()), this, SLOT(TimerSounds()));
    dataTimer.start(10);
}


void HeatmapExample::on_pushButton_stop_clicked()
{
    // stop timers
    if (dataTimer.isActive()) dataTimer.stop();
    dataTimer.disconnect();

    // toggle buttons
    ui->pushButton_start->setVisible(true);
    ui->pushButton_stop->setVisible(false);
}

void HeatmapExample::TimerSounds()
{
    if(     scale > scaleMax) isGrowing = false;
    else if(scale < scaleMin) isGrowing = true ;

    if(isGrowing) scale += 0.01;
    else scale -= 0.01;

    PlotData();
    ui->plot->replot();
}

