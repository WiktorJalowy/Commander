#include "lib/Commander.hpp"
#include "lib/Orders.hpp"
#include "lib/Validator.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Commander commander;
    Orders orders;
    
    orders.SaveOrders(commander.GiveOrders(argv[1], argv[2]), argv[3]);
}