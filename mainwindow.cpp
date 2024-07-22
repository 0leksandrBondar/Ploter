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
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));

    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::xAxisChanged(QCPRange range)
{
    ui->xAxisScroll->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
    ui->xAxisScroll->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupLegend()
{
    ui->widget->legend->setVisible(true);
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

bool MainWindow::isItemExist(const QString& fileName) const
{
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        if (ui->listWidget->item(i)->text() == fileName)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::onFileButtonClicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select tlog"), QApplication::applicationDirPath()+"../../../deserializeFlyData/",
                                                    tr("Points Files (*.csv)"));

    if(isItemExist(fileName))
    {
        // file already exist do nothing
        return;
    }
    else
    {
        ui->fileNameLable->setText(fileName);

        setupLegend();
        Graph *item = new Graph(fileName, ui->xAxisScroll, ui->widget, ui->listWidget);
        item->initGraphs(fileName.toStdString());
        item->setCheckState(Qt::Checked);

        item->showGraphX(ui->buttonX->isChecked());
        item->showGraphY(ui->buttonY->isChecked());
        item->showGraphRSSI(ui->buttonZ->isChecked());

        ui->listWidget->addItem(item);
        update();
    }
}

void MainWindow::onHorzScrollBarChanged(int value)
{
    QCPAxis* xAxis = ui->widget->xAxis;

    if (qAbs(xAxis->range().center()-value/10.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        xAxis->setRange(value/10.0, xAxis->range().size(), Qt::AlignCenter);
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
