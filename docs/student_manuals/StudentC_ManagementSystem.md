# 学生C任务手册 - Management System

## 任务概述

你的任务是实现**游戏管理系统**，负责处理所有角色的行动、判定游戏规则、更新分数和检测游戏结束条件。

**目标**：
- 处理吃豆人和怪物的移动
- 实现豆子收集逻辑
- 检测碰撞（吃豆人与怪物）
- 判定游戏胜负条件
- 管理分数系统

---

## 你需要修改的文件

**文件路径**：`src/management/management_system.cpp`

**核心方法**：
```cpp
bool ManagementSystem::processActions(const std::vector<Action> &actions, GameStateManager &gameState)
```

这个方法会在每个回合被调用，处理所有角色的行动并更新游戏状态。

**返回值**：
- `true` - 游戏继续
- `false` - 游戏结束

---

## 当前示例代码

当前的示例代码非常简单，只实现了基础功能：

```cpp
bool ManagementSystem::processActions(const std::vector<Action> &actions, GameStateManager &gameState) {
    auto characters = gameState.getCharacters();

    // 确保行动数量与角色数量匹配
    if (actions.size() != characters.size()) {
        return false;
    }

    // 处理所有角色的移动
    for (size_t i = 0; i < characters.size(); ++i) {
        Position newPos = getNewPosition(characters[i].position, actions[i].direction);

        // 根据角色类型进行不同的处理
        if (characters[i].type == CharacterType::PACMAN) {
            // 吃豆人：检查是否越界（撞墙）
            if (isValidMove(gameState, newPos)) {
                characters[i].position = newPos;
            }
            // 如果撞墙，吃豆人保持在原位置
        } else if (characters[i].type == CharacterType::MONSTER) {
            // 怪物：直接移动，每次移动加1分
            characters[i].position = newPos;
            gameState.incrementMonsterScore(1);
        }
    }

    // 更新角色位置到游戏状态
    gameState.setCharacters(characters);

    // 游戏继续
    return true;
}
```

**你需要添加的功能**：
1. 豆子收集判定
2. 吃豆人与怪物的碰撞检测
3. 游戏胜负条件判定
4. 更完善的分数系统

---

## 可用的数据结构

### 1. GameStateManager（游戏状态管理器）

这是你的主要工具，用于获取和修改游戏状态。

**获取信息的方法**：

```cpp
// 获取地图
const GameMap &getMap() const;

// 获取所有角色
const std::vector<Character> &getCharacters() const;

// 获取特定角色
const Character &getCharacter(int index) const;
const Character &getPacman() const;
std::vector<Character> getMonsters() const;

// 获取分数和状态
int getPacmanScore() const;
int getMonsterScore() const;
int getRemainingDots() const;
int getTurnCount() const;

// 查询地图信息
CellType getCellType(const Position &pos) const;
bool isValidPosition(const Position &pos) const;
bool isWall(const Position &pos) const;
bool hasDot(const Position &pos) const;
```

**修改状态的方法**：

```cpp
// 更新角色位置
void updateCharacterPosition(int index, const Position &newPos);

// 消耗豆子（会自动减少remainingDots）
void consumeDot(const Position &pos);

// 设置角色存活状态
void setCharacterAlive(int index, bool alive);

// 增加分数
void incrementPacmanScore(int points);
void incrementMonsterScore(int points);

// 更新所有角色（批量更新）
void setCharacters(const std::vector<Character> &chars);
```

### 2. Character（角色）

```cpp
struct Character {
    int id;                    // 角色ID
    Position position;         // 当前位置
    CharacterType type;        // 角色类型（PACMAN或MONSTER）
    bool isAlive;             // 是否存活
};
```

### 3. Position（位置）

```cpp
struct Position {
    int x, y;

    // 计算曼哈顿距离
    int manhattanDistance(const Position &other) const;

    // 比较运算符
    bool operator==(const Position &other) const;
    bool operator!=(const Position &other) const;
};
```

### 4. Action（行动）

```cpp
struct Action {
    Direction direction;  // 移动方向
};

enum class Direction {
    UP, DOWN, LEFT, RIGHT, STAY
};
```

### 5. CellType（地图单元格类型）

```cpp
enum class CellType {
    EMPTY,        // 空地
    WALL,         // 墙壁
    DOT           // 豆子
};
```

---

## 辅助方法（已提供）

### 1. isValidMove - 检查移动是否有效

```cpp
bool isValidMove(const GameStateManager &gameState, const Position &pos) const;
```

**功能**：检查目标位置是否可以移动（不是墙壁）

**返回值**：
- `true` - 可以移动（空地或有豆子）
- `false` - 不能移动（墙壁或越界）

**使用示例**：
```cpp
Position newPos = getNewPosition(character.position, action.direction);
if (isValidMove(gameState, newPos)) {
    // 可以移动
    character.position = newPos;
}
```

