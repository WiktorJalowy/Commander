#include "../Commander.hpp"
#include "gtest/gtest.h"

TEST(CommmanderTests, ShouldMoveToFieldNextToEnemyBase)
{
    Commander commander;
    commander.GiveOrders(std::stringstream("00000 00009 00000"), std::stringstream("400 P B 0 0 0 200 0 E B 1 2 2 200 A P K 2 0 0 80"));
    std::string str = commander.GetElement(1);
    EXPECT_EQ(str[4], '1');
    EXPECT_EQ(str[6], '2');
}

TEST(CommmanderTests, ShouldMoveToFieldAboveEnemyBase)
{
    Commander commander;
    commander.GiveOrders(std::stringstream("00000 00009 09000"), std::stringstream("400 P B 0 0 0 200 0 E B 1 2 2 200 A P K 2 0 0 80"));
    std::string str = commander.GetElement(1);
    EXPECT_EQ(str[4], '2');
    EXPECT_EQ(str[6], '1');
}

TEST(CommmanderTests, ShouldMoveToFieldWithoutStandingOnOccupiedField)
{
    Commander commander;
    commander.GiveOrders(std::stringstream("00000 00009 00000"), std::stringstream("400 P B 0 0 0 200 0 E B 1 4 2 200 A P K 2 0 0 80"));
    std::string str = commander.GetElement(1);
    EXPECT_EQ(str[4], '4');
    EXPECT_EQ(str[6], '0');
}

TEST(CommmanderTests, ShouldMoveToMine)
{
    Commander commander;
    commander.GiveOrders(std::stringstream("00600 00009 00000"), std::stringstream("400 P B 0 0 0 200 0 E B 1 4 2 200 A P W 2 0 0 80"));
    std::string str = commander.GetElement(1);
    EXPECT_EQ(str[4], '2');
    EXPECT_EQ(str[6], '0');
}

TEST(CommmanderTests, ShouldMoveToTheClosestFieldToMine)
{
    Commander commander;
    commander.GiveOrders(std::stringstream("00060 00009 00000"), std::stringstream("400 P B 0 0 0 200 0 E B 1 4 2 200 A P W 2 0 0 80"));
    std::string str = commander.GetElement(1);
    EXPECT_EQ(str[4], '2');
    EXPECT_EQ(str[6], '0');
}