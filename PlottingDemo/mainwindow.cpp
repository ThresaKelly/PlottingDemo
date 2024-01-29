#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->examplePlot->setVisible(false);
    ui->timePlot->setVisible(false);
    ui->heatmap->setVisible(false);
    ui->doodle->setVisible(false);

    ui->radioButton_assorted->setChecked(true);
    // ui->radioButton_doodle->setChecked(true);
    // ui->radioButton_time->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_assorted_toggled(bool checked)
{
    ui->examplePlot->setVisible( checked );
}


void MainWindow::on_radioButton_time_toggled(bool checked)
{
    ui->timePlot->setVisible( checked );
}


void MainWindow::on_radioButton_heat_toggled(bool checked)
{
    ui->heatmap->setVisible( checked );
}


void MainWindow::on_radioButton_doodle_toggled(bool checked)
{
    ui->doodle->setVisible( checked );
}

