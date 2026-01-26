# Agent 实现指南

本文档提供了协作式吃豆人项目中实现 Agent（包括吃豆人和怪物）的详细信息，重点关注视野参数的处理和动作返回值的类型及用法。

## AIInterface 概述

`AIInterface` 是一个抽象类，定义了所有 Agent 实现的接口。吃豆人和怪物的 Agent 都将实现这个接口。

```cpp
class AIInterface {
public:
    virtual Action getAction(const Character& character, const VisibleArea& visibleArea) = 0;
};
```

## 视野参数

### 类型
```cpp
class VisibleArea {
public:
    enum class CellContent {
        EMPTY,
        WALL,
        DOT,
        POWER_PELLET,
        PACMAN,
        MONSTER
    };

    CellContent getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
};
```

### 描述
- `VisibleArea` 类表示 Agent 的可见区域。
- 可见区域是一个以 Agent 为中心的正方形区域。
- 可见区域的大小由游戏配置中的可视半径决定，总是一个奇数。
- Agent 始终位于可见区域的中心。

### 如何获取视野
在 `AIInterface::getAction` 方法中，`VisibleArea` 对象会作为参数传递给你的 Agent。你不需要手动请求或计算视野，游戏系统会自动为每个 Agent 在每个回合提供更新后的视野信息。

### 可用的方法
`VisibleArea` 类提供以下方法来访问视野信息：

1. `CellContent getCell(int x, int y) const`
   - 获取指定坐标的单元格内容。
   - 参数 `x` 和 `y` 是相对于可见区域左上角的坐标。
   - 返回该位置的 `CellContent`。

2. `int getWidth() const`
   - 获取可见区域的宽度。
   - 返回值总是奇数，等于 (2 * 可视半径 + 1)。

3. `int getHeight() const`
   - 获取可见区域的高度。
   - 返回值总是奇数，等于 (2 * 可视半径 + 1)。

### 使用示例
```cpp
// 检查是否可以看到吃豆人
bool canSeePacman(const VisibleArea& visibleArea) {
    int width = visibleArea.getWidth();
    int height = visibleArea.getHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (visibleArea.getCell(x, y) == VisibleArea::CellContent::PACMAN) {
                return true;
            }
        }
    }
    return false;
}

// 计算相对于角色的方向
Direction getRelativeDirection(const VisibleArea& visibleArea, int targetX, int targetY) {
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;
    int dx = targetX - centerX;
    int dy = targetY - centerY;

    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    } else {
        return (dy > 0) ? Direction::DOWN : Direction::UP;
    }
}
```

## 动作返回值

### 类型
```cpp
struct Action {
    Direction direction;
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STAY
};
```

### 描述
- `Action` 结构体表示 Agent 为下一步移动做出的决定。
- 它包含一个类型为 `Direction` 的 `direction` 字段。
- `Direction` 是一个枚举类，表示可能的移动方向或原地不动的选项。

### 用法
1. 你的 Agent 应该分析 `character` 和 `visibleArea` 来决定下一步行动。
2. 创建并返回一个包含所选方向的 `Action` 对象。

### 示例
```cpp
Action decideNextMove(const Character& character, const VisibleArea& visibleArea) {
    // 在这里实现你的决策逻辑
    // 这个例子中，我们寻找最近的豆子

    Direction bestDirection = Direction::STAY;
    int minDistance = std::numeric_limits<int>::max();
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;

    for (int y = 0; y < visibleArea.getHeight(); ++y) {
        for (int x = 0; x < visibleArea.getWidth(); ++x) {
            if (visibleArea.getCell(x, y) == VisibleArea::CellContent::DOT) {
                int distance = std::abs(x - centerX) + std::abs(y - centerY);
                if (distance < minDistance) {
                    minDistance = distance;
                    bestDirection = getRelativeDirection(visibleArea, x, y);
                }
            }
        }
    }

    return Action{bestDirection};
}
```

## 实现你的 Agent

要创建你的 Agent，请按照以下步骤进行：

1. 创建一个继承自 `AIInterface` 的新类。
2. 实现 `getAction` 方法。
3. 使用 `character` 参数来了解你当前的状态和类型（吃豆人或怪物）。
4. 分析 `visibleArea` 以做出明智的决策。
5. 返回一个包含你选择方向的 `Action`。

### 实现示例

```cpp
class MyPacmanAI : public AIInterface {
public:
    Action getAction(const Character& character, const VisibleArea& visibleArea) override {
        // 在这里实现你的吃豆人 AI 逻辑
        // 这是一个简单的找最近豆子的例子
        return decideNextMove(character, visibleArea);
    }
};
```

通过遵循本指南，你应该能够为吃豆人和怪物实现有效的 Agent，充分利用游戏框架提供的可见性系统和动作机制，同时遵守只能访问可见区域信息的限制。