#include "Map.hpp"
#include <fstream>

void Map::SetMap(std::stringstream &map) // method for getting map into vector, size of vector is total of Y and size of strings inside the vector are total of X
{
    std::fstream file;
    std::string line;
    map >> line;
    file.open(line);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            board.push_back(line);
        }
    }
    else
    {
        board.push_back(line);
        while(map >> line)
        {
            board.push_back(line);
        }
    }
}

std::vector<std::string> Map::GetMap()
{
    return board;
}