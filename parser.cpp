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
    bool firstLineSkipped = false;

    while (std::getline(file, line))
    {
        if (!firstLineSkipped)
        {
            firstLineSkipped = true;
            continue;
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        int count = 0;
        while (std::getline(iss, token, ','))
        {
            if (count > 3)
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
        else if(tokens.size() == 2)
        {
            double LatValue = std::stod(tokens[0]);
            double LonValue = std::stod(tokens[1]);

            lat.push_back(LatValue);
            lon.push_back(LonValue);
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
