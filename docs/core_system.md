# 游戏核心系统设计文档

## 概述

本文档描述了协作式吃豆人游戏项目的核心系统设计。核心系统负责管理游戏状态、实现回合制游戏循环，并提供接口供学生实现AI决策和管理系统。

## 核心组件

1. 游戏状态管理器（GameStateManager）
2. 回合制游戏循环（TurnBasedGameLoop）
3. AI接口（AIInterface）
4. 管理系统接口（ManagementInterface）
5. 可见性系统（VisibilitySystem）
6. 随机地图生成器（RandomMapGenerator）
7. 游戏控制系统（GameControlSystem）

## 详细设计

### 1. 游戏状态管理器（GameStateManager）

[保持原有内容]

### 2. 回合制游戏循环（TurnBasedGameLoop）

[保持原有内容]

### 3. AI接口（AIInterface）

[保持原有内容]

### 4. 管理系统接口（ManagementInterface）

[保持原有内容]

### 5. 可见性系统（VisibilitySystem）

实现基于可配置半径的可见性计算，考虑墙壁阻挡。

```cpp
class VisibilitySystem {
private:
    const GameStateManager& gameState;
    int visibilityRadius;

public:
    VisibilitySystem(const GameStateManager& state, int radius);
    VisibleArea getVisibleArea(const Position& center) const;

private:
    bool isVisible(const Position& from, const Position& to) const;
    bool isInBounds(const Position& pos) const;
};
```

#### 视野生成算法实现

视野生成算法使用 Bresenham's line 算法来检查从中心点到目标点的直线是否被墙壁阻挡。这确保了视野的准确性，同时考虑了游戏中的障碍物。

```cpp
VisibleArea VisibilitySystem::getVisibleArea(const Position& center) const {
    // ... [保持原有实现]
}

bool VisibilitySystem::isVisible(const Position& from, const Position& to) const {
    // ... [保持原有实现]
}

bool VisibilitySystem::isInBounds(const Position& pos) const {
    // ... [保持原有实现]
}
```

### 6. 随机地图生成器（RandomMapGenerator）

负责创建随机的游戏地图，包括随机的道路布局、角色初始位置和豆子分布。

```cpp
class RandomMapGenerator {
public:
    RandomMapGenerator(int width, int height, float dotRatio);
    GameMap generateMap();
    std::vector<Position> generateCharacterPositions(int characterCount);

private:
    int width;
    int height;
    float dotRatio;

    void generateMaze();
    void placeDots();
    bool isValidCharacterPosition(const Position& pos);
};
```

#### 地图生成算法实现

地图生成算法使用递归分割方法来创建迷宫结构，然后随机放置豆子。这种方法可以生成复杂且随机的地图布局，增加游戏的可玩性和挑战性。

```cpp
GameMap RandomMapGenerator::generateMap() {
    // ... [保持原有实现]
}

void RandomMapGenerator::placeDots() {
    // ... [保持原有实现]
}

std::vector<Position> RandomMapGenerator::generateCharacterPositions(int characterCount) {
    // ... [保持原有实现]
}

bool RandomMapGenerator::isValidCharacterPosition(const Position& pos) {
    // ... [保持原有实现]
}
```

### 7. 游戏控制系统（GameControlSystem）

负责处理游戏流程的保存、暂停、回放和重启等功能。

```cpp
class GameControlSystem {
public:
    GameControlSystem(GameStateManager& gameState, RandomMapGenerator& mapGenerator);

    void pauseGame();
    void resumeGame();
    void saveGameState();
    void loadGameState(int turnNumber);
    void restartCurrentGame();
    void startNewGame();
    void nextTurn();
    void previousTurn();

private:
    GameStateManager& gameState;
    RandomMapGenerator& mapGenerator;
    std::vector<GameState> gameHistory;
    int currentTurn;
    bool isPaused;

    void resetRandomSeed();
};
```

#### 游戏回合历史记录保存和跳转算法实现

游戏控制系统实现了回合历史记录的保存和加载，允许玩家在游戏过程中前进、后退和重新开始。这个功能对于调试和分析AI行为特别有用。

```cpp
void GameControlSystem::saveGameState() {
    // ... [保持原有实现]
}

void GameControlSystem::loadGameState(int turnNumber) {
    // ... [保持原有实现]
}

void GameControlSystem::nextTurn() {
    // ... [保持原有实现]
}

void GameControlSystem::previousTurn() {
    // ... [保持原有实现]
}

void GameControlSystem::restartCurrentGame() {
    // ... [保持原有实现]
}

void GameControlSystem::startNewGame() {
    // ... [保持原有实现]
}

void GameControlSystem::resetRandomSeed() {
    // ... [保持原有实现]
}

void GameControlSystem::pauseGame() {
    // ... [保持原有实现]
}

void GameControlSystem::resumeGame() {
    // ... [保持原有实现]
}
```

## 系统交互

[保持原有内容]

## 配置系统

[保持原有内容]

## 注意事项

[保持原有内容]

## 下一步

[保持原有内容]

请审阅以上设计，并提供任何反馈或建议。我们可以根据需要进一步细化或调整任何部分。