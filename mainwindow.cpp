#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graph.h"


#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonX, &QCheckBox::toggled, this, &MainWindow::update);
    connect(ui->buttonY, &QCheckBox::toggled, this, &MainWindow::update);
    connect(ui->buttonZ, &QCheckBox::toggled, this, &MainWindow::update);
    connect(ui->listWidget, &QListWidget::itemChanged, this, &MainWindow::onItemChanged);
    connect(ui->fileButton, &QPushButton::clicked, this, &MainWindow::onFileButtonClicked);
    connect(ui->xAxisScroll, &QScrollBar::valueChanged, this, &MainWindow::onHorzScrollBarChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGraphs()
{
    setupLegend();
    //setupScrollBar(maxDistance);
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
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        Graph* item =  dynamic_cast<Graph*>(ui->listWidget->item(i));
        if(item->checkState() == Qt::Checked)
        {
            item->showGraphX(ui->buttonX->isChecked());
            item->showGraphY(ui->buttonY->isChecked());
            item->showGraphRSSI(ui->buttonZ->isChecked());
        }
        else
        {
            item->hide();
        }
    }

    ui->widget->replot();
}

void MainWindow::onFileButtonClicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select tlog"), QApplication::applicationDirPath()+"../../../deserializeFlyData/",
                                                    tr("Points Files (*.csv)"));

    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        if (ui->listWidget->item(i)->text() == fileName)
        {
            // file already exist do nothing
            return;
        }
    }

    ui->fileNameLable->setText(fileName);


    setupGraphs();
    Graph *item = new Graph(fileName,ui->widget, ui->listWidget);
    item->initGraphs(fileName.toStdString());
    item->setCheckState(Qt::Checked);

    item->showGraphX(ui->buttonX->isChecked());
    item->showGraphY(ui->buttonY->isChecked());
    item->showGraphRSSI(ui->buttonZ->isChecked());

    ui->listWidget->addItem(item);
    update();
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

void MainWindow::onItemChanged(QListWidgetItem *item)
{
    update();
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QCPAxis* xAxis = ui->widget->xAxis;
    const auto minRange = xAxis->range().lower;
    const auto maxRange = xAxis->range().upper;
    const auto isIncreaseScale = event->angleDelta().toPointF().y() > 0;

    if(maxRange - minRange < 5 && isIncreaseScale)
        return;

    _oldScaleRation = 1 - event->angleDelta().toPointF().y() / 1000;

    ui->widget->xAxis->scaleRange(_oldScaleRation);
    update();
}
