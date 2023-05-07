#include "Map.hpp"
#include <fstream>

void Map::SetMap(std::string map)
{
    std::fstream file;
    std::string line;
    while(getline(file, line))
    {
        board.push_back(line);
    }
}

std::vector<std::string> Map::GetMap()
{
    return board;
}