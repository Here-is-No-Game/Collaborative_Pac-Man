#pragma once

#include "game_types.h"
#include "config.h"
#include <vector>
#include <string>

class GameMap {
private:
    std::vector<std::vector<CellType>> grid;
    int width;
    int height;
    int totalDots;

public:
    // 构造函数
    GameMap();
    GameMap(int width, int height);

    // 初始化
    void initialize();
    void clear();

    // 地图访问
    CellType getCell(int x, int y) const;
    CellType getCell(const Position& pos) const;
    void setCell(int x, int y, CellType type);
    void setCell(const Position& pos, CellType type);

    // 地图查询
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isInBounds(int x, int y) const;
    bool isInBounds(const Position& pos) const;
    bool isWall(const Position& pos) const;
    bool isEmpty(const Position& pos) const;
    bool hasDot(const Position& pos) const;

    // 地图统计
    int countDots() const;
    int countEmptyCells() const;
    int getTotalDots() const { return totalDots; }
    void setTotalDots(int dots) { totalDots = dots; }

    // 地图加载与保存
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    bool loadFromString(const std::string& mapData);
    std::string saveToString() const;

    // 地图验证
    bool validate() const;

    // 地图复制
    GameMap clone() const;
};
