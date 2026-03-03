#pragma once

#include "game_map.h"
#include "game_types.h"
#include <random>
#include <vector>

class RandomMapGenerator {
  private:
    int width;
    int height;
    float dotRatio;
    std::mt19937 randomEngine;
    GameMap *currentMap;

    // 深度优先搜索生成迷宫
    void generateMaze(int startX, int startY);

    // 创建小型开放区域
    void createSmallOpenArea(int centerX, int centerY);

    // 辅助方法
    bool isInBounds(int x, int y) const;

  public:
    RandomMapGenerator(int w, int h, float dotRatio);

    void setSeed(unsigned int seed);
    GameMap generateMap();
    std::vector<Position> generateCharacterPositions(int characterCount);

  private:
    void placeDots();
    bool isValidCharacterPosition(const Position &pos, const std::vector<Position> &existingPositions) const;
};
