#pragma once

#include <cmath>

// Position 结构体
struct Position {
    int x;
    int y;

    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}

    bool operator==(const Position &other) const { return x == other.x && y == other.y; }

    bool operator!=(const Position &other) const { return !(*this == other); }

    // 计算曼哈顿距离
    int manhattanDistance(const Position &other) const { return std::abs(x - other.x) + std::abs(y - other.y); }
};

// Direction 枚举
enum class Direction { UP, DOWN, LEFT, RIGHT, STAY };

// 方向辅助函数
inline Direction getOppositeDirection(Direction dir) {
    switch (dir) {
    case Direction::UP:
        return Direction::DOWN;
    case Direction::DOWN:
        return Direction::UP;
    case Direction::LEFT:
        return Direction::RIGHT;
    case Direction::RIGHT:
        return Direction::LEFT;
    case Direction::STAY:
        return Direction::STAY;
    default:
        return Direction::STAY;
    }
}

inline const char *directionToString(Direction dir) {
    switch (dir) {
    case Direction::UP:
        return "UP";
    case Direction::DOWN:
        return "DOWN";
    case Direction::LEFT:
        return "LEFT";
    case Direction::RIGHT:
        return "RIGHT";
    case Direction::STAY:
        return "STAY";
    default:
        return "UNKNOWN";
    }
}

// CellType 枚举
enum class CellType { EMPTY, WALL, DOT };

inline const char *cellTypeToString(CellType type) {
    switch (type) {
    case CellType::EMPTY:
        return "EMPTY";
    case CellType::WALL:
        return "WALL";
    case CellType::DOT:
        return "DOT";
    default:
        return "UNKNOWN";
    }
}

// CharacterType 枚举
enum class CharacterType { PACMAN, MONSTER };

inline const char *characterTypeToString(CharacterType type) {
    switch (type) {
    case CharacterType::PACMAN:
        return "PACMAN";
    case CharacterType::MONSTER:
        return "MONSTER";
    default:
        return "UNKNOWN";
    }
}

// Character 结构体
struct Character {
    int id;
    Position position;
    CharacterType type;
    bool isAlive;

    Character() : id(0), position(0, 0), type(CharacterType::PACMAN), isAlive(true) {}

    Character(Position pos, CharacterType t) : id(0), position(pos), type(t), isAlive(true) {}
};

// Action 结构体
struct Action {
    Direction direction;

    Action() : direction(Direction::STAY) {}
    Action(Direction dir) : direction(dir) {}
};
