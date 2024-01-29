#include "doodleplot.h"
#include "ui_doodleplot.h"

DoodlePlot::DoodlePlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DoodlePlot)
{
    ui->setupUi(this);

    // checkboxes
    ui->checkBox_fill->setChecked(true);
    ui->checkBox_snap->setChecked(true);
    ui->checkBox_rand->setChecked(true);

    // setup color dialog
    colorDialog = new QColorDialog ( this );
    connect ( colorDialog,SIGNAL(currentColorChanged(QColor)),SLOT(SetColor(QColor)));

    // graph
    SetupPlot();
}

DoodlePlot::~DoodlePlot()
{
    delete ui;
}

void DoodlePlot::OnMousePress(QMouseEvent *event)
{
    // build new curve
    currentCurve = new QCPCurve(ui->plot->xAxis, ui->plot->yAxis);
    currentCurve->setPen(QPen(lineColor,ui->spinBox_size->value()));
    if(ui->checkBox_fill->isChecked()){
        QColor fill = lineColor;
        fill.setAlpha(50);
        currentCurve->setBrush(QBrush(fill));
    }
    // add starting data point
    xStart = XFromMouse(event);
    yStart = YFromMouse(event);
    currentCurve->addData(xStart, yStart);
    // update ui
    ui->plot->replot();
    // draw when moveing mouse
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(OnMouseMove(QMouseEvent*)));
}

void DoodlePlot::OnMouseMove(QMouseEvent *event)
{
    // add data point to curve and update ui
    currentCurve->addData(XFromMouse(event),YFromMouse(event));
    ui->plot->replot();
}

void DoodlePlot::OnMouseRelease(QMouseEvent *event)
{
    // add final point
    OnMouseMove(event);
    // connect curve back to start
    if(ui->checkBox_snap->isChecked()){
        currentCurve->addData(xStart, yStart);
    }
    // update ui
    ui->plot->replot();
    // stop drawing
    disconnect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)),0,0);
    currentCurve = nullptr;

    // change color if random
    if(ui->checkBox_rand->isChecked()){
        on_checkBox_rand_toggled(true);
    }
}

void DoodlePlot::SetupPlot()
{
    // setup axis
    ui->plot->axisRect()->setupFullAxesBox();

    // allow user to draw shapes
    ui->plot->setMouseTracking(true);
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)),   this, SLOT(OnMousePress(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(OnMouseRelease(QMouseEvent*)));
}

void DoodlePlot::SetColor(const QColor &color )
{
    lineColor = color;

    ui->pushButton_color->setStyleSheet(
        QString("color: rgb(%1,%2,%3); background-color: rgb(%1,%2,%3);")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
        );
}

double DoodlePlot::XFromMouse(QMouseEvent *event)
{
    return( ui->plot->xAxis->pixelToCoord( event->position().x() ) );
}

double DoodlePlot::YFromMouse(QMouseEvent *event)
{
    return( ui->plot->yAxis->pixelToCoord( event->position().y() ) );
}

QColor DoodlePlot::RandomColor()
{
    return ( QColor(QRandomGenerator::global()->bounded(0,225),
                    QRandomGenerator::global()->bounded(0,225),
                    QRandomGenerator::global()->bounded(0,225) ));
}

void DoodlePlot::on_pushButton_reset_clicked(bool checked)
{
    ui->plot->clearPlottables();
    ui->plot->replot();
}


void DoodlePlot::on_pushButton_color_clicked()
{
    QColor original = lineColor;
    colorDialog->setCurrentColor( original );
    if ( colorDialog->exec() == QDialog::Rejected ){
        SetColor ( original );
    }
}

void DoodlePlot::on_checkBox_rand_toggled(bool checked)
{
    if(checked){
        SetColor(RandomColor());
    }
}

