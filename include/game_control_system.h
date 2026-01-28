#pragma once

#include "game_state_manager.h"
#include <string>
#include <vector>

// 游戏控制系统 - 处理暂停、保存、加载、回放等功能
class GameControlSystem {
  public:
    enum class PlaybackStatus {
        PLAYING,   // 正在运行
        PAUSED,    // 暂停
        STEPPED_BACKWARD, // 刚刚后退一步
        STEPPED_FORWARD   // 刚刚前进一步
    };

  private:
    bool isPaused;
    PlaybackStatus playbackStatus;
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
    PlaybackStatus getPlaybackStatus() const { return playbackStatus; }

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

    // 回放控制
    bool canStepBackward() const;  // 是否可以后退一回合
    bool canStepForward() const;   // 是否可以前进一回合
    GameStateManager stepBackward(); // 后退一回合
    GameStateManager stepForward();  // 前进一回合
    GameStateManager restartFromBeginning(); // 从第一回合重新开始
    bool hasHistory() const;       // 是否有历史记录

    // 游戏重启
    void reset();
};
