#pragma once

#include <string>
#include <QVector>
#include <filesystem>

class Parser
{
public:
    Parser() = default;

    void readCSV(const std::filesystem::path& filename);

    void reset();

    QVector<double> time;
    QVector<double> xAng;
    QVector<double> yAng;
    QVector<double> rssi;

    QVector<double> lat;
    QVector<double> lon;
};
