#pragma once


#include "qcustomplot.h"

#include <QListWidgetItem>
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
    void setupLegend();

    void update();

    bool isItemExist(const QString& fileName) const;

    // slots
    void xAxisChanged(QCPRange range);
    void onFileButtonClicked(bool checked);
    void onHorzScrollBarChanged(int value);
    void onItemChanged(QListWidgetItem* item);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;

    double _oldScaleRation = 1;
    double _maxDistance = 0.0;
};
