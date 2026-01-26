# 学生技术手册 (STUDENT_GUIDE.md)

## 👋 欢迎！

这是一个协作式吃豆人游戏项目。三位同学将分别负责不同的模块：
- **学生A**: 开发吃豆人的AI（让吃豆人聪明地移动）
- **学生B**: 开发怪物的AI（让怪物追踪吃豆人）
- **学生C**: 开发管理系统（判断游戏规则，如碰撞、吃豆子等）

## 🎮 游戏是怎么运行的？

游戏采用**回合制**，每0.5秒执行一个回合：

```
1. AI决策阶段
   ├─ 吃豆人AI决定往哪走
   └─ 怪物AI决定往哪走

2. 管理系统处理
   ├─ 检查是否撞墙（撞墙就不动）
   ├─ 移动所有角色
   ├─ 检查吃豆人是否吃到豆子
   └─ 检查吃豆人是否被怪物抓到

3. 渲染显示
   └─ 在屏幕上画出地图和角色
```

## 📚 基础知识

### 1. 坐标系统

地图是一个15x15的网格，左上角是(0,0)：

```
(0,0) ─────────────> X轴
  │
  │    #  #  #  #  #
  │    #  .  .  .  #
  │    #  .  P  .  #
  │    #  .  .  M  #
  │    #  #  #  #  #
  ↓
 Y轴

P = 吃豆人 (Pacman)
M = 怪物 (Monster)
# = 墙 (Wall)
. = 豆子 (Dot)
```

### 2. 方向 (Direction)

角色可以往四个方向移动，或者停留不动：

```cpp
Direction::UP      // 向上 (y减1)
Direction::DOWN    // 向下 (y加1)
Direction::LEFT    // 向左 (x减1)
Direction::RIGHT   // 向右 (x加1)
Direction::STAY    // 不动
```

### 3. 位置 (Position)

表示地图上的一个点：

```cpp
Position pos(5, 3);  // x=5, y=3
int x = pos.x;       // 获取x坐标
int y = pos.y;       // 获取y坐标

// 计算两点之间的曼哈顿距离
int distance = pos.manhattanDistance(otherPos);
```

### 4. 可见区域 (VisibleArea)

AI只能看到角色周围一定范围内的地图（默认半径为3格）：

```
视野范围示例（半径=3）：

  ? ? ? ? ? ? ?
  ? . . . . . ?
  ? . . . . . ?
  ? . . P . . ?    P在中心
  ? . . . . . ?
  ? . . . . . ?
  ? ? ? ? ? ? ?

? = 看不见的区域
. = 可以看见的区域
```

**重要**: 墙会阻挡视线！如果中间有墙，后面的东西就看不见。

## 🎯 学生A - 吃豆人AI开发

### 你的任务

让吃豆人聪明地移动，吃掉所有豆子，同时避开怪物。

### 需要修改的文件

- `src/agents/pacman_ai.cpp`

### 当前实现

现在的吃豆人是**随机移动**的：

```cpp
Action PacmanAI::getAction(const Character& character, const VisibleArea& visibleArea) {
    // 获取所有有效的移动方向（不会撞墙的方向）
    std::vector<Direction> validMoves = getValidMoves(visibleArea);

    // 随机选择一个方向
    if (!validMoves.empty()) {
        int index = randomEngine() % validMoves.size();
        return Action{validMoves[index]};
    }

    // 如果没有有效移动，停留
    return Action{Direction::STAY};
}
```

### 你可以获得的信息

1. **character** - 吃豆人自己的信息：
   ```cpp
   Position pos = character.position;  // 当前位置
   bool alive = character.isAlive;     // 是否存活
   bool powered = character.isPowered; // 是否吃了能量豆
   ```

2. **visibleArea** - 可见区域的信息：
   ```cpp
   int width = visibleArea.getWidth();   // 可见区域宽度（总是7）
   int height = visibleArea.getHeight(); // 可见区域高度（总是7）

   // 获取某个位置的内容（吃豆人在中心，即(3,3)）
   auto content = visibleArea.getCell(x, y);

   // content 可能是：
   // - CellContent::EMPTY        空地
   // - CellContent::WALL         墙
   // - CellContent::DOT          豆子
   // - CellContent::POWER_PELLET 能量豆
   // - CellContent::PACMAN       吃豆人（自己）
   // - CellContent::MONSTER      怪物
   ```

