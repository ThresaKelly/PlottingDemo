#ifndef MYCOOLPLOT_H
#define MYCOOLPLOT_H

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

namespace Ui {
class MyCoolPlot;
}

class MyCoolPlot : public QWidget
{
    Q_OBJECT

public:
    explicit MyCoolPlot(QWidget *parent = nullptr);
    ~MyCoolPlot();

    void CleanPlot();
    void BasicParabola();
    void BasicDecayPlot();
    void BusyPlot();
    void DateTimePlot();
    void BarChart();
    void TextItemPlot();
    void RangeSelectorPlot();
    void PokePlot();
    void RealTimeExample();
    void Curve();

private slots:
    void on_selectPlot_comboBox_currentIndexChanged(int index);
    void OnMousePress_Range(QMouseEvent *  event);
    void OnMouseMove_Range(QMouseEvent *  event);
    void OnMouseRelease_Range(QMouseEvent *  event);
    void OnMousePress_Poke(QMouseEvent *  event);
    void RealtimeDataSlot();

private:
    Ui::MyCoolPlot *ui;

    double range_x1 = 0.0;
    double range_x2 = 0.0;

    QTimer dataTimer;
    QElapsedTimer time;

    void SetupSelectPlotComboBox();
    double XFromMouse(QMouseEvent *event);
    double YFromMouse(QMouseEvent *event);


};

#endif // MYCOOLPLOT_H
