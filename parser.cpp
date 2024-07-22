#include "parser.h"

#include <fstream>
#include <sstream>

void Parser::readCSV(const std::filesystem::path &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        bool firstLineSkipped = false;

        while (std::getline(file, line))
        {
            if (!firstLineSkipped)
            {
                firstLineSkipped = true;
                continue;
            }

            std::string token;
            std::istringstream iss(line);
            std::vector<std::string> tokens;

            while(std::getline(iss, token, ','))
            {
                tokens.push_back(token);
            }

            if(tokens[0] == "GPS_GLOBAL_ORIGIN")
            {
                homeLat = std::stod(tokens[1]);
                homeLon = std::stod(tokens[2]);
            }
            else if(tokens[0] == "GLOBAL_POSITION_INT")
            {
                lat.push_back(std::stod(tokens[1]));
                lon.push_back(std::stod(tokens[2]));
            }
            else if(tokens[0] == "DEBUG_VECT")
            {
                time.push_back(std::stod(tokens[1]));
                xAng.push_back(std::stod(tokens[2]));
                yAng.push_back(std::stod(tokens[3]));
                rssi.push_back(std::stod(tokens[4]));
            }
        }
    }
    file.close();

    toDeg();
    revers();
}

void Parser::toDeg()
{
    for (int var = 0; var < lat.size(); ++var)
        lat[var] /= 10000000;

    for (int var = 0; var < lon.size(); ++var)
        lon[var] /= 10000000;

    homeLat /=  10000000;
    homeLon /=  10000000;
}

void Parser::reset()
{
    time.clear();
    xAng.clear();
    yAng.clear();
    rssi.clear();
    lat.clear();
    lon.clear();
}

void Parser::revers()
{
    //std::reverse(time.begin(), time.end());
    //std::reverse(xAng.begin(), xAng.end());
    //std::reverse(yAng.begin(), yAng.end());
    //std::reverse(rssi.begin(), rssi.end());
    //std::reverse(lat.begin(), lat.end());
    //std::reverse(lon.begin(), lon.end());
}
