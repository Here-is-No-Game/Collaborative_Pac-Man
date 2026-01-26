#pragma once

#include "management_interface.h"

// 简单的管理系统实现 - 基础碰撞检测和豆子判定
// 这是给学生C的参考实现，学生可以在此基础上改进
class ManagementSystem : public ManagementInterface {
public:
    ManagementSystem() = default;

    // 实现行动处理：
    // 1. 验证移动（撞墙判定）
    // 2. 更新角色位置
    // 3. 处理豆子消耗
    // 4. 检查碰撞（敌人判定）
    // 5. 检查胜利条件
    bool processActions(const std::vector<Action>& actions, GameStateManager& gameState) override;
};
