# 学生A任务手册 - Pacman AI

## 任务概述

你的任务是实现**吃豆人的AI算法**，让吃豆人能够智能地在迷宫中移动，收集豆子并避开怪物。

**目标**：
- 收集尽可能多的豆子
- 避免被怪物抓住
- 实现高效的移动策略

---

## 你需要修改的文件

**文件路径**：`src/agents/pacman_ai.cpp`

**核心方法**：
```cpp
Action PacmanAI::getAction(const VisibleArea &visibleArea)
```

这个方法会在每个回合被调用，你需要返回吃豆人的移动方向。

---

## 可用的数据结构

### 1. Position（位置）

```cpp
struct Position {
    int x, y;

    // 计算曼哈顿距离
    int manhattanDistance(const Position &other) const;
};
```

**可用方法**：
- `pos.x`, `pos.y` - 获取坐标
- `pos.manhattanDistance(otherPos)` - 计算到另一个位置的曼哈顿距离

### 2. VisibleArea（可见区域）

吃豆人可以看到周围4格范围内的区域（9x9网格）。

```cpp
class VisibleArea {
    enum class CellContent {
        EMPTY,        // 空地
        WALL,         // 墙壁
        DOT,          // 豆子
        PACMAN,       // 吃豆人
        MONSTER,      // 怪物
        OVERBOUND,    //越界
        UNKNOWN       //超出视野范围
    };
};
```

**可用方法**：

```cpp
// 获取指定位置的内容
CellContent getCell(int x, int y) const;

// 获取可见区域的尺寸（吃豆人是9x9）
int getWidth() const;
int getHeight() const;

// 辅助方法：获取所有有效的移动方向（不会撞墙）
std::vector<Direction> getValidMoves(const VisibleArea &visibleArea) const;

// 辅助方法：检查某个方向是否可以移动
bool canMove(const VisibleArea &visibleArea, Direction dir) const;

// 辅助方法：获取方向的偏移量
void getDirectionOffset(Direction dir, int &dx, int &dy) const;
```

**重要提示**：
- 可见区域的中心 `(getWidth()/2, getHeight()/2)` 就是吃豆人的当前位置
- 坐标 `(0, 0)` 是可见区域的左上角
- **AI只能使用可见区域的相对坐标，无法获取全局地图坐标**
- 墙壁会阻挡视线（使用Bresenham算法）
- **吃豆人的视野半径为4格，比怪物（3格）更大，这是你的优势！**

### 3. Direction（方向）

```cpp
enum class Direction {
    UP,      // 向上
    DOWN,    // 向下
    LEFT,    // 向左
    RIGHT,   // 向右
    STAY     // 停留不动
};
```

### 4. Action（行动）

```cpp
struct Action {
    Direction direction;  // 移动方向
};
```

**返回示例**：
```cpp
return Action{Direction::UP};     // 向上移动
return Action{Direction::STAY};   // 停留不动
```

---

## 实现策略建议

### 初级策略（随机移动）

当前的示例代码就是随机移动：

```cpp
Action PacmanAI::getAction(const VisibleArea &visibleArea) {
    // 随机选择一个方向
    std::vector<Direction> directions = {
        Direction::UP, Direction::DOWN,
        Direction::LEFT, Direction::RIGHT
    };

    int randomIndex = rand() % directions.size();
    return Action{directions[randomIndex]};
}
```

### 中级策略（寻找最近的豆子）

1. **扫描可见区域**，找到所有豆子的位置
2. **计算距离**，找到最近的豆子（使用可见区域内的相对距离）
3. **移动方向**，朝最近的豆子移动

```cpp
Action PacmanAI::getAction(const VisibleArea &visibleArea) {
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;

    int nearestDotX = -1, nearestDotY = -1;
    int minDistance = 999999;

    // 扫描可见区域寻找豆子
    for (int y = 0; y < visibleArea.getHeight(); ++y) {
        for (int x = 0; x < visibleArea.getWidth(); ++x) {
            if (visibleArea.getCell(x, y) == VisibleArea::CellContent::DOT) {
                // 计算曼哈顿距离（在可见区域内）
                int dist = abs(x - centerX) + abs(y - centerY);
                if (dist < minDistance) {
                    minDistance = dist;
                    nearestDotX = x;
                    nearestDotY = y;
                }
            }
        }
    }

    // 如果找到豆子，朝它移动
    if (nearestDotX != -1) {
        // 优先横向移动
        if (nearestDotX < centerX) return Action{Direction::LEFT};
        if (nearestDotX > centerX) return Action{Direction::RIGHT};
        // 然后纵向移动
        if (nearestDotY < centerY) return Action{Direction::UP};
        if (nearestDotY > centerY) return Action{Direction::DOWN};
    }

    // 没有找到豆子，随机移动
    return Action{Direction::STAY};
}
```

