#include "../../include/game_state_manager.h"

GameStateManager::GameStateManager() : score(0), remainingDots(0), turnCount(1) {}

GameStateManager::GameStateManager(const GameMap &gameMap, const std::vector<Character> &chars)
    : map(gameMap), characters(chars), score(0), remainingDots(0), turnCount(1) {
    remainingDots = map.countDots();
}

void GameStateManager::initializeGame(const GameMap &gameMap, const std::vector<Character> &chars) {
    map = gameMap;
    characters = chars;
    score = 0;
    turnCount = 1;
    remainingDots = map.countDots();
}

const Character &GameStateManager::getCharacter(int index) const {
    if (index >= 0 && index < (int)characters.size()) {
        return characters[index];
    }
    static Character dummy;
    return dummy;
}

const Character &GameStateManager::getPacman() const {
    for (const auto &character : characters) {
        if (character.type == CharacterType::PACMAN) {
            return character;
        }
    }
    static Character dummy;
    return dummy;
}

std::vector<Character> GameStateManager::getMonsters() const {
    std::vector<Character> monsters;
    for (const auto &character : characters) {
        if (character.type == CharacterType::MONSTER) {
            monsters.push_back(character);
        }
    }
    return monsters;
}

void GameStateManager::updateCharacterPosition(int index, const Position &newPos) {
    if (index >= 0 && index < (int)characters.size()) {
        characters[index].position = newPos;
    }
}

void GameStateManager::consumeDot(const Position &pos) {
    if (map.hasDot(pos)) {
        map.setCell(pos, CellType::EMPTY);
        remainingDots--;
        incrementScore(GameConfig::POINTS_PER_DOT);
    }
}

void GameStateManager::setCharacterAlive(int index, bool alive) {
    if (index >= 0 && index < (int)characters.size()) {
        characters[index].isAlive = alive;
    }
}

void GameStateManager::incrementTurnCount() { turnCount++; }

void GameStateManager::incrementScore(int points) { score += points; }

CellType GameStateManager::getCellType(const Position &pos) const { return map.getCell(pos); }

bool GameStateManager::isValidPosition(const Position &pos) const { return map.isInBounds(pos); }

bool GameStateManager::isWall(const Position &pos) const { return map.isWall(pos); }

bool GameStateManager::hasDot(const Position &pos) const { return map.hasDot(pos); }

bool GameStateManager::isGameOver() const {
    // 检查吃豆人是否存活
    bool pacmanAlive = false;
    for (const auto &character : characters) {
        if (character.type == CharacterType::PACMAN && character.isAlive) {
            pacmanAlive = true;
            break;
        }
    }

    if (!pacmanAlive) {
        return true; // 吃豆人死亡，游戏结束
    }

    // 检查是否收集完所有豆子
    if (remainingDots == 0) {
        return true; // 胜利
    }

    return false;
}

GameState GameStateManager::getCurrentState() const {
    GameState state;
    state.map = map;
    state.characters = characters;
    state.score = score;
    state.remainingDots = remainingDots;
    state.turnCount = turnCount;
    return state;
}

void GameStateManager::restoreState(const GameState &state) {
    map = state.map;
    characters = state.characters;
    score = state.score;
    remainingDots = state.remainingDots;
    turnCount = state.turnCount;
}

void GameStateManager::setCharacters(const std::vector<Character> &chars) { characters = chars; }

void GameStateManager::setScore(int newScore) { score = newScore; }
