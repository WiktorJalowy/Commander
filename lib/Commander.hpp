#pragma once

#include "Map.hpp"
#include "Status.hpp"

class Commander
{
public:
    std::vector<std::string> GiveOrders(std::string map, std::string status);
    
private:
    void ShouldBuildWorker();
    void ShouldBuildKnight(int checker);
    void BuildUnit(std::string unit);

    std::pair<int, int> GetEnemyBase();
    int GetEnemyBaseID();
    void GiveOrderToMove(int unitID, int destinationX, int destinationY);
    void GiveOrderToAttack(int attacker, int defender);
    bool CheckIfFieldIsFree(int x, int y);

    int CheckNumOfUnit(std::string unit);
    void GetClosestMine(int x, int y);

    std::vector<std::string> orders;
    std::vector<Status> enemyUnits;
    std::vector<Status> playerUnits;
    
    Map board;
    int mineX = -1;
    int mineY = -1;
    int gold = 0;
    std::string production = "0";
};