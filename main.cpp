#include "lib/Commander.hpp"
#include "lib/Orders.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Commander commander;
    Orders orders;
    orders.SaveOrders(commander.GiveOrders(std::stringstream(argv[1]), std::stringstream(argv[2])), argv[3]);
}