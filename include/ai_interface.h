#pragma once

#include "game_types.h"
#include "visible_area.h"
#include <vector>

// AI接口抽象类
// 学生需要继承此类并实现getAction方法
class AIInterface {
  public:
    virtual ~AIInterface() = default;

    // 核心方法：根据当前角色状态和可见区域返回行动决策
    // character: 当前角色信息（位置、类型等）
    // visibleArea: 当前可见区域（以角色为中心）
    // 返回：Action对象，包含移动方向
    virtual Action getAction(const Character &character, const VisibleArea &visibleArea) = 0;

  protected:
    // 辅助方法：获取从当前位置到目标位置的相对方向
    Direction getRelativeDirection(const VisibleArea &visibleArea, int targetX, int targetY) const;

    // 辅助方法：获取所有有效的移动方向（不会撞墙）
    std::vector<Direction> getValidMoves(const VisibleArea &visibleArea) const;

    // 辅助方法：检查某个方向是否可以移动
    bool canMove(const VisibleArea &visibleArea, Direction dir) const;

    // 辅助方法：获取方向的偏移量
    void getDirectionOffset(Direction dir, int &dx, int &dy) const;
};