### 辅助方法

框架提供了一些辅助方法，你可以直接使用：

```cpp
// 获取所有不会撞墙的方向
std::vector<Direction> validMoves = getValidMoves(visibleArea);

// 检查某个方向是否可以移动
bool canGo = canMove(visibleArea, Direction::UP);

// 获取朝向目标位置的方向
Direction dir = getRelativeDirection(visibleArea, targetX, targetY);
```

### 改进建议

你可以让吃豆人更聪明，例如：

1. **寻找最近的豆子**
   ```cpp
   // 伪代码
   找到可见区域内最近的豆子
   朝着豆子的方向移动
   ```

2. **躲避怪物**
   ```cpp
   // 伪代码
   如果看到怪物：
       如果吃了能量豆：
           追着怪物跑（可以吃掉怪物）
       否则：
           远离怪物
   ```

3. **优先吃能量豆**
   ```cpp
   // 伪代码
   如果看到能量豆：
       优先朝能量豆移动
   ```

### 示例：寻找最近的豆子

```cpp
Action PacmanAI::getAction(const Character& character, const VisibleArea& visibleArea) {
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;

    // 寻找最近的豆子
    int minDistance = 999;
    int targetX = centerX;
    int targetY = centerY;

    for (int y = 0; y < visibleArea.getHeight(); ++y) {
        for (int x = 0; x < visibleArea.getWidth(); ++x) {
            auto content = visibleArea.getCell(x, y);
            if (content == VisibleArea::CellContent::DOT) {
                int distance = std::abs(x - centerX) + std::abs(y - centerY);
                if (distance < minDistance) {
                    minDistance = distance;
                    targetX = x;
                    targetY = y;
                }
            }
        }
    }

    // 朝着豆子的方向移动
    Direction dir = getRelativeDirection(visibleArea, targetX, targetY);

    // 检查这个方向是否可以走
    if (canMove(visibleArea, dir)) {
        return Action{dir};
    }

    // 如果不能走，随机选择一个方向
    std::vector<Direction> validMoves = getValidMoves(visibleArea);
    if (!validMoves.empty()) {
        int index = randomEngine() % validMoves.size();
        return Action{validMoves[index]};
    }

    return Action{Direction::STAY};
}
```

---

## 👾 学生B - 怪物AI开发

### 你的任务

让怪物聪明地追踪吃豆人，或者在地图上巡逻。

### 需要修改的文件

- `src/agents/monster_ai.cpp`

### 当前实现

现在的怪物也是**随机移动**的（和吃豆人一样）。

### 你可以获得的信息

和吃豆人AI一样，你可以获得：
- `character` - 怪物自己的信息
- `visibleArea` - 可见区域的信息

### 改进建议

1. **追踪吃豆人**
   ```cpp
   // 伪代码
   如果看到吃豆人：
       朝着吃豆人的方向移动
   否则：
       随机移动或巡逻
   ```

2. **包围策略**
   ```cpp
   // 伪代码
   多个怪物协作，从不同方向包围吃豆人
   （这个比较难，需要怪物之间通信）
   ```

3. **躲避能量豆状态的吃豆人**
   ```cpp
   // 伪代码
   如果吃豆人吃了能量豆：
       远离吃豆人
   否则：
       追踪吃豆人
   ```

### 示例：追踪吃豆人

```cpp
Action MonsterAI::getAction(const Character& character, const VisibleArea& visibleArea) {
    int centerX = visibleArea.getWidth() / 2;
    int centerY = visibleArea.getHeight() / 2;

    // 寻找吃豆人
    bool foundPacman = false;
    int pacmanX = centerX;
    int pacmanY = centerY;

    for (int y = 0; y < visibleArea.getHeight(); ++y) {
        for (int x = 0; x < visibleArea.getWidth(); ++x) {
            auto content = visibleArea.getCell(x, y);
            if (content == VisibleArea::CellContent::PACMAN) {
                foundPacman = true;
                pacmanX = x;
                pacmanY = y;
                break;
            }
        }
        if (foundPacman) break;
    }

    if (foundPacman) {
        // 朝着吃豆人的方向移动
        Direction dir = getRelativeDirection(visibleArea, pacmanX, pacmanY);

        if (canMove(visibleArea, dir)) {
            return Action{dir};
        }
    }

    // 如果没看到吃豆人，随机移动
    std::vector<Direction> validMoves = getValidMoves(visibleArea);
    if (!validMoves.empty()) {
        int index = randomEngine() % validMoves.size();
        return Action{validMoves[index]};
    }

    return Action{Direction::STAY};
}
```

