#pragma once

#include "game_types.h"
#include "game_state_manager.h"
#include <vector>

// 管理系统接口抽象类
// 学生C需要继承此类并实现processActions方法
class ManagementInterface {
public:
    virtual ~ManagementInterface() = default;

    // 核心方法：处理所有角色的行动并更新游戏状态
    // actions: 所有角色的行动决策
    // gameState: 当前游戏状态管理器
    // 返回：true表示游戏继续，false表示游戏结束
    virtual bool processActions(const std::vector<Action>& actions, GameStateManager& gameState) = 0;

protected:
    // 辅助方法：检查移动是否有效（不会撞墙）
    bool isValidMove(const GameStateManager& gameState, const Position& pos) const;

    // 辅助方法：根据方向获取新位置
    Position getNewPosition(const Position& currentPos, Direction dir) const;

    // 辅助方法：更新角色位置
    void updateCharacterPosition(GameStateManager& gameState, int characterIndex, const Position& newPos);

    // 辅助方法：消耗豆子
    void consumeDot(GameStateManager& gameState, const Position& pos);

    // 辅助方法：检查吃豆人是否与怪物碰撞
    bool checkPacmanMonsterCollision(const GameStateManager& gameState) const;

    // 辅助方法：检查是否获胜（所有豆子被吃完）
    bool checkWinCondition(const GameStateManager& gameState) const;
};
