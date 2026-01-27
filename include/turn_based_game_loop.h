#pragma once

#include "ai_interface.h"
#include "game_control_system.h"
#include "game_state_manager.h"
#include "management_interface.h"
#include "visibility_system.h"
#include <memory>
#include <vector>

// 回合制游戏循环 - 协调AI决策、管理系统和渲染
class TurnBasedGameLoop {
  private:
    GameStateManager gameState;
    GameControlSystem controlSystem;
    VisibilitySystem visibilitySystem;

    std::vector<std::unique_ptr<AIInterface>> aiAgents;
    std::unique_ptr<ManagementInterface> managementSystem;

    bool isRunning;
    int currentTurn;

  public:
    TurnBasedGameLoop(const GameMap &map, const std::vector<Character> &characters);

    // 设置AI代理
    void setAIAgent(int characterIndex, std::unique_ptr<AIInterface> ai);

    // 设置管理系统
    void setManagementSystem(std::unique_ptr<ManagementInterface> management);

    // 游戏循环控制
    void start();
    void stop();
    bool getRunning() const { return isRunning; }

    // 执行一个回合
    bool executeTurn();

    // 获取游戏状态
    const GameStateManager &getGameState() const { return gameState; }
    GameStateManager &getGameState() { return gameState; }

    // 获取控制系统
    GameControlSystem &getControlSystem() { return controlSystem; }

    // 获取当前回合数
    int getCurrentTurn() const { return currentTurn; }

  private:
    // 收集所有AI的决策
    std::vector<Action> collectAIActions();
};