### 高级策略（避开怪物 + 寻找豆子）

1. **检测怪物**，如果怪物太近，优先逃跑
2. **寻找豆子**，在安全的情况下收集豆子
3. **路径规划**，使用BFS或A*算法找到最优路径

**避开怪物示例**：

```cpp
// 检查可见区域内是否有怪物
bool hasNearbyMonster = false;
int monsterX = -1, monsterY = -1;
int centerX = visibleArea.getWidth() / 2;
int centerY = visibleArea.getHeight() / 2;

for (int y = 0; y < visibleArea.getHeight(); ++y) {
    for (int x = 0; x < visibleArea.getWidth(); ++x) {
        if (visibleArea.getCell(x, y) == VisibleArea::CellContent::MONSTER) {
            int dist = abs(x - centerX) + abs(y - centerY);
            if (dist <= 2) {  // 怪物距离小于等于2格
                hasNearbyMonster = true;
                monsterX = x;
                monsterY = y;
                break;
            }
        }
    }
    if (hasNearbyMonster) break;
}

// 如果有怪物靠近，逃跑（选择远离怪物的方向）
if (hasNearbyMonster) {
    if (monsterX < centerX) return Action{Direction::RIGHT};
    if (monsterX > centerX) return Action{Direction::LEFT};
    if (monsterY < centerY) return Action{Direction::DOWN};
    if (monsterY > centerY) return Action{Direction::UP};
}
```

### 专家策略（路径规划算法）

**BFS（广度优先搜索）**：
- 找到到达目标豆子的最短路径
- 避开墙壁和怪物

**A*算法**：
- 使用启发式函数（曼哈顿距离）
- 更高效地找到最优路径

**状态机**：
- 收集模式：寻找并收集豆子
- 逃跑模式：远离怪物
- 探索模式：探索未知区域

---

## 调试技巧

### 1. 使用控制台输出

```cpp
#include <iostream>

Action PacmanAI::getAction(const VisibleArea &visibleArea) {
    std::cout << "Visible area size: " << visibleArea.getWidth()
              << "x" << visibleArea.getHeight() << std::endl;

    // 你的AI逻辑...

    return Action{Direction::UP};
}
```

### 2. 可视化可见区域

```cpp
void printVisibleArea(const VisibleArea &visibleArea) {
    for (int y = 0; y < visibleArea.getHeight(); ++y) {
        for (int x = 0; x < visibleArea.getWidth(); ++x) {
            auto cell = visibleArea.getCell(x, y);
            switch (cell) {
                case VisibleArea::CellContent::WALL: std::cout << "#"; break;
                case VisibleArea::CellContent::DOT: std::cout << "."; break;
                case VisibleArea::CellContent::PACMAN: std::cout << "P"; break;
                case VisibleArea::CellContent::MONSTER: std::cout << "M"; break;
                case VisibleArea::CellContent::EMPTY: std::cout << " "; break;
            }
        }
        std::cout << std::endl;
    }
}
```

---

## 评分标准（参考）

- **基础分（60分）**：能够移动并收集豆子
- **中级分（80分）**：能够寻找最近的豆子并避开明显的危险
- **高级分（90分）**：实现路径规划，高效收集豆子
- **专家分（100分）**：复杂策略，能够应对各种情况，收集效率高

---

## 常见问题

**Q: 如何判断某个方向是否可以移动？**

A: 检查可见区域中心周围的格子是否是墙壁：
```cpp
// 可见区域的中心就是吃豆人当前位置
int centerX = visibleArea.getWidth() / 2;
int centerY = visibleArea.getHeight() / 2;

// 检查向上移动是否可行
int nextX = centerX;
int nextY = centerY - 1;  // 向上

if (nextY >= 0 && visibleArea.getCell(nextX, nextY) != VisibleArea::CellContent::WALL) {
    // 可以向上移动
    return Action{Direction::UP};
}

// 类似地检查其他方向
// 向下: nextY = centerY + 1
// 向左: nextX = centerX - 1
// 向右: nextX = centerX + 1
```

**Q: 如何知道游戏地图的大小？**

A: 可见区域只能看到周围3格，无法直接获取完整地图大小。你需要通过探索来了解地图。

**Q: 可以记录历史信息吗？**

A: 可以！你可以在 `PacmanAI` 类中添加成员变量来记录历史位置、已访问的区域等信息。
