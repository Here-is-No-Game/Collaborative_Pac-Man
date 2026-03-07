#include "../../include/monster_ai.h"
#include <ctime>

MonsterAI::MonsterAI() { randomEngine.seed(static_cast<unsigned int>(std::time(nullptr))); }

MonsterAI::MonsterAI(unsigned int seed) { randomEngine.seed(seed); }

Action MonsterAI::getAction(const VisibleArea &visibleArea) {
    // 获取所有有效的移动方向
    std::vector<Direction> validMoves = getValidMoves(visibleArea);

    // 随机选择一个方向
    if (!validMoves.empty()) {
        int index = randomEngine() % validMoves.size();
        return Action{validMoves[index]};
    }

    // 如果没有有效移动，停留
    return Action{Direction::STAY};
}

// 辅助方法实现
std::vector<Direction> MonsterAI::getValidMoves(const VisibleArea &visibleArea) const {
    std::vector<Direction> validMoves;

    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : directions) {
        if (canMove(visibleArea, dir)) {
            validMoves.push_back(dir);
        }
    }

    // 如果没有有效移动，可以选择停留
    if (validMoves.empty()) {
        validMoves.push_back(Direction::STAY);
    }

    return validMoves;
}

bool MonsterAI::canMove(const VisibleArea &visibleArea, Direction dir) const {
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;

    int dx, dy;
    getDirectionOffset(dir, dx, dy);

    int newX = centerX + dx;
    int newY = centerY + dy;

    // 检查是否在边界内
    if (newX < 0 || newX >= visibleArea.getWidth() || newY < 0 || newY >= visibleArea.getHeight()) {
        return false;
    }

    // 检查是否是墙
    return visibleArea.getCell(newX, newY) == VisibleArea::CellContent::EMPTY;
}

void MonsterAI::getDirectionOffset(Direction dir, int &dx, int &dy) const {
    dx = 0;
    dy = 0;

    switch (dir) {
    case Direction::UP:
        dy = -1;
        break;
    case Direction::DOWN:
        dy = 1;
        break;
    case Direction::LEFT:
        dx = -1;
        break;
    case Direction::RIGHT:
        dx = 1;
        break;
    case Direction::STAY:
        break;
    }
}
