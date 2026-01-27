#pragma once

#include "game_types.h"
#include "visible_area.h"

// AI接口抽象类
// 学生需要继承此类并实现getAction方法
// 注意：此类只定义接口，不提供辅助函数
// 每个AI实现类应该自己编写所需的辅助函数
class AIInterface {
  public:
    virtual ~AIInterface() = default;

    // 核心方法：根据当前角色状态和可见区域返回行动决策
    // character: 当前角色信息（位置、类型等）
    // visibleArea: 当前可见区域（以角色为中心）
    // 返回：Action对象，包含移动方向
    virtual Action getAction(const Character &character, const VisibleArea &visibleArea) = 0;
};
