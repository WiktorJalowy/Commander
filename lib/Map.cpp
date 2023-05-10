#include "Map.hpp"
#include <fstream>

void Map::SetMap(std::string map) // method for getting map into vector, size of vector is total of Y and size of strings inside the vector are total of X
{
    std::fstream file;
    std::string line;
    file.open(map);
    while(getline(file, line))
    {
        board.push_back(line);
    }
}

std::vector<std::string> Map::GetMap()
{
    return board;
}