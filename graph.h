#pragma once

#include "qcustomplot.h"
#include "parser.h"

class Graph : public QListWidgetItem
{
public:
    Graph(const QString& text, QScrollBar *scrollBar, QCustomPlot* customPlot, QListWidget* parent = nullptr);

    void hide();
    void initGraphs(const std::filesystem::path& filePath);

    void showGraphY(bool checked);
    void showGraphX(bool checked);
    void showGraphRSSI(bool checked);

private:
    QCPGraph *_graphX = nullptr;
    QCPGraph *_graphY = nullptr;
    QCPGraph *_graphRSSI = nullptr;

    QCustomPlot* _plot = nullptr;
    QScrollBar* _scrollBar = nullptr;

    QVector<double> _distances;

    Parser _parser;
};
