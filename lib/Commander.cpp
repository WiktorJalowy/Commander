#include "Commander.hpp"
#include <fstream>

std::vector<std::string> Commander::GiveOrders(std::string map, std::string status)
{
    board.SetMap(map);
    orders.clear();
    std::fstream file;
    std::string line;
    bool mineIsOnTheMap = false;
    int helper = 0;

    //gets information from status file and pushing it to different vectors, one vector for player units and one for enemy units
    file.open(status);
    while(getline(file, line))
    {
        if(helper > 0)
        {
            Status stat;
            stat.affiliation = line[0];
            line = line.substr(2);
            stat.type = line[0];
            line = line.substr(2);
            stat.id = stoi(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1);
            stat.x = stoi(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1);
            stat.y = stoi(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1);
            stat.hitpoints = stoi(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1);
            if(stat.type == "B")
            {
                stat.production = line[0];
                production = stat.production;
            }
            if(stat.affiliation == "E")
            {
                enemyUnits.push_back(stat);
            }
            else if(stat.affiliation == "P")
            {
                playerUnits.push_back(stat);
            }
        }
        else
        {
            gold = stoi(line.substr(0, line.find("\n")));
            helper++;
        }
    }

    //checks if mine is on the map and gets closest mine to the base
    for(int i = 0; i < board.GetMap().size(); i++)
    {
        int x = 0;
        int y = 0;
        if(board.GetMap()[i].find("6") != std::string::npos)
        {
            mineIsOnTheMap = true;
            x = (board.GetMap()[i].find("6") / 2);
            y = i;
            GetClosestMine(x, y);
        }
    }

    //first deciding what unit to build, prioritizing building Knights over Workers for first turns
    if(mineIsOnTheMap)
    {
        if(CheckNumOfUnit("K") < 3)
        {
            BuildUnit("K");
            gold -= 400;
        }
        else if(CheckNumOfUnit("K") == 3 && CheckNumOfUnit("W") < 2)
        {
            BuildUnit("W");
            gold -= 100;
        }
        else if(CheckNumOfUnit("W") == 2)
        {
            BuildUnit("K");
            gold -= 400;
        }
    }
    else
    {
        BuildUnit("K");
        gold -= 400;
    }

    //second ordering workers to move to a mine
    if(CheckNumOfUnit("W") > 0)
    {
        std::string str;
        for(int i = 0; i < playerUnits.size(); i++)
        {
            if(playerUnits[i].type == "W")
            {
                GiveOrderToMove(playerUnits[i].id, mineX, mineY);
            }
        }
    }

    //third ordering knights to move towards enemy base
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].type == "K")
        {
            GiveOrderToMove(playerUnits[i].id, GetEnemyBase().first, GetEnemyBase().second);
        }
    }

    //fourth ordering knights to attack enemy base
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].type == "K")
        {
            GiveOrderToAttack(playerUnits[i].id, GetEnemyBaseID());
        }
    }
    return orders;
}

void Commander::BuildUnit(std::string unit) //creating order to build unit type passed by argument
{
    std::string order;

    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].type == "B" && playerUnits[i].production == "0")
        {
            order = std::to_string(playerUnits[i].id);
            order += " B ";
            order += unit;
            orders.push_back(order);
            break;
        }
    }
}

int Commander::CheckNumOfUnit(std::string unit) //simple check for quantity of units by type
{
    int numOfUnits = 0;
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].type == unit)
        {
            numOfUnits++;
        }
    }
    return numOfUnits;
}

void Commander::GetClosestMine(int x, int y) //calculates the distance between mine and base and then saves the coordinates of closest mine
{
    int base = 0;
    int baseX = 0;
    int baseY = 0;
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].type == "B")
        {
            base = playerUnits[i].id;
            baseX = playerUnits[i].x;
            baseY = playerUnits[i].y;
        }
    }

    if(firstmine)
    {
        mineX = x;
        mineY = y;
        firstmine = false;
    }
    else if(abs(((mineX - baseX) + (mineY - baseY))) > abs(((x - baseX) + (y - baseY))))
    {
        mineX = x;
        mineY = y;
    }
}

