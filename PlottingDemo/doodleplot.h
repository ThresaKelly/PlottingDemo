#ifndef DOODLEPLOT_H
#define DOODLEPLOT_H

#include <QWidget>
#include <QCustomPlot\qcustomplot.h>

namespace Ui {
class DoodlePlot;
}

class DoodlePlot : public QWidget
{
    Q_OBJECT

public:
    explicit DoodlePlot(QWidget *parent = nullptr);
    ~DoodlePlot();

private slots:

    void SetColor( const QColor &color );
    void OnMousePress(QMouseEvent *  event);
    void OnMouseMove(QMouseEvent *  event);
    void OnMouseRelease(QMouseEvent *  event);
    void on_pushButton_reset_clicked(bool checked);
    void on_pushButton_color_clicked();

    void on_checkBox_rand_toggled(bool checked);

private:
    Ui::DoodlePlot *ui;

    QColorDialog * colorDialog;
    QColor lineColor;
    QCPCurve * currentCurve = nullptr;
    double xStart;
    double yStart;

    void SetupPlot();

    double XFromMouse(QMouseEvent *event);
    double YFromMouse(QMouseEvent *event);
    QColor RandomColor();
};

#endif // DOODLEPLOT_H
