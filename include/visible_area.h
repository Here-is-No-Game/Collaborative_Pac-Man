#pragma once

#include "game_types.h"
#include <vector>

// VisibleArea 类
class VisibleArea {
  public:
    enum class CellContent { EMPTY, WALL, DOT, PACMAN, MONSTER, UNKNOWN, OVERBOUND };

  private:
    std::vector<std::vector<CellContent>> grid;
    int width;
    int height;
    Position centerPosition;

  public:
    VisibleArea(int w, int h);

    CellContent getCell(int x, int y) const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setCell(int x, int y, CellContent content);
};
