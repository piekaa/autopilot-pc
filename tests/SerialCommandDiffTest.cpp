#include <gtest/gtest.h>
#include "../serial/SerialCommandDiff.h"

TEST(SerialCommandDiffTest, NoDifference) {
    std::string cmd1 = "H 100\nA 2000\nS 250";
    std::string cmd2 = "H 100\nA 2000\nS 250";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "");
}

TEST(SerialCommandDiffTest, SingleLineDifference) {
    std::string cmd1 = "H 100\nA 2000\nS 250";
    std::string cmd2 = "H 100\nA 3000\nS 250";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "A 3000");
}

TEST(SerialCommandDiffTest, MultipleDifferences) {
    std::string cmd1 = "H 100\nA 2000\nS 250";
    std::string cmd2 = "H 150\nA 3000\nS 250";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "H 150\nA 3000");
}

TEST(SerialCommandDiffTest, AllDifferent) {
    std::string cmd1 = "H 100\nA 2000\nS 250";
    std::string cmd2 = "H 150\nA 3000\nS 300";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "H 150\nA 3000\nS 300");
}

TEST(SerialCommandDiffTest, EmptyFirst) {
    std::string cmd1 = "";
    std::string cmd2 = "H 100\nA 2000";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "H 100\nA 2000");
}

TEST(SerialCommandDiffTest, EmptySecond) {
    std::string cmd1 = "H 100\nA 2000";
    std::string cmd2 = "";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "");
}

TEST(SerialCommandDiffTest, BothEmpty) {
    std::string cmd1 = "";
    std::string cmd2 = "";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "");
}

TEST(SerialCommandDiffTest, DifferentLengths) {
    std::string cmd1 = "H 100\nA 2000";
    std::string cmd2 = "H 100\nA 2000\nS 250\nVS -1000";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "S 250\nVS -1000");
}

TEST(SerialCommandDiffTest, WithAutopilotCommands) {
    std::string cmd1 = "AP ON\nAP_HEADING ON\nH 100";
    std::string cmd2 = "AP ON\nAP_HEADING OFF\nH 150";
    std::string result = SerialCommandDiff::diff(cmd1, cmd2);
    EXPECT_EQ(result, "AP_HEADING OFF\nH 150");
}
