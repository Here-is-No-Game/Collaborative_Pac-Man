#pragma once

#include "ai_interface.h"
#include <random>
#include <vector>

// 简单的怪物AI - 随机移动示例
// 这是给学生B的参考实现，学生可以在此基础上改进策略
class MonsterAI : public AIInterface {
  private:
    std::mt19937 randomEngine;

    // 辅助方法：获取所有有效的移动方向（不会撞墙）
    std::vector<Direction> getValidMoves(const VisibleArea &visibleArea) const;

    // 辅助方法：检查某个方向是否可以移动
    bool canMove(const VisibleArea &visibleArea, Direction dir) const;

    // 辅助方法：获取方向的偏移量
    void getDirectionOffset(Direction dir, int &dx, int &dy) const;

  public:
    MonsterAI();
    explicit MonsterAI(unsigned int seed);

    // 实现AI决策：随机选择一个有效的移动方向
    Action getAction(const Character &character, const VisibleArea &visibleArea) override;
};
