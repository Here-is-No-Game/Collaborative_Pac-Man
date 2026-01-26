#pragma once

#include "game_types.h"
#include "game_map.h"
#include <vector>

// GameState 结构体 - 用于保存/加载
struct GameState {
    GameMap map;
    std::vector<Character> characters;
    int score;
    int remainingDots;
    int turnCount;

    GameState() : score(0), remainingDots(0), turnCount(1) {}
};

class GameStateManager {
private:
    GameMap map;
    std::vector<Character> characters;
    int score;
    int remainingDots;
    int turnCount;

public:
    // 构造函数
    GameStateManager();
    GameStateManager(const GameMap& gameMap, const std::vector<Character>& chars);

    // 初始化
    void initializeGame(const GameMap& gameMap, const std::vector<Character>& chars);

    // Getter 方法
    const GameMap& getMap() const { return map; }
    const std::vector<Character>& getCharacters() const { return characters; }
    const Character& getCharacter(int index) const;
    const Character& getPacman() const;
    std::vector<Character> getMonsters() const;
    int getScore() const { return score; }
    int getRemainingDots() const { return remainingDots; }
    int getTurnCount() const { return turnCount; }

    // 状态更新方法
    void updateCharacterPosition(int index, const Position& newPos);
    void consumeDot(const Position& pos);
    void setCharacterAlive(int index, bool alive);
    void incrementTurnCount();
    void incrementScore(int points);
    void setCharacters(const std::vector<Character>& chars);
    void setScore(int newScore);

    // 状态查询方法
    CellType getCellType(const Position& pos) const;
    bool isValidPosition(const Position& pos) const;
    bool isWall(const Position& pos) const;
    bool hasDot(const Position& pos) const;
    bool isGameOver() const;

    // 状态保存与恢复
    GameState getCurrentState() const;
    void restoreState(const GameState& state);
};
