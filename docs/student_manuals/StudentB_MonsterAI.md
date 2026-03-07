# 学生B任务手册 - Monster AI

## 任务概述

你的任务是实现**怪物的AI算法**，让怪物能够智能地追捕吃豆人。

**目标**：
- 追踪并抓住吃豆人
- 实现有效的围堵策略
- 与其他怪物协作（如果有多个怪物）

---

## 你需要修改的文件

**文件路径**：`src/agents/monster_ai.cpp`

**核心方法**：
```cpp
Action MonsterAI::getAction(const VisibleArea &visibleArea)
```

这个方法会在每个回合被调用，你需要返回怪物的移动方向。

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

怪物可以看到周围3格范围内的区域（7x7网格）。

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

// 获取可见区域的尺寸（怪物是7x7）
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
- 可见区域的中心 `(getWidth()/2, getHeight()/2)` 就是怪物的当前位置
- 坐标 `(0, 0)` 是可见区域的左上角
- **AI只能使用可见区域的相对坐标，无法获取全局地图坐标**
- 墙壁会阻挡视线（使用Bresenham算法）
- **怪物的视野半径为3格，比吃豆人（4格）更小，需要更聪明的追踪策略！**

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
Action MonsterAI::getAction(const VisibleArea &visibleArea) {
    // 随机选择一个方向
    std::vector<Direction> directions = {
        Direction::UP, Direction::DOWN,
        Direction::LEFT, Direction::RIGHT
    };

    int randomIndex = rand() % directions.size();
    return Action{directions[randomIndex]};
}
```

### 中级策略（直接追踪）

1. **扫描可见区域**，寻找吃豆人
2. **计算方向**，朝吃豆人移动
3. **简单追踪**，直线接近目标


### 高级策略（智能追踪 + 预判）

1. **预测位置**，根据吃豆人的移动趋势预判其位置
2. **避开墙壁**，选择可行的路径
3. **记忆位置**，记住上次看到吃豆人的位置



### 专家策略（多怪物协作 + 围堵）

**围堵策略**：
- 如果有多个怪物，分散包围吃豆人
- 一个怪物直接追踪，其他怪物预判并堵截

**巡逻模式**：
- 没有看到吃豆人时，在地图上巡逻
- 覆盖不同的区域，提高发现概率

**路径规划**：
- 使用A*算法找到最优追踪路径
- 避开死胡同，防止被困

```cpp
// 多怪物协作示例（需要在类中添加静态变量）
class MonsterAI : public AIInterface {
  private:
    static Position sharedPacmanPos;  // 共享的吃豆人位置
    static int updateTime;            // 更新时间

  public:
    Action getAction(const Character &self, const VisibleArea &visibleArea) override {
        // 扫描并更新共享信息
        for (int y = 0; y < visibleArea.getHeight(); ++y) {
            for (int x = 0; x < visibleArea.getWidth(); ++x) {
                if (visibleArea.getCell(x, y) == VisibleArea::CellContent::PACMAN) {
                    sharedPacmanPos = visibleArea.toGamePosition(x, y);
                    updateTime = getCurrentTurn();  // 需要实现获取当前回合数
                }
            }
        }

        // 根据怪物ID选择不同的策略
        if (self.id % 2 == 0) {
            // 偶数ID：直接追踪
            return directChase(self, sharedPacmanPos);
        } else {
            // 奇数ID：预判拦截
            return interceptChase(self, sharedPacmanPos);
        }
    }
};
```

---

## 调试技巧

### 1. 使用控制台输出

```cpp
#include <iostream>

Action MonsterAI::getAction(const VisibleArea &visibleArea) {
    std::cout << "Monster AI decision making..." << std::endl;

    // 你的AI逻辑...

    return Action{Direction::UP};
}
```

### 2. 可视化追踪状态

你可以在 AI 中添加成员变量来跟踪目标位置：

```cpp
class MonsterAI : public AIInterface {
  private:
    Position lastSeenPacmanPos;  // 记录上次看到的吃豆人位置（相对坐标）

  public:
    void printChaseStatus(const Position &targetRelativePos) {
        std::cout << "Chasing target at relative position: ("
                  << targetRelativePos.x << ", " << targetRelativePos.y << ")" << std::endl;
    }
};
```

### 3. 调整游戏速度

在 `src/main.cpp` 中修改 `FRAMES_PER_TURN` 的值：
```cpp
const int FRAMES_PER_TURN = 30;  // 增大这个值会让游戏变慢
```

---

## 评分标准（参考）

- **基础分（60分）**：能够移动并在看到吃豆人时追踪
- **中级分（80分）**：能够持续追踪，记住上次看到的位置
- **高级分（90分）**：实现路径规划，避开障碍物
- **专家分（100分）**：多怪物协作，围堵策略，高效抓捕

---

## 常见问题

**Q: 如何判断某个方向是否可以移动？**

A: 检查可见区域中心周围的格子是否是墙壁：
```cpp
// 可见区域的中心就是怪物当前位置
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

**Q: 如何实现多个怪物的协作？**

A: 可以使用静态成员变量在不同怪物之间共享信息：
```cpp
class MonsterAI : public AIInterface {
  private:
    static Position sharedTarget;  // 所有怪物共享的目标位置
    static std::vector<int> monsterRoles;  // 每个怪物的角色分配
};
```

**Q: 怪物会被墙壁阻挡吗？**

A: 会的！管理系统会检查移动是否有效。如果怪物试图穿墙，会停留在原地。

**Q: 如何避免怪物聚在一起？**

A: 可以检测附近是否有其他怪物，如果有，选择不同的路径：
```cpp
// 检查可见区域内的其他怪物
int nearbyMonsters = 0;
for (int y = 0; y < visibleArea.getHeight(); ++y) {
    for (int x = 0; x < visibleArea.getWidth(); ++x) {
        if (visibleArea.getCell(x, y) == VisibleArea::CellContent::MONSTER) {
            nearbyMonsters++;
        }
    }
}

if (nearbyMonsters > 1) {
    // 有其他怪物在附近，选择不同的路径
}
```

---

## 参考资源

- **追踪算法**：直线追踪、预判追踪
- **路径规划**：A*算法、BFS算法
- **协作策略**：角色分配、信息共享
- **状态机**：追踪模式、巡逻模式、围堵模式

祝你实现出色的怪物AI！👾
