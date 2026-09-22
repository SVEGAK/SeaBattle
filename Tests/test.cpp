#include "pch.h"
#include "SeaBattleStaticLib.cpp"


//class Position
TEST(PositionTest, DefaultConstructor) {
    Position pos;
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, CopyConstructor) {
    Position pos1(5, 7);
    Position pos2(pos1);
    EXPECT_EQ(pos2.row(), 5);
    EXPECT_EQ(pos2.col(), 7);
}

TEST(PositionTest, ValidIntConstructor) {
    Position pos(3, 8);
    EXPECT_EQ(pos.row(), 3);
    EXPECT_EQ(pos.col(), 8);
}

TEST(PositionTest, IntConstructorMinBounds) {
    Position pos(1, 1);
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, IntConstructorMaxBounds) {
    Position pos(10, 10);
    EXPECT_EQ(pos.row(), 10);
    EXPECT_EQ(pos.col(), 10);
}

TEST(PositionTest, IntConstructorRowTooLow) {
    EXPECT_THROW(Position(0, 5), std::logic_error);
}

TEST(PositionTest, IntConstructorRowTooHigh) {
    EXPECT_THROW(Position(11, 5), std::logic_error);
}

TEST(PositionTest, IntConstructorColTooLow) {
    EXPECT_THROW(Position(5, 0), std::logic_error);
}

TEST(PositionTest, IntConstructorColTooHigh) {
    EXPECT_THROW(Position(5, 11), std::logic_error);
}

TEST(PositionTest, ValidStringConstructor) {
    Position pos("(3, 7)");
    EXPECT_EQ(pos.row(), 3);
    EXPECT_EQ(pos.col(), 7);
}

TEST(PositionTest, StringConstructorMinBounds) {
    Position pos("(1, 1)");
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, StringConstructorMaxBounds) {
    Position pos("(10, 10)");
    EXPECT_EQ(pos.row(), 10);
    EXPECT_EQ(pos.col(), 10);
}

TEST(PositionTest, StringConstructorMissingOpenParen) {
    EXPECT_THROW(Position("3, 7)"), std::logic_error);
}

TEST(PositionTest, StringConstructorMissingComma) {
    EXPECT_THROW(Position("(3 7)"), std::logic_error);
}

TEST(PositionTest, StringConstructorMissingCloseParen) {
    EXPECT_THROW(Position("(3, 7"), std::logic_error);
}

TEST(PositionTest, StringConstructorInvalidFormat) {
    EXPECT_THROW(Position("3, 7"), std::logic_error);
}

TEST(PositionTest, StringConstructorRowOutOfBounds) {
    EXPECT_THROW(Position("(11, 5)"), std::logic_error);
}

TEST(PositionTest, StringConstructorColOutOfBounds) {
    EXPECT_THROW(Position("(5, 11)"), std::logic_error);
}

TEST(PositionTest, SetValidRow) {
    Position pos(5, 5);
    pos.row(3);
    EXPECT_EQ(pos.row(), 3);
}

TEST(PositionTest, SetRowTooLow) {
    Position pos(5, 5);
    EXPECT_THROW(pos.row(0), std::logic_error);
    EXPECT_EQ(pos.row(), 5);
}

TEST(PositionTest, SetRowTooHigh) {
    Position pos(5, 5);
    EXPECT_THROW(pos.row(11), std::logic_error);
    EXPECT_EQ(pos.row(), 5);
}

TEST(PositionTest, SetValidCol) {
    Position pos(5, 5);
    pos.col(8);
    EXPECT_EQ(pos.col(), 8);
}

TEST(PositionTest, SetColTooLow) {
    Position pos(5, 5);
    EXPECT_THROW(pos.col(0), std::logic_error);
    EXPECT_EQ(pos.col(), 5);
}

TEST(PositionTest, SetColTooHigh) {
    Position pos(5, 5);
    EXPECT_THROW(pos.col(11), std::logic_error);
    EXPECT_EQ(pos.col(), 5);
}

TEST(PositionTest, ToStringBasic) {
    Position pos(3, 7);
    EXPECT_EQ(to_string(pos), "(3, 7)");
}

TEST(PositionTest, ToStringMinBounds) {
    Position pos(1, 1);
    EXPECT_EQ(to_string(pos), "(1, 1)");
}

TEST(PositionTest, ToStringMaxBounds) {
    Position pos(10, 10);
    EXPECT_EQ(to_string(pos), "(10, 10)");
}

TEST(PositionTest, ParseValidString) {
    Position pos = parse("(4, 6)");
    EXPECT_EQ(pos.row(), 4);
    EXPECT_EQ(pos.col(), 6);
}

TEST(PositionTest, ParseMinBounds) {
    Position pos = parse("(1, 1)");
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, ParseMaxBounds) {
    Position pos = parse("(10, 10)");
    EXPECT_EQ(pos.row(), 10);
    EXPECT_EQ(pos.col(), 10);
}

TEST(PositionTest, ParseMissingOpenParen) {
    EXPECT_THROW(parse("4, 6)"), std::logic_error);
}

TEST(PositionTest, ParseMissingComma) {
    EXPECT_THROW(parse("(4 6)"), std::logic_error);
}

TEST(PositionTest, ParseMissingCloseParen) {
    EXPECT_THROW(parse("(4, 6"), std::logic_error);
}

TEST(PositionTest, ParseInvalidFormat) {
    EXPECT_THROW(parse("4, 6"), std::logic_error);
}

TEST(PositionTest, ParseRowOutOfBounds) {
    EXPECT_THROW(parse("(11, 5)"), std::logic_error);
}

TEST(PositionTest, ParseColOutOfBounds) {
    EXPECT_THROW(parse("(5, 11)"), std::logic_error);
}
