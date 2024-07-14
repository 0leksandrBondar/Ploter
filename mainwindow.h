#pragma once

#include "parser.h"
#include "qcustomplot.h"

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showGraphX(bool checked);
    void showGraphY(bool checked);
    void showGraphZ(bool checked);

private:
    void setupGraphs();
    void setupLegend();
    void setupScrollBar(const double totalDist);

    void update();

// slots
    void onHorzScrollBarChanged(int value);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;
    Parser _parser;
    QVector<double> _distances;

    double _oldScaleRation = 1;

    QCPGraph *_graphX;
    QCPGraph *_graphY;
    QCPGraph *_graphZ;
};
