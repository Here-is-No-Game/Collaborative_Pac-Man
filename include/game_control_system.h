#pragma once

#include "game_state_manager.h"
#include <string>
#include <vector>

// 游戏控制系统 - 处理暂停、保存、加载、回放等功能
class GameControlSystem {
  private:
    bool isPaused;
    std::vector<GameStateManager> stateHistory; // 状态历史记录
    int currentHistoryIndex;
    int maxHistorySize;

  public:
    GameControlSystem();
    explicit GameControlSystem(int maxHistory);

    // 暂停/恢复控制
    void pause();
    void resume();
    bool getPaused() const { return isPaused; }
    void togglePause();

    // 状态保存/加载
    bool saveGame(const GameStateManager &gameState, const std::string &filename) const;
    bool loadGame(GameStateManager &gameState, const std::string &filename) const;

    // 回放功能
    void recordState(const GameStateManager &gameState);
    bool canUndo() const;
    bool canRedo() const;
    GameStateManager undo();
    GameStateManager redo();
    void clearHistory();

    // 游戏重启
    void reset();
};