### 2. getNewPosition - 计算新位置

```cpp
Position getNewPosition(const Position &currentPos, Direction dir) const;
```

**功能**：根据当前位置和移动方向，计算新位置

**使用示例**：
```cpp
Position newPos = getNewPosition(character.position, Direction::UP);
// newPos.y 会比 character.position.y 小1
```

---

## 实现建议

### 第一步：实现豆子收集



### 第二步：实现碰撞检测



### 第三步：实现胜利条件



---

## 高级功能（可选）

### 1. 更复杂的分数系统

```cpp
// 根据不同情况给予不同分数
if (gameState.hasDot(newPos)) {
    gameState.consumeDot(newPos);

    // 连续收集豆子有额外奖励
}

// 怪物抓住吃豆人时的分数
if (collision) {
    //...
}
```

### 2. 回合限制

```cpp
// 设置最大回合数
const int MAX_TURNS = 1000;

if (gameState.getTurnCount() >= MAX_TURNS) {
    // 达到最大回合数，游戏结束
    // 根据分数判定胜负
    return false;
}
```



---

## 调试技巧

### 1. 输出调试信息

```cpp
#include <iostream>

bool ManagementSystem::processActions(const std::vector<Action> &actions, GameStateManager &gameState) {
    std::cout << "=== Turn " << gameState.getTurnCount() << " ===" << std::endl;
    std::cout << "Remaining dots: " << gameState.getRemainingDots() << std::endl;
    std::cout << "Pacman score: " << gameState.getPacmanScore() << std::endl;
    std::cout << "Monster score: " << gameState.getMonsterScore() << std::endl;

    // 你的逻辑...

    return true;
}
```

### 2. 检查碰撞

```cpp
void checkCollisions(const GameStateManager &gameState) {
    const Character &pacman = gameState.getPacman();
    std::vector<Character> monsters = gameState.getMonsters();

    for (const auto &monster : monsters) {
        int distance = pacman.position.manhattanDistance(monster.position);
        std::cout << "Distance to monster " << monster.id << ": " << distance << std::endl;

        if (distance == 0) {
            std::cout << "COLLISION DETECTED!" << std::endl;
        }
    }
}
```

### 3. 验证游戏状态

```cpp
void validateGameState(const GameStateManager &gameState) {
    // 检查角色位置是否合法
    for (const auto &character : gameState.getCharacters()) {
        if (gameState.isWall(character.position)) {
            std::cout << "ERROR: Character in wall!" << std::endl;
        }
    }

    // 检查豆子数量
    if (gameState.getRemainingDots() < 0) {
        std::cout << "ERROR: Negative dots!" << std::endl;
    }
}
```

---

## 评分标准（参考）

- **基础分（60分）**：实现基本的移动处理和墙壁碰撞检测
- **中级分（80分）**：实现豆子收集和简单的碰撞检测
- **高级分（90分）**：完整的游戏规则，包括胜负判定和分数系统
- **专家分（100分）**：高级功能（复杂分数系统、回合限制、特殊机制）

---

## 常见问题

**Q: 如何区分吃豆人和怪物？**

A: 使用 `character.type` 字段：
```cpp
if (character.type == CharacterType::PACMAN) {
    // 这是吃豆人
} else if (character.type == CharacterType::MONSTER) {
    // 这是怪物
}
```

**Q: 怪物需要检查墙壁碰撞吗？**

A: 这取决于你的设计。当前示例中怪物不检查墙壁，但你可以添加这个检查让游戏更合理。

**Q: 如何判断游戏结束？**

A: 返回 `false` 表示游戏结束。常见的结束条件：
- 吃豆人被怪物抓住
- 所有豆子被收集完
- 达到最大回合数（可选）

**Q: 分数系统应该如何设计？**

A: 这完全由你决定！一些建议：
- 吃豆人收集豆子：+10分
- 怪物抓住吃豆人：+100分
- 连续收集豆子：额外奖励
- 快速完成游戏：时间奖励

**Q: 如何处理同时到达同一位置的情况？**

A: 可以按照角色ID顺序处理，或者实现更复杂的优先级系统。

---

## 配置参数

在 `include/config.h` 中可以找到游戏配置：

```cpp
namespace GameConfig {
    constexpr int MAP_WIDTH = 15;
    constexpr int MAP_HEIGHT = 15;
    constexpr int DOTS_TO_WIN = 100;
    constexpr int POINTS_PER_DOT = 10;
    // ... 更多配置
}
```

---

## 参考资源

- **游戏规则设计**：平衡性、公平性
- **碰撞检测**：位置比较、距离计算
- **状态管理**：游戏状态的一致性
- **分数系统**：激励机制设计

祝你实现出色的管理系统！🎯
