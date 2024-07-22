#include "graph.h"

Graph::Graph(const QString &text, QScrollBar *scrollBar, QCustomPlot *customPlot, QListWidget *parent)
    : QListWidgetItem(text, parent), _plot(customPlot), _scrollBar(scrollBar)
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
    if (!filePath.empty())
    {
        _parser.reset();
        _parser.readCSV(filePath);
    }

    double maxDistance{ 0 };

    double lastDistance = 0;
    for (int i = 0; i < _parser.lat.size(); ++i)
    {
        const double dlat = _parser.lat[i] - _parser.homeLat;
        const double dlon = _parser.lon[i] - _parser.homeLon;
        const double dist = qSqrt(dlat * dlat + dlon * dlon) * 111000;
        if (dist > lastDistance)
        {
            maxDistance = std::max(maxDistance, dist);
            _distances.push_back(dist);
            lastDistance = dist;
        }
        else
        {
            _parser.lat.erase(_parser.lat.begin() + i);
            _parser.lon.erase(_parser.lon.begin() + i);
            _parser.rssi.erase(_parser.rssi.begin() + i);
            _parser.time.erase(_parser.time.begin() + i);
            _parser.xAng.erase(_parser.xAng.begin() + i);
            _parser.yAng.erase(_parser.yAng.begin() + i);
            i--;
        }
    }

    std::reverse(_parser.rssi.begin(), _parser.rssi.end());
    std::reverse(_parser.yAng.begin(), _parser.yAng.end());
    std::reverse(_parser.xAng.begin(), _parser.xAng.end());

    _graphY->setData(_distances, _parser.yAng);
    _graphX->setData(_distances, _parser.xAng);
    _graphRSSI->setData(_distances, _parser.rssi);

    _plot->yAxis->setLabel("Height");
    _plot->xAxis->setLabel("Home distance");

    _plot->yAxis->setRange(-90, 50);
    _plot->xAxis->setRange(0, maxDistance);

    _scrollBar->setRange(0, maxDistance);
}

void Graph::showGraphY(bool checked)
{
    if(_graphY == nullptr)
        return;

    _graphY->setVisible(checked);
    _graphY->setPen(QPen(Qt::red));
}

void Graph::showGraphX(bool checked)
{
    if(_graphX == nullptr)
        return;

    _graphX->setVisible(checked);
}

void Graph::showGraphRSSI(bool checked)
{
    if(_graphRSSI == nullptr)
        return;

    _graphRSSI->setVisible(checked);
    _graphRSSI->setPen(QPen(Qt::black));
}
