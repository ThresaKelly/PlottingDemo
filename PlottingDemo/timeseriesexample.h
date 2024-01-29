#ifndef TIMESERIESEXAMPLE_H
#define TIMESERIESEXAMPLE_H

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <qcustomplot.h>

namespace Ui {
class TimeSeriesExample;
}

class TimeSeriesExample : public QWidget
{
    Q_OBJECT

public:
    explicit TimeSeriesExample(QWidget *parent = nullptr);
    ~TimeSeriesExample();

private slots:
    void RealtimeDataSlot();
    void DelayedRealtimeDataSlot();
    void OnMousePress(QMouseEvent *  event);
    void OnMouseMove(QMouseEvent *  event);
    void OnMouseRelease(QMouseEvent *  event);
    void OnXAxisChanged(QCPRange range);
    void on_start_clicked();
    void on_stop_clicked();
    void on_axisType_currentIndexChanged(int index);
    void on_horizontalPlotScroll_valueChanged(int value);

private:
    Ui::TimeSeriesExample *ui;

    // time sensitive vars
    QElapsedTimer timePass;
    QTimer dataTimer;
    QTimer delayTimer;
    bool plotDelay = false;
    double tStart = 0.0;
    double tStop = 0.0;
    bool lockScroll = false;

    // amplitude line trackers
    double amplitude = 1;
    QCPItemStraightLine * amplitudeLine1 = nullptr;
    QCPItemStraightLine * amplitudeLine2 = nullptr;

    // annotations
    QList<QCPItemRect*> annRectList;
    QList<QCPItemText*> annTextList;

    void SetupAxisOptions();
    void SetupPlot();
    void SetupAmpLine();
    void SetAmplitudeLine(double a);

    void ResetPlot();
    void StartGraphs();
    void StopGraphs();

    void ToggleUIStartStop(bool state);
    void StartTimer(int nsec = 0);
    void StopTimer();

    void AddAnnotation(double x1, double x2, QString text);
    QString FunnyFace();

    double XFromMouse(QMouseEvent *event);
    double YFromMouse(QMouseEvent *event);

    double ScrollStep();
};

#endif // TIMESERIESEXAMPLE_H
