#pragma once

#include "management_interface.h"

// 简单的管理系统实现 - 基础示例
// 这是给学生C的参考实现，学生需要在此基础上扩展功能
class ManagementSystem : public ManagementInterface {
  public:
    ManagementSystem() = default;

    // 实现行动处理：
    // 1. 处理吃豆人移动（检查越界）
    // 2. 处理怪物移动（简单计分示例）
    // 学生需要添加：豆子判定、碰撞检测、胜利条件等
    bool processActions(const std::vector<Action> &actions, GameStateManager &gameState) override;
};
