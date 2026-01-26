#include "../../include/management_interface.h"

bool ManagementInterface::isValidMove(const GameStateManager& gameState, const Position& pos) const {
    return gameState.getMap().isEmpty(pos) ||
           gameState.getMap().hasDot(pos);
}

Position ManagementInterface::getNewPosition(const Position& currentPos, Direction dir) const {
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

void ManagementInterface::updateCharacterPosition(GameStateManager& gameState, int characterIndex, const Position& newPos) {
    auto characters = gameState.getCharacters();
    if (characterIndex >= 0 && characterIndex < static_cast<int>(characters.size())) {
        characters[characterIndex].position = newPos;
        gameState.setCharacters(characters);
    }
}

void ManagementInterface::consumeDot(GameStateManager& gameState, const Position& pos) {
    if (gameState.getMap().hasDot(pos)) {
        gameState.consumeDot(pos);
    }
}

bool ManagementInterface::checkPacmanMonsterCollision(const GameStateManager& gameState) const {
    const auto& characters = gameState.getCharacters();

    // 找到吃豆人
    Position pacmanPos;
    bool foundPacman = false;
    for (const auto& character : characters) {
        if (character.type == CharacterType::PACMAN) {
            pacmanPos = character.position;
            foundPacman = true;
            break;
        }
    }

    if (!foundPacman) {
        return false;
    }

    // 检查是否与怪物在同一位置
    for (const auto& character : characters) {
        if (character.type == CharacterType::MONSTER) {
            if (character.position.x == pacmanPos.x && character.position.y == pacmanPos.y) {
                return true;
            }
        }
    }

    return false;
}

bool ManagementInterface::checkWinCondition(const GameStateManager& gameState) const {
    return gameState.getRemainingDots() == 0;
}
