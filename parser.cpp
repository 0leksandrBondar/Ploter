#include "parser.h"

#include <iostream>
#include <fstream>
#include <sstream>

void Parser::readCSV(const std::filesystem::path& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Can't open file " << filename << std::endl;
        return;
    }

    std::string line;
    bool firstLineSkipped = false; // Добавляем флаг для пропуска первой строки

    while (std::getline(file, line))
    {
        if (!firstLineSkipped)
        {
            firstLineSkipped = true;
            continue; // Пропускаем первую строку
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        int count = 0;
        while (std::getline(iss, token, ','))
        {
            if (count > 3) // Убираем лишний счетчик
            {
                break;
            }
            tokens.push_back(token);
            count++;
        }

        if (tokens.size() == 4)
        {
            double timeValue = std::stod(tokens[0]);
            double xAngValue = std::stod(tokens[1]);
            double yAngValue = std::stod(tokens[2]);
            double rssiValue = std::stod(tokens[3]);

            time.push_back(timeValue);
            xAng.push_back(xAngValue);
            yAng.push_back(yAngValue);
            rssi.push_back(rssiValue);
        }
    }

    file.close();
}

void Parser::reset()
{
    time.clear();
    xAng.clear();
    yAng.clear();
    rssi.clear();
}
