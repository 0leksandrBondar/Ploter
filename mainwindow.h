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

private:
    void setupGraphs();
    void setupLegend();
    void setupScrollBar(const double totalDist);

    void update();

    // slots
    void onFileButtonClicked(bool checked);
    void onHorzScrollBarChanged(int value);
    void onItemChanged(QListWidgetItem* item);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;

    double _oldScaleRation = 1;

    QCPGraph *_graphX;
    QCPGraph *_graphY;
    QCPGraph *_graphZ;
};