void Commander::GiveOrderToAttack(int attacker, int defender) // Order to attack checks if attack can be done and if yes it pushes it to orders vector
{
    std::string order;
    order = std::to_string(attacker);
    order += " A ";
    order += std::to_string(defender);
    int attackerX;
    int attackerY;
    int defenderX;
    int defenderY;
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].id == attacker)
        {
            attackerX = playerUnits[i].x;
            attackerY = playerUnits[i].y;
        }
    }
    for(int i = 0; i < enemyUnits.size(); i++)
    {
        if(enemyUnits[i].id == defender)
        {
            defenderX = enemyUnits[i].x;
            defenderY = enemyUnits[i].y;
        }
    }
    if((abs((attackerX - defenderX) + abs((attackerY - defenderY)))) == 1)
    {
        orders.push_back(order);
    }
}

void Commander::GiveOrderToMove(int unitID, int destinationX, int destinationY) // this order decides on many things
{
    int id;
    std::string order;
    int x;
    int y;
    std::string type;
    int remainingSpeed = 0;
    int distance = 0;
    // first of all simple loop to calculate speed of unit and get it most important things out of playerUnits vector
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(unitID == playerUnits[i].id)
        {
            id = playerUnits[i].id;
            x = playerUnits[i].x;
            y = playerUnits[i].y;
            type = playerUnits[i].type;
            if(playerUnits[i].type == "K")
            {
                remainingSpeed = 5;
            }
            else
            {
                remainingSpeed = 2;
            }
            break;
        }
    }
    // calculating distance
    distance = abs((x - destinationX) + (y - destinationY));

    if(type == "W")
    {
        //first if is for checking if the unit can move straight to the point
        if(distance <= 2)
        {
            x = destinationX;
            y = destinationY;
            remainingSpeed = 0;
        }
        // if destination is not reachable instantly, it first moves on X axis and check if it is available to stand on to travel as much distance as possible
        for(int i = remainingSpeed; i > 0; i--)
        {
            if((x + i) > destinationX) { continue; }
            if(CheckIfFieldIsFree(x + i, y))
            {
                x += i;
                remainingSpeed -= i;
                break;
            }
        }
        // this time Y axis if any speed remains it checks if it can move and stand to travel as much distance as possible
        for(int i = remainingSpeed; i > 0; i--)
        {
            if((y + i) > destinationX) { continue; }
            if(CheckIfFieldIsFree(x, y + i))
            {
                y += i;
                remainingSpeed -= i;
                break;
            }
        }
    }
    else if(type == "K")
    {
        if(distance <= 6)
        {
            x = destinationX;
            y = destinationY;
            remainingSpeed = 0;
        }
        if(distance <= 6)
        {
            if(destinationX > 0)
            {
                x = destinationX - 1;
            }
            else if(destinationY > 0)
            {
                y = destinationY - 1;
            }
            else if(destinationY == 0 && destinationX == 0)
            {
                x = destinationX + 1;
            }
        }
        else
        {
            for(int i = remainingSpeed; i > 0; i--)
            {
                if((x + i) > destinationX) { continue; }
                if(CheckIfFieldIsFree(x + i, y))
                {
                    x += i;
                    remainingSpeed -= i;
                    break;
                }
            }
            for(int i = remainingSpeed; i > 0; i--)
            {
                if((y + i) > destinationX) { continue; }
                if(CheckIfFieldIsFree(x, y + i))
                {
                    y += i;
                    remainingSpeed -= i;
                    break;
                }
            }
        }
    }
    order = std::to_string(id);
    order += " M ";
    order += std::to_string(x);
    order += " ";
    order += std::to_string(y);
    orders.push_back(order);
}

bool Commander::CheckIfFieldIsFree(int x , int y) // method for checking if field on coordinates is not an obstacle or enemy occupies it
{
    for(int i = 0; i < enemyUnits.size(); i++)
    {
        if(board.GetMap()[y][x * 2] != '9' && board.GetMap()[y].size() > x && board.GetMap().size() > y && enemyUnits[i].x != x && enemyUnits[i].y != y)
        {
            return true;
        }
    }
    return false;
}

std::pair<int, int> Commander::GetEnemyBase() // simple method for getting coordinates of enemy base
{
    std::pair<int, int> cords;
    for(int i = 0; i < enemyUnits.size(); i++)
    {
        if(enemyUnits[i].type == "B")
        {
            cords.first = enemyUnits[i].x;
            cords.second = enemyUnits[i].y;
            break;
        }
    }
    return cords;
}

int Commander::GetEnemyBaseID() // simple method for getting id of enemy base
{
    int base;
    for(int i = 0; i < enemyUnits.size(); i++)
    {
        if(enemyUnits[i].type == "B")
        {
            base = enemyUnits[i].id;
        }
    }
    return base;
}