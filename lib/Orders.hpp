#pragma once

#include <vector>
#include <string>
#include <fstream>

class Orders
{
public:
    void SaveOrders(const std::vector<std::string>& orders, std::string path);
private:
};