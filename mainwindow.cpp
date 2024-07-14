#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _parser.readCSV("C:\\Users\\aleks\\Desktop\\Plotter\\Plotter\\selected_data.csv");

    connect(ui->buttonX, &QCheckBox::toggled, this, &MainWindow::showGraphX);
    connect(ui->buttonY, &QCheckBox::toggled, this, &MainWindow::showGraphY);
    connect(ui->buttonZ, &QCheckBox::toggled, this, &MainWindow::showGraphZ);
    connect(ui->xAxisScroll, &QScrollBar::valueChanged, this, &MainWindow::onHorzScrollBarChanged);

    setupGraphs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showGraphX(bool checked)
{
    _graphX->setVisible(checked);
    _graphX->setData(_distances, _parser.xAng);
    update();
}

void MainWindow::showGraphY(bool checked)
{
    _graphY->setVisible(checked);
    _graphY->setPen(QPen(Qt::red));
    _graphY->setData(_distances, _parser.yAng);
    update();
}

void MainWindow::showGraphZ(bool checked)
{
    _graphZ->setVisible(checked);
    _graphZ->setPen(QPen(Qt::black));
    _graphZ->setData(_distances, _parser.rssi);
    update();
}

void MainWindow::setupGraphs()
{
    _graphX = ui->widget->addGraph();
    _graphY = ui->widget->addGraph();
    _graphZ = ui->widget->addGraph();

    _graphX->setName("X");
    _graphY->setName("Y");
    _graphZ->setName("Z");


    double totalDistance = 0.0;

    for (int i = 1; i < _parser.xAng.size(); ++i)
    {
        double dx = _parser.xAng[i] - _parser.xAng[i - 1];
        double dy = _parser.yAng[i] - _parser.yAng[i - 1];
        double dist = qSqrt(dx * dx + dy * dy);
        totalDistance += dist;
        _distances.append(totalDistance);
    }

    ui->widget->xAxis->setLabel("Home distance");
    ui->widget->xAxis->axisRect()->setRangeZoom(Qt::Orientation::Horizontal);
    ui->widget->xAxis->axisRect()->setRangeZoomFactor(2);
    ui->widget->yAxis->setLabel("Height");
    ui->widget->xAxis->setRange(0, totalDistance);
    ui->widget->yAxis->setRange(-90, 50);

    setupLegend();
    setupScrollBar(totalDistance);
}

void MainWindow::setupLegend()
{
    ui->widget->legend->setVisible(true);
}

void MainWindow::setupScrollBar(const double totalDist)
{
    ui->xAxisScroll->setRange(0, totalDist);
}

void MainWindow::update()
{
    ui->widget->replot();
}

void MainWindow::onHorzScrollBarChanged(int value)
{
    QCPAxis* xAxis = ui->widget->xAxis;
    if (qAbs(xAxis->range().center() - value) > 0.01)
    {
        xAxis->setRange(value, xAxis->range().size(), Qt::AlignCenter);
        update();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QCPAxis* xAxis = ui->widget->xAxis;
    const auto minRange = xAxis->range().lower;
    const auto maxRange = xAxis->range().upper;
    const auto isIncreaseScale = event->angleDelta().toPointF().y() > 0;

    if(maxRange - minRange < 200 && isIncreaseScale)
        return;

    _oldScaleRation = 1 - event->angleDelta().toPointF().y() / 1000;

    ui->widget->xAxis->scaleRange(_oldScaleRation);
    update();
}