---

## 🎮 学生C - 管理系统开发

### 你的任务

实现游戏规则的判定，包括：
- 撞墙判定（角色不能穿墙）
- 豆子判定（吃豆人吃到豆子）
- 碰撞判定（吃豆人和怪物相遇）
- 胜利条件（所有豆子被吃完）

### 需要修改的文件

- `src/management/management_system.cpp`

### 当前实现

现在的管理系统已经实现了基础功能：

```cpp
bool ManagementSystem::processActions(const std::vector<Action>& actions,
                                      GameStateManager& gameState) {
    auto characters = gameState.getCharacters();

    // 第一步：验证并执行所有移动
    for (size_t i = 0; i < characters.size(); ++i) {
        Position newPos = getNewPosition(characters[i].position, actions[i].direction);

        // 撞墙判定
        if (isValidMove(gameState, newPos)) {
            characters[i].position = newPos;
        }
    }

    gameState.setCharacters(characters);

    // 第二步：豆子判定
    for (const auto& character : characters) {
        if (character.type == CharacterType::PACMAN) {
            if (gameState.getMap().hasDot(character.position)) {
                consumeDot(gameState, character.position);
            }
            else if (gameState.getMap().hasPowerPellet(character.position)) {
                consumePowerPellet(gameState, character.position);
            }
        }
    }

    // 第三步：敌人判定
    if (checkPacmanMonsterCollision(gameState)) {
        return false;  // 游戏结束
    }

    // 第四步：检查胜利条件
    if (checkWinCondition(gameState)) {
        return false;  // 游戏胜利
    }

    return true;  // 游戏继续
}
```

### 你可以使用的辅助方法

```cpp
// 检查某个位置是否可以移动（不是墙）
bool isValid = isValidMove(gameState, position);

// 根据方向计算新位置
Position newPos = getNewPosition(currentPos, Direction::UP);

// 更新角色位置
updateCharacterPosition(gameState, characterIndex, newPos);

// 消耗豆子
consumeDot(gameState, position);

// 消耗能量豆
consumePowerPellet(gameState, position);

// 检查吃豆人是否与怪物碰撞
bool collision = checkPacmanMonsterCollision(gameState);

// 检查是否获胜
bool win = checkWinCondition(gameState);
```

### 你可以获得的信息

```cpp
// 获取地图
const GameMap& map = gameState.getMap();

// 获取所有角色
const std::vector<Character>& characters = gameState.getCharacters();

// 获取分数
int score = gameState.getScore();

// 获取剩余豆子数
int dots = gameState.getRemainingDots();

// 检查某个位置的内容
bool isWall = map.isWall(position);
bool isEmpty = map.isEmpty(position);
bool hasDot = map.hasDot(position);
bool hasPowerPellet = map.hasPowerPellet(position);
```

### 改进建议

1. **能量豆效果**
   ```cpp
   // 伪代码
   如果吃豆人吃了能量豆：
       设置能量豆状态
       如果吃豆人碰到怪物：
           怪物被吃掉（加分）
       否则：
           吃豆人被抓（游戏结束）
   ```

2. **更复杂的碰撞检测**
   ```cpp
   // 伪代码
   检测角色是否在移动路径上相遇
   而不仅仅是最终位置相同
   ```

3. **特殊道具系统**
   ```cpp
   // 伪代码
   添加新的道具类型
   实现道具效果
   ```

### 示例：能量豆效果处理

