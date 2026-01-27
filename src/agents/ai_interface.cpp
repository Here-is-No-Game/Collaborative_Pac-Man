#include "../../include/ai_interface.h"

Direction AIInterface::getRelativeDirection(const VisibleArea &visibleArea, int targetX, int targetY) const {
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;

    int dx = targetX - centerX;
    int dy = targetY - centerY;

    // 优先选择距离更远的方向
    if (std::abs(dx) > std::abs(dy)) {
        return dx > 0 ? Direction::RIGHT : Direction::LEFT;
    } else if (std::abs(dy) > std::abs(dx)) {
        return dy > 0 ? Direction::DOWN : Direction::UP;
    } else if (dx != 0) {
        return dx > 0 ? Direction::RIGHT : Direction::LEFT;
    } else if (dy != 0) {
        return dy > 0 ? Direction::DOWN : Direction::UP;
    }

    return Direction::STAY;
}

std::vector<Direction> AIInterface::getValidMoves(const VisibleArea &visibleArea) const {
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

bool AIInterface::canMove(const VisibleArea &visibleArea, Direction dir) const {
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
    return visibleArea.getCell(newX, newY) != VisibleArea::CellContent::WALL;
}

void AIInterface::getDirectionOffset(Direction dir, int &dx, int &dy) const {
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
