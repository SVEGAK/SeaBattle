#include "pch.h"
#include "SeaBattleStaticLib.cpp"
#include "GameField.h"

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


//Ship tests

TEST(ShipTest, Constructor_ValidHorizontalShip) {
    Position pos(1, 1);
    Ship ship(3, pos, Horizontal);
    EXPECT_EQ(ship.size(), 3);
    EXPECT_EQ(ship.row(), 1);
    EXPECT_EQ(ship.col(), 1);
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, Constructor_ValidVerticalShip) {
    Position pos(5, 5);
    Ship ship(4, pos, Vertical);
    EXPECT_EQ(ship.size(), 4);
    EXPECT_EQ(ship.row(), 5);
    EXPECT_EQ(ship.col(), 5);
    EXPECT_EQ(ship.direction(), Vertical);
}

TEST(ShipTest, Constructor_CharInterfaceHorizontal) {
    Ship ship(2, 'H', 3, 'C');
    EXPECT_EQ(ship.size(), 2);
    EXPECT_EQ(ship.row(), 3);
    EXPECT_EQ(ship.col(), 3);
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, Constructor_CharInterfaceVertical) {
    Ship ship(3, 'V', 1, 'A');
    EXPECT_EQ(ship.size(), 3);
    EXPECT_EQ(ship.row(), 1);
    EXPECT_EQ(ship.col(), 1);
    EXPECT_EQ(ship.direction(), Vertical);
}

TEST(ShipTest, Constructor_LowercaseDirection) {
    Ship ship(2, 'h', 5, 'E');
    EXPECT_EQ(ship.direction(), Horizontal);

    Ship ship2(2, 'v', 5, 'E');
    EXPECT_EQ(ship2.direction(), Vertical);
}

