#ifndef HEATMAPEXAMPLE_H
#define HEATMAPEXAMPLE_H

#include <QWidget>
#include <QList>
#include <qcustomplot.h>

namespace Ui {
class HeatmapExample;
}

class HeatmapExample : public QWidget
{
    Q_OBJECT

public:
    explicit HeatmapExample(QWidget *parent = nullptr);
    ~HeatmapExample();

private slots:
    void on_slider_pixels_sliderMoved(int position);
    void on_comboBox_cmap_currentIndexChanged(int index);
    void on_checkBox_interp_toggled(bool checked);
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();

    void TimerSounds();

private:
    Ui::HeatmapExample *ui;

    QCPColorMap * colorMap;

    QTimer dataTimer;
    double scaleMin = 1;
    double scaleMax = 6;
    double scale = (scaleMax + scaleMin)/ 2.;
    bool isGrowing = true;

    void SetupPlot();
    void PlotData();
};

#endif // HEATMAPEXAMPLE_H
