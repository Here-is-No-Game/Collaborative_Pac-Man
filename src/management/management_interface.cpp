#include "../../include/management_interface.h"

bool ManagementInterface::isValidMove(const GameStateManager &gameState, const Position &pos) const {
    return gameState.getMap().isEmpty(pos) || gameState.getMap().hasDot(pos);
}

Position ManagementInterface::getNewPosition(const Position &currentPos, Direction dir) const {
    Position newPos = currentPos;

    switch (dir) {
    case Direction::UP:
        newPos.y--;
        break;
    case Direction::DOWN:
        newPos.y++;
        break;
    case Direction::LEFT:
        newPos.x--;
        break;
    case Direction::RIGHT:
        newPos.x++;
        break;
    case Direction::STAY:
        break;
    }

    return newPos;
}
