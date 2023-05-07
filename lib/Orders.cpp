#include "Orders.hpp"
#include <iostream>

void Orders::SaveOrders(const std::vector<std::string>& orders, std::string path)
{
    std::ofstream file;
    file.open(path);
    if(!file)
    {
        std::cout << "problem";
    }
    for(int i = 0; i < orders.size(); i++)
    {
        file << orders[i] << "\n";
    }
}