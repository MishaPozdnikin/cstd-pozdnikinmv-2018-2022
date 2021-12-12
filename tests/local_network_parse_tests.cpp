#include "local_network_parse.h"

#include <gtest/gtest.h>

TEST(NetworkParsingTest, getValueTestTrue)
{
    char input[10] = "1;2;3;4;";

    EXPECT_EQ(getValue(input, ";", 2), 2);
}

TEST(NetworkParsingTest, getValueTestFalse)
{
    char input[10] = "1;2;3;4;";

    EXPECT_EQ(getValue(input, ";", 10), -1);
}

TEST(NetworkParsingTest, getDelimsNumTestTrue)
{
    char input[10] = "1;2;3;4;";

    EXPECT_EQ(getDelimsNum(input, ";"), 4);
}

TEST(NetworkParsingTest, getDelimsNumTestFalse)
{
    char input[30] = "Text without delimeter";

    EXPECT_EQ(getDelimsNum(input, ";"), 1);
}

