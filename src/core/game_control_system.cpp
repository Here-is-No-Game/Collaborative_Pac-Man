#include "../../include/game_control_system.h"
#include <fstream>

GameControlSystem::GameControlSystem()
    : isPaused(false), playbackStatus(PlaybackStatus::PLAYING), currentHistoryIndex(-1), maxHistorySize(100) {}

GameControlSystem::GameControlSystem(int maxHistory)
    : isPaused(false), playbackStatus(PlaybackStatus::PLAYING), currentHistoryIndex(-1), maxHistorySize(maxHistory) {}

void GameControlSystem::pause() {
    isPaused = true;
    playbackStatus = PlaybackStatus::PAUSED;
}

void GameControlSystem::resume() {
    isPaused = false;
    playbackStatus = PlaybackStatus::PLAYING;
}

void GameControlSystem::togglePause() { isPaused = !isPaused; }

bool GameControlSystem::saveGame(const GameStateManager &gameState, const std::string &filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // 保存游戏状态
    std::string mapData = gameState.getMap().saveToString();

    // 写入地图数据长度
    size_t mapSize = mapData.size();
    file.write(reinterpret_cast<const char *>(&mapSize), sizeof(mapSize));

    // 写入地图数据
    file.write(mapData.c_str(), mapSize);

    // 保存其他状态数据
    int pacmanScore = gameState.getPacmanScore();
    int monsterScore = gameState.getMonsterScore();
    int remainingDots = gameState.getRemainingDots();

    file.write(reinterpret_cast<const char *>(&pacmanScore), sizeof(pacmanScore));
    file.write(reinterpret_cast<const char *>(&monsterScore), sizeof(monsterScore));
    file.write(reinterpret_cast<const char *>(&remainingDots), sizeof(remainingDots));

    // 保存角色数据
    const auto &characters = gameState.getCharacters();
    size_t charCount = characters.size();
    file.write(reinterpret_cast<const char *>(&charCount), sizeof(charCount));

    for (const auto &character : characters) {
        file.write(reinterpret_cast<const char *>(&character), sizeof(Character));
    }

    file.close();
    return true;
}

bool GameControlSystem::loadGame(GameStateManager &gameState, const std::string &filename) const {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // 读取地图数据长度
    size_t mapSize;
    file.read(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    // 读取地图数据
    std::string mapData(mapSize, '\0');
    file.read(&mapData[0], mapSize);

    // 加载地图
    GameMap map(0, 0);
    if (!map.loadFromString(mapData)) {
        file.close();
        return false;
    }

    // 读取其他状态数据
    int pacmanScore, monsterScore, remainingDots;
    file.read(reinterpret_cast<char *>(&pacmanScore), sizeof(pacmanScore));
    file.read(reinterpret_cast<char *>(&monsterScore), sizeof(monsterScore));
    file.read(reinterpret_cast<char *>(&remainingDots), sizeof(remainingDots));

    // 读取角色数据
    size_t charCount;
    file.read(reinterpret_cast<char *>(&charCount), sizeof(charCount));

    std::vector<Character> characters(charCount);
    for (size_t i = 0; i < charCount; ++i) {
        file.read(reinterpret_cast<char *>(&characters[i]), sizeof(Character));
    }

    file.close();

    // 更新游戏状态
    gameState = GameStateManager(map, characters);
    gameState.setPacmanScore(pacmanScore);
    gameState.setMonsterScore(monsterScore);

    return true;
}

void GameControlSystem::recordState(const GameStateManager &gameState) {
    // 如果当前不在历史末尾，删除后面的历史
    if (currentHistoryIndex < static_cast<int>(stateHistory.size()) - 1) {
        stateHistory.erase(stateHistory.begin() + currentHistoryIndex + 1, stateHistory.end());
    }

    // 添加新状态
    stateHistory.push_back(gameState);
    currentHistoryIndex++;

    // 限制历史大小
    if (static_cast<int>(stateHistory.size()) > maxHistorySize) {
        stateHistory.erase(stateHistory.begin());
        currentHistoryIndex--;
    }
}

bool GameControlSystem::canUndo() const { return currentHistoryIndex > 0; }

bool GameControlSystem::canRedo() const { return currentHistoryIndex < static_cast<int>(stateHistory.size()) - 1; }

GameStateManager GameControlSystem::undo() {
    if (canUndo()) {
        currentHistoryIndex--;
        return stateHistory[currentHistoryIndex];
    }
    return stateHistory[currentHistoryIndex];
}

GameStateManager GameControlSystem::redo() {
    if (canRedo()) {
        currentHistoryIndex++;
        return stateHistory[currentHistoryIndex];
    }
    return stateHistory[currentHistoryIndex];
}

void GameControlSystem::clearHistory() {
    stateHistory.clear();
    currentHistoryIndex = -1;
}

void GameControlSystem::reset() {
    isPaused = false;
    clearHistory();
}

bool GameControlSystem::canStepBackward() const { return currentHistoryIndex > 0; }

bool GameControlSystem::canStepForward() const {
    return currentHistoryIndex < static_cast<int>(stateHistory.size()) - 1;
}

GameStateManager GameControlSystem::stepBackward() {
    if (canStepBackward()) {
        currentHistoryIndex--;
        isPaused = true; // 后退后保持暂停
        playbackStatus = PlaybackStatus::STEPPED_BACKWARD;
        return stateHistory[currentHistoryIndex];
    }
    return stateHistory[currentHistoryIndex];
}

GameStateManager GameControlSystem::stepForward() {
    if (canStepForward()) {
        currentHistoryIndex++;
        isPaused = true; // 前进后保持暂停
        playbackStatus = PlaybackStatus::STEPPED_FORWARD;
        return stateHistory[currentHistoryIndex];
    }
    return stateHistory[currentHistoryIndex];
}

GameStateManager GameControlSystem::restartFromBeginning() {
    if (!stateHistory.empty()) {
        currentHistoryIndex = 0;
        playbackStatus = PlaybackStatus::PLAYING;
        return stateHistory[0];
    }
    return stateHistory[currentHistoryIndex];
}

bool GameControlSystem::hasHistory() const { return !stateHistory.empty(); }
