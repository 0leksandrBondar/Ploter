#pragma once

#include <string>
#include <QVector>
#include <filesystem>

class Parser
{
public:
    Parser() = default;

    void readCSV(const std::filesystem::path& filename);

    void toDeg();

    void reset();
    void revers();

    QVector<double> time;
    QVector<double> xAng;
    QVector<double> yAng;
    QVector<double> rssi;

    QVector<double> lat;
    QVector<double> lon;

    double homeLat;
    double homeLon;
};
