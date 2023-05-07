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

    //gets information from status file
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
                stat.production = line;
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
            x = board.GetMap()[i].find("6");
            y = i;
            GetClosestMine(x, y);
        }
    }

    //first deciding what unit to build
    if(mineIsOnTheMap)
    {
        ShouldBuildKnight(0);
        ShouldBuildWorker();
    }
    else
    {
        ShouldBuildKnight(1);
    }

    //second ordering worker to move to a mine
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

void Commander::ShouldBuildWorker()
{
    if(CheckNumOfUnit("W") < 2 && gold >= 100)
    {
        BuildUnit("W");
        gold -= 100;
    }
}

void Commander::ShouldBuildKnight(int checker)
{
    if(CheckNumOfUnit("K") < 3 && gold >= 400)
    {
        BuildUnit("K");
        gold -= 400;
    }
    else if(CheckNumOfUnit("W") == 2 && checker == 0 && gold >= 400)
    {
        BuildUnit("K");
        gold -= 400;
    }
    else if(checker == 1 && gold >= 400)
    {
        BuildUnit("K");
        gold -= 400;
    }
}

void Commander::BuildUnit(std::string unit)
{
    std::string order;

    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(playerUnits[i].type == "B")
        {
            order = std::to_string(playerUnits[i].id);
            order += " B ";
            order += unit;
            orders.push_back(order);
            break;
        }
    }
}

int Commander::CheckNumOfUnit(std::string unit)
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

    if(mineX == -1 && mineY == -1)
    {
        mineX = x;
        mineY = y;
    }
    else if(abs(((mineX - baseX) + (mineY - baseY))) > abs(((x - baseX) + (y - baseY))))
    {
        mineX = x;
        mineY = y;
    }
}

void Commander::GiveOrderToAttack(int attacker, int defender)
{
    std::string order;
    order = std::to_string(attacker);
    order += " A ";
    order += std::to_string(defender);
    int attackerX = -1;
    int attackerY = -1;
    int defenderX = -1;
    int defenderY = -1;
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
    if(abs((attackerX - defenderX) + (attackerY - defenderY)) == 1)
    {
        orders.push_back(order);
    }
}

void Commander::GiveOrderToMove(int unitID, int destinationX, int destinationY)
{
    int id = -1;
    std::string order;
    int x = -1;
    int y = -1;
    std::string aff;
    int remainingSpeed = 0;
    int distance = 0;
    for(int i = 0; i < playerUnits.size(); i++)
    {
        if(unitID == playerUnits[i].id)
        {
            id = playerUnits[i].id;
            x = playerUnits[i].x;
            y = playerUnits[i].y;
            aff = playerUnits[i].affiliation;
            break;
        }
    }
    distance = abs((x - destinationX) + (y - destinationY));
    if(aff == "K")
    {
        remainingSpeed = 5;
        if(distance <= 5)
        {
            x = destinationX;
            y = destinationY;
            if(y > 0 && board.GetMap()[y - 1][x] != '9')
            {
                y--;
            }
            else if(y == 0 && board.GetMap()[y + 1][x] != '9')
            {
                y++;
            }
            else if(x == 0 && board.GetMap()[y][x + 1] != '9')
            {
                x++;
            }
            else
            {
                x--;
            }
        }
        if(x != destinationX && abs((x - destinationX)) > distance)
        {
            for(int i = 0; i < 5; i++)
            {
                if(destinationX > x)
                {
                    if(CheckIfFieldIsFree((x + remainingSpeed), y))
                    {
                        x += remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        x--;
                    }
                }
                else if(destinationX < x)
                {
                    if(CheckIfFieldIsFree((x - remainingSpeed), y))
                    {
                        x -= remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        x++;
                    }
                }
                else if(i == 4 && destinationX < x)
                {
                    x -= 5;
                    break;
                }
                else if(i == 4 && destinationX > x)
                {
                    x += 5;
                    break;
                }
            }
        }
        if(y != destinationY && abs((y - destinationY)) > distance)
        {
            for(int i = 0; i < 5; i++)
            {
                if(destinationY > y)
                {
                    if(CheckIfFieldIsFree(x, (y - remainingSpeed)))
                    {
                        y += remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        y--;
                    }
                }
                if(destinationY < y)
                {
                    if(CheckIfFieldIsFree(x, (y + remainingSpeed)))
                    {
                        y -= remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        y++;
                    }
                }
                else if(i == 4 && destinationY < y)
                {
                    y -= 5;
                    break;
                }
                else if(i == 4 && destinationY > y)
                {
                    y += 5;
                    break;
                }
            }
        }
    }
    if(aff == "W")
    {
        remainingSpeed = 2;
        if(distance <= 2)
        {
            x = destinationX;
            y = destinationY;
            if(y > 0 && board.GetMap()[y - 1][x] != '9')
            {
                y--;
            }
            else if(y == 0 && board.GetMap()[y + 1][x] != '9')
            {
                y++;
            }
            else if(x == 0 && board.GetMap()[y][x + 1] != '9')
            {
                x++;
            }
            else
            {
                x--;
            }
        }
        if(x != destinationX && abs((x - destinationX)) > distance)
        {
            for(int i = 0; i < 2; i++)
            {
                if(destinationX > x)
                {
                    if(CheckIfFieldIsFree((x + remainingSpeed), y))
                    {
                        x += remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        x--;
                    }
                }
                else if(destinationX < x)
                {
                    if(CheckIfFieldIsFree((x - remainingSpeed), y))
                    {
                        x -= remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        x++;
                    }
                }
                else if(i == 1 && destinationX < x)
                {
                    x -= 2;
                    break;
                }
                else if(i == 1 && destinationX > x)
                {
                    x += 2;
                    break;
                }
            }
        }
        if(y != destinationY && abs((y - destinationY)) > distance)
        {
            for(int i = 0; i < 5; i++)
            {
                if(destinationY > y)
                {
                    if(CheckIfFieldIsFree(x, (y + remainingSpeed)))
                    {
                        y += remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        y--;
                    }
                }
                if(destinationY < y)
                {
                    if(CheckIfFieldIsFree(x, (y + remainingSpeed)))
                    {
                        y -= remainingSpeed;
                        remainingSpeed -= i; 
                        break;
                    }
                    else
                    {
                        y++;
                    }
                }
                else if(i == 4 && destinationY < y)
                {
                    y -= 5;
                    break;
                }
                else if(i == 4 && destinationY > y)
                {
                    y += 5;
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

bool Commander::CheckIfFieldIsFree(int x , int y)
{
    if(board.GetMap()[y][x] != '9')
    {
        return true;
    } 
    else
    {
        return false;
    }
}

std::pair<int, int> Commander::GetEnemyBase()
{
    std::pair<int, int> cords(-1, -1);
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

int Commander::GetEnemyBaseID()
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