TEST(ShipTest, Constructor_DefaultDirectionIsHorizontal) {
    Position pos(2, 2);
    Ship ship(2, pos);
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, Constructor_InvalidSizeZero) {
    Position pos(1, 1);
    EXPECT_THROW(Ship(0, pos, Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_InvalidSizeNegative) {
    Position pos(1, 1);
    EXPECT_THROW(Ship(-1, pos, Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_InvalidSizeFive) {
    Position pos(1, 1);
    EXPECT_THROW(Ship(5, pos, Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_HorizontalExceedsRightBorder) {
    Position pos(1, 9);
    EXPECT_THROW(Ship(3, pos, Horizontal), std::logic_error);
}

TEST(ShipTest, Constructor_HorizontalFitsExactly) {
    Position pos(1, 8);
    Ship ship(3, pos, Horizontal);
    EXPECT_NO_THROW();
}

TEST(ShipTest, Constructor_VerticalExceedsBottomBorder) {
    Position pos(9, 1);
    EXPECT_THROW(Ship(3, pos, Vertical), std::logic_error);
}

TEST(ShipTest, Constructor_VerticalFitsExactly) {
    Position pos(8, 1);
    Ship ship(3, pos, Vertical);
    EXPECT_NO_THROW();
}

TEST(ShipTest, Constructor_InvalidDirectionChar) {
    EXPECT_THROW(Ship(2, 'X', 1, 'A'), std::logic_error);
    EXPECT_THROW(Ship(2, 'z', 1, 'A'), std::logic_error);
}

TEST(ShipTest, IsCollision_HorizontalFits) {
    Ship ship(1, Position(1, 1), Horizontal);
    Position pos(1, 1);
    EXPECT_FALSE(ship.is_collision(3, pos, Horizontal));
}

TEST(ShipTest, IsCollision_HorizontalOutOfBounds) {
    Ship ship(1, Position(1, 1), Horizontal);
    Position pos(1, 9);
    EXPECT_TRUE(ship.is_collision(3, pos, Horizontal));
}

TEST(ShipTest, IsCollision_VerticalFits) {
    Ship ship(1, Position(1, 1), Horizontal);
    Position pos(1, 1);
    EXPECT_FALSE(ship.is_collision(4, pos, Vertical));
}

TEST(ShipTest, IsCollision_VerticalOutOfBounds) {
    Ship ship(1, Position(1, 1), Horizontal);
    Position pos(9, 1);
    EXPECT_TRUE(ship.is_collision(3, pos, Vertical));
}

TEST(ShipTest, Rotate_HorizontalToVertical) {
    Position pos(5, 5);
    Ship ship(3, pos, Horizontal);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Vertical);
    EXPECT_EQ(ship.row(), 5);
    EXPECT_EQ(ship.col(), 5);
}

TEST(ShipTest, Rotate_VerticalToHorizontal) {
    Position pos(5, 5);
    Ship ship(3, pos, Vertical);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, Rotate_MultipleTimes) {
    Position pos(5, 5);
    Ship ship(2, pos, Horizontal);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Vertical);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Horizontal);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Vertical);
}


TEST(ShipTest, Getters_Size1To4) {
    for (int s = 1; s <= 4; ++s) {
        Position pos(1, 1);
        Ship ship(s, pos, Horizontal);
        EXPECT_EQ(ship.size(), s);
    }
}

TEST(ShipTest, Getters_RowCol) {
    Ship ship(2, 'H', 7, 'D');
    EXPECT_EQ(ship.row(), 7);
    EXPECT_EQ(ship.col(), 4);
}

TEST(ShipTest, Constructor_ShipAtCornerHorizontal) {
    Position pos(10, 10);
    Ship ship(1, pos, Horizontal);
    EXPECT_EQ(ship.row(), 10);
    EXPECT_EQ(ship.col(), 10);
}

TEST(ShipTest, Constructor_ShipAtCornerVertical) {
    Position pos(10, 10);
    Ship ship(1, pos, Vertical);
    EXPECT_EQ(ship.row(), 10);
    EXPECT_EQ(ship.col(), 10);
}

TEST(ShipTest, Rotate_Size1Ship) {
    Position pos(5, 5);
    Ship ship(1, pos, Horizontal);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Vertical);
    ship.rotate();
    EXPECT_EQ(ship.direction(), Horizontal);
}

TEST(ShipTest, Constructor_CharInterfaceRowBounds) {
    EXPECT_THROW(Ship(1, 'H', 0, 'A'), std::logic_error);
    EXPECT_THROW(Ship(1, 'H', 11, 'A'), std::logic_error);
}

TEST(ShipTest, Constructor_CharInterfaceColBounds) {
    EXPECT_THROW(Ship(1, 'H', 1, '@'), std::logic_error);
    EXPECT_THROW(Ship(1, 'H', 1, '['), std::logic_error);
}

//class GameField


TEST(GameFieldTest, DefaultConstructor) {
    GameField field;
    EXPECT_EQ(field.get(1, 'A'), ' ');
    EXPECT_EQ(field.get(10, 'J'), ' ');
}

TEST(GameFieldTest, PartialConstructor) {
    GameField field(5, 5);
    EXPECT_EQ(field.get(1, 'A'), ' ');
    EXPECT_EQ(field.get(5, 'E'), ' ');
}


TEST(GameFieldTest, FullConstructor) {
    char** data = new char* [3];
    for (int i = 0; i < 3; ++i) {
        data[i] = new char[3];
        for (int j = 0; j < 3; ++j) {
            data[i][j] = '*';
        }
    }

    GameField field(data, 3, 3);
    EXPECT_EQ(field.get(1, 'A'), '*');
    EXPECT_EQ(field.get(3, 'C'), '*');

    for (int i = 0; i < 3; ++i) {
        delete[] data[i];
    }
    delete[] data;
}


TEST(GameFieldTest, CopyConstructor) {
    GameField original(3, 3);
    original.set(2, 'B');

    GameField copy(original);
    EXPECT_EQ(copy.get(2, 'B'), '*');
    EXPECT_EQ(original.get(2, 'B'), '*');
}

TEST(GameFieldTest, SetMethod) {
    GameField field(3, 3);
    field.set(1, 'A');
    EXPECT_EQ(field.get(1, 'A'), '*');
}


TEST(GameFieldTest, GetMethod) {
    GameField field(3, 3);
    EXPECT_EQ(field.get(1, 'A'), ' ');
    field.set(2, 'B');
    EXPECT_EQ(field.get(2, 'B'), '*');
}


TEST(GameFieldTest, ToString) {
    GameField field(2, 2);
    field.set(1, 'A');

    std::string expected = "  |A B|\n  +---+\n1 |* | |\n2 | | |\n  +---+";
    EXPECT_EQ(to_string(field), expected);
}


TEST(GameFieldTest, InvalidDimensions) {
    EXPECT_THROW(GameField(0, 10), std::logic_error);
    EXPECT_THROW(GameField(10, 0), std::logic_error);
    EXPECT_THROW(GameField(26, 10), std::logic_error);
    EXPECT_THROW(GameField(10, 26), std::logic_error);
}


TEST(GameFieldTest, InvalidPosition) {
    GameField field(10, 10);

    EXPECT_THROW(field.get(0, 'A'), std::logic_error);
    EXPECT_THROW(field.get(11, 'A'), std::logic_error);
    EXPECT_THROW(field.get(1, 'K'), std::logic_error);
    EXPECT_THROW(field.set(0, 'A'), std::logic_error);
    EXPECT_THROW(field.set(1, 'K'), std::logic_error);
}

TEST(GameFieldTest, MultipleSets) {
    GameField field(3, 3);
    field.set(1, 'A');
    field.set(2, 'B');
    field.set(3, 'C');

    EXPECT_EQ(field.get(1, 'A'), '*');
    EXPECT_EQ(field.get(2, 'B'), '*');
    EXPECT_EQ(field.get(3, 'C'), '*');
    EXPECT_EQ(field.get(1, 'B'), ' ');
}