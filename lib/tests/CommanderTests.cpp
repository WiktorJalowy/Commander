#include "../Commander.hpp"
#include "gtest/gtest.h"

TEST(CommmanderTests, ShouldMoveToFieldNextToEnemyBase)
{
    Commander commander;
    const std::string map = "00000 00009 00000";
    const std::string status = "400 P B 0 0 0 200 0 E B 1 2 2 200 A P K 2 0 0 80";

    commander.GiveOrders(std::stringstream(map), std::stringstream(status));
    std::string orderToCheck = commander.GetElement(1);
    EXPECT_EQ(orderToCheck[4], '1');
    EXPECT_EQ(orderToCheck[6], '2');
}

TEST(CommmanderTests, ShouldMoveToFieldAboveEnemyBase)
{
    Commander commander;
    const std::string map = "00000 00009 09000";
    const std::string status = "400 P B 0 0 0 200 0 E B 1 2 2 200 A P K 2 0 0 80";

    commander.GiveOrders(std::stringstream(map), std::stringstream(status));
    std::string orderToCheck = commander.GetElement(1);
    EXPECT_EQ(orderToCheck[4], '2');
    EXPECT_EQ(orderToCheck[6], '1');
}

TEST(CommmanderTests, ShouldMoveToFieldWithoutStandingOnOccupiedField)
{
    Commander commander;
    const std::string map = "00000 00009 00000";
    const std::string status = "400 P B 0 0 0 200 0 E B 1 4 2 200 A P K 2 0 0 80";

    commander.GiveOrders(std::stringstream(map), std::stringstream(status));
    std::string orderToCheck = commander.GetElement(1);
    EXPECT_EQ(orderToCheck[4], '4');
    EXPECT_EQ(orderToCheck[6], '0');
}

TEST(CommmanderTests, ShouldMoveToMine)
{
    Commander commander;
    const std::string map = "00600 00009 00000";
    const std::string status = "400 P B 0 0 0 200 0 E B 1 4 2 200 A P W 2 0 0 80";

    commander.GiveOrders(std::stringstream(map), std::stringstream(status));
    std::string orderToCheck = commander.GetElement(1);
    EXPECT_EQ(orderToCheck[4], '2');
    EXPECT_EQ(orderToCheck[6], '0');
}

TEST(CommmanderTests, ShouldMoveToTheClosestFieldToMine)
{
    Commander commander;
    const std::string map = "00960 00009 00000";
    const std::string status = "400 P B 0 0 0 200 0 E B 1 4 2 200 A P W 2 0 0 80";
    commander.GiveOrders(std::stringstream(map), std::stringstream(status));
    std::string orderToCheck = commander.GetElement(1);
    EXPECT_EQ(orderToCheck[4], '1');
    EXPECT_EQ(orderToCheck[6], '1');
}