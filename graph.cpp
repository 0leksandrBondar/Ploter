#include "graph.h"

Graph::Graph(const QString &text, QCustomPlot *customPlot, QListWidget *parent) : QListWidgetItem(text, parent), _plot(customPlot)
{
    _graphX = customPlot->addGraph();
    _graphY = customPlot->addGraph();
    _graphRSSI = customPlot->addGraph();

    _graphX->setName("X");
    _graphY->setName("Y");
    _graphRSSI->setName("Z");

}

void Graph::hide()
{
    _graphX->setVisible(false);
    _graphY->setVisible(false);
    _graphRSSI->setVisible(false);
}

void Graph::initGraphs(const std::filesystem::path &filePath)
{
    if(!filePath.empty())
    {
        _parser.reset();
        _parser.readCSV(filePath);
    }

    double maxDistance{ 0 };

    for (int i = 1; i < _parser.xAng.size(); ++i)
    {
        double dx = _parser.xAng[i];
        double dy = _parser.yAng[i];
        double dz = _parser.rssi[i];
        double dist = qSqrt(dx * dx + dy * dy + dz * dz);

        maxDistance = std::max(maxDistance, dist);

        _distances.append(dist);
    }

    _plot->yAxis->setLabel("Height");
    _plot->xAxis->setLabel("Home distance");

    _plot->yAxis->setRange(-90, 50);
    _plot->xAxis->setRange(0, maxDistance * 1.2);
}

void Graph::showGraphY(bool checked)
{
    if(_graphY == nullptr)
        return;

    _graphY->setVisible(checked);
    _graphY->setPen(QPen(Qt::red));
    _graphY->setData(_distances, _parser.yAng);
}

void Graph::showGraphX(bool checked)
{
    if(_graphX == nullptr)
        return;

    _graphX->setVisible(checked);
    _graphX->setData(_distances, _parser.xAng);
}

void Graph::showGraphRSSI(bool checked)
{
    if(_graphRSSI == nullptr)
        return;

    _graphRSSI->setVisible(checked);
    _graphRSSI->setPen(QPen(Qt::black));
    _graphRSSI->setData(_distances, _parser.rssi);
}
