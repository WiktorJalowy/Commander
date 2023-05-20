#pragma once

#include <vector>
#include <string>
#include <sstream>

class Map
{
public:
    std::vector<std::string> GetMap();
    void SetMap(std::stringstream &map);
private:
    std::vector<std::string> board;
};