#pragma once

#include <vector>
#include <string>

class Map
{
public:
    std::vector<std::string> GetMap();
    void SetMap(std::string map);
private:
    std::vector<std::string> board;
};