```cpp
bool ManagementSystem::processActions(const std::vector<Action>& actions,
                                      GameStateManager& gameState) {
    auto characters = gameState.getCharacters();

    // 移动所有角色...
    // （省略移动代码）

    // 处理豆子
    for (const auto& character : characters) {
        if (character.type == CharacterType::PACMAN) {
            if (gameState.getMap().hasDot(character.position)) {
                consumeDot(gameState, character.position);
            }
            else if (gameState.getMap().hasPowerPellet(character.position)) {
                consumePowerPellet(gameState, character.position);
                // 能量豆效果已经在consumePowerPellet中处理
            }
        }
    }

    // 检查碰撞
    characters = gameState.getCharacters();  // 重新获取（可能已更新）

    // 找到吃豆人
    Character* pacman = nullptr;
    for (auto& c : characters) {
        if (c.type == CharacterType::PACMAN) {
            pacman = &c;
            break;
        }
    }

    if (pacman) {
        // 检查是否与怪物碰撞
        for (auto& monster : characters) {
            if (monster.type == CharacterType::MONSTER &&
                monster.position == pacman->position) {

                if (pacman->isPowered) {
                    // 吃豆人吃掉怪物
                    monster.isAlive = false;
                    gameState.incrementScore(GameConfig::POINTS_PER_MONSTER);
                } else {
                    // 吃豆人被抓
                    return false;
                }
            }
        }
    }

    // 检查胜利条件
    if (checkWinCondition(gameState)) {
        return false;
    }

    return true;
}
```

---

## 🔧 编译和运行

### 编译项目

```bash
# 生成构建文件
cmake -B build -G Ninja

# 编译
cmake --build build
```

### 运行游戏

```bash
# Windows
build\pacman_game.exe

# 或者直接双击 build 文件夹中的 pacman_game.exe
```

### 游戏控制

- **空格键**: 暂停/恢复游戏
- **ESC键**: 退出游戏

---

## 💡 调试技巧

### 1. 使用输出调试

在代码中添加输出语句：

```cpp
#include <iostream>

// 在你的代码中
std::cout << "Pacman position: " << character.position.x << ", "
          << character.position.y << std::endl;
```

**注意**: 输出会显示在命令行窗口，不是游戏窗口。

### 2. 修改游戏速度

在 `src/main.cpp` 中修改 `FRAMES_PER_TURN` 的值：

```cpp
const int FRAMES_PER_TURN = 30;  // 数字越大，游戏越慢
```

- 30 = 每0.5秒一回合
- 60 = 每1秒一回合
- 15 = 每0.25秒一回合

### 3. 修改地图大小和角色数量

在 `include/config.h` 中修改：

```cpp
constexpr int MAP_WIDTH = 15;      // 地图宽度
constexpr int MAP_HEIGHT = 15;     // 地图高度
constexpr int PACMAN_COUNT = 1;    // 吃豆人数量
constexpr int MONSTER_COUNT = 1;   // 怪物数量
constexpr int VISIBILITY_RADIUS = 3; // 视野半径
```

---

## ⚠️ 常见问题

### Q: 编译出错怎么办？

A: 检查以下几点：
1. 文件是否保存为 UTF-8 编码
2. 是否有语法错误（缺少分号、括号不匹配等）
3. 是否包含了必要的头文件

### Q: 游戏运行后立即结束？

A: 可能是：
1. 吃豆人和怪物在同一位置生成（立即碰撞）
2. 地图上没有豆子（立即胜利）
3. AI返回了无效的移动

### Q: 角色不动？

A: 检查：
1. AI是否返回了 `Direction::STAY`
2. 是否所有方向都被墙挡住了
3. 游戏是否被暂停了（按空格键恢复）

### Q: 看不到角色？

A: 可能是：
1. 角色在墙里面（生成位置有问题）
2. 渲染颜色和背景色相同
3. 窗口太小，角色在屏幕外

---

## 📖 进阶学习

### 推荐算法

1. **寻路算法**:
   - BFS（广度优先搜索）- 找最短路径
   - A* 算法 - 更高效的寻路
   - Dijkstra 算法 - 带权重的寻路

2. **决策算法**:
   - 状态机 - 不同状态下不同行为
   - 行为树 - 复杂的决策逻辑
   - 极小极大算法 - 预测对手行为

3. **协作算法**:
   - 多智能体协作
   - 通信协议设计

### 参考资料

- `docs/` 文件夹 - 详细技术文档
- `TODO/` 文件夹 - 开发任务清单
- 在线搜索 "游戏AI算法"、"寻路算法"

---

## 🎉 祝你成功！

记住：
- 先让代码能跑起来
- 再慢慢优化和改进
- 遇到问题多调试、多输出
- 不要害怕尝试新想法

加油！💪
