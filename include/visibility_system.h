#pragma once

#include "game_map.h"
#include "game_types.h"
#include "visible_area.h"
#include <vector>

class VisibilitySystem {
  private:
    int visibilityRadius;

    // Bresenham's line 算法检查视线
    bool isVisible(const Position &from, const Position &to, const GameMap &map) const;
    int manhattanDistance(const Position &a, const Position &b) const;

    // 将CellType转换为VisibleArea::CellContent
    VisibleArea::CellContent getCellContent(const Position &pos, const GameMap &map,
                                            const std::vector<Character> &characters) const;

  public:
    VisibilitySystem(int radius);

    // 计算指定位置的可见区域
    VisibleArea calculateVisibleArea(const Position &center, const GameMap &map,
                                     const std::vector<Character> &characters) const;
};
