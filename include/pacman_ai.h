#pragma once

  

#include "ai_interface.h"

#include <random>

#include <vector>

  

// 简单的吃豆人AI - 随机移动示例

// 这是给学生A的参考实现，学生可以在此基础上改进策略

class PacmanAI : public AIInterface {

   private:

      std::mt19937 randomEngine;

      Direction lastDirection = Direction::STAY; // 新增：记录上一步移动方向，避免回头

  

      // 辅助方法：获取所有有效的移动方向（不会撞墙）

      std::vector<Direction> getValidMoves(const VisibleArea &visibleArea) const;

  

      // 辅助方法：检查某个方向是否可以移动

      bool canMove(const VisibleArea &visibleArea, Direction dir) const;

  

      // 辅助方法：获取方向的偏移量

      void getDirectionOffset(Direction dir, int &dx, int &dy) const;

  

      // 新增辅助方法：过滤掉回头方向（如上次向右则排除向左）

      std::vector<Direction> filterBackwardMoves(const std::vector<Direction> &validMoves) const;

  

      // 辅助方法：找到最近食物的移动方向（贪心策略）

      Direction getNearestFoodDirection(const VisibleArea &visibleArea, int pacmanX, int pacmanY, const std::vector<Direction> &validMoves) const;

  

   public:

      PacmanAI();

      explicit PacmanAI(unsigned int seed);

  

      // 实现AI决策：随机选择一个有效的移动方向

    Action getAction(const VisibleArea &visibleArea) override;
  
};