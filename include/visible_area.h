#pragma once

#include "game_types.h"
#include <vector>

// 前向声明
class VisibilitySystem;

// VisibleArea 类
class VisibleArea {
  public:                   //空单元格，墙壁，豆子，吃豆人，怪物，未知（视野外），超出边界（访问无效位置时返回）
    enum class CellContent { EMPTY, WALL, DOT, PACMAN, MONSTER, UNKNOWN, OVERBOUND };

  private:
    std::vector<std::vector<CellContent>> grid;//创建视野，动态的二维数组
    int width;                                 //横向的视野
    int height;                                //纵向的
    Position centerPosition;                   //就是角色的位置

    // 只允许 VisibilitySystem 修改视野内容
    friend class VisibilitySystem;
    void setCell(int x, int y, CellContent content);

  public:
    VisibleArea(int w, int h);

    // AI 只能读取视野，不能修改
    CellContent getCell(int x, int y) const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
