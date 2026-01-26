# 管理系统实现指南

## 概述

本文档描述了协作式吃豆人游戏项目中学生需要实现的管理系统部分。管理系统负责处理游戏中的碰撞检测和行动执行，是连接AI决策和游戏状态更新的关键组件。

## 管理系统接口（ManagementInterface）

学生需要实现以下接口来创建管理系统：

```cpp
class ManagementInterface {
public:
    virtual bool processActions(const std::vector<Action>& actions, GameStateManager& gameState) = 0;

protected:
    // 辅助方法，学生可以在实现中使用
    bool isValidMove(const Position& from, const Direction& direction, const std::vector<std::vector<CellType>>& map) const;
    Position getNewPosition(const Position& current, const Direction& direction) const;
    void updateCharacterPosition(GameStateManager& gameState, int characterIndex, const Position& newPosition);
    void consumeDot(GameStateManager& gameState, const Position& position);
};
```

## 主要任务

1. 实现 `processActions` 方法，该方法是管理系统的核心，负责处理所有角色的行动并更新游戏状态。

2. 使用提供的辅助方法来执行常见操作，如验证移动、更新位置和消耗豆子。

3. 实现碰撞检测逻辑，处理Pacman与怪物、Pacman与豆子的交互。

4. 确保游戏规则得到正确执行，如Pacman吃掉豆子时增加得分。

5. 返回一个布尔值，指示游戏是否结束（例如，Pacman被吃掉或所有豆子都被吃完）。

## 实现指南

### processActions 方法

这是你需要实现的主要方法。它应该执行以下步骤：

1. 遍历所有角色的行动。
2. 对于每个行动，检查移动是否有效（使用 `isValidMove`）。
3. 如果移动有效，计算新的位置（使用 `getNewPosition`）。
4. 检查新位置是否会导致碰撞（Pacman与怪物）。
5. 如果没有碰撞，更新角色位置（使用 `updateCharacterPosition`）。
6. 如果是Pacman移动到了豆子的位置，消耗豆子并更新得分（使用 `consumeDot`）。
7. 检查游戏是否结束（所有豆子被吃完或Pacman被吃掉）。

### 碰撞检测

- 检查Pacman的新位置是否与任何怪物的位置重叠。
- 如果发生碰撞，游戏应该结束。

### 游戏规则执行

- 确保Pacman吃掉豆子时，得分增加，剩余豆子数量减少。
- 实现任何其他特殊规则，如吃掉能量豆后的效果（如果适用）。

## 辅助方法说明

1. `isValidMove`: 检查给定的移动是否有效（不会撞墙）。
2. `getNewPosition`: 根据当前位置和移动方向计算新的位置。
3. `updateCharacterPosition`: 在GameStateManager中更新角色的位置。
4. `consumeDot`: 当Pacman吃掉豆子时，更新GameStateManager中的得分和剩余豆子数量。

## 注意事项

- 确保你的实现是线程安全的，因为它可能在多线程环境中被调用。
- 考虑性能优化，尤其是在处理大地图时。
- 添加适当的错误处理和日志记录，以便于调试和维护。

## 测试建议

- 创建单元测试来验证你的 `processActions` 方法在各种情况下的行为。
- 测试边界情况，如角色试图移动到墙上或地图边缘。
- 模拟完整的游戏场景，确保长时间运行时不会出现问题。

## 扩展性考虑

- 你的实现应该易于扩展，以支持未来可能添加的新规则或互动类型。
- 考虑使用策略模式来实现不同的碰撞处理或得分计算方法。

通过实现这个管理系统，你将深入理解游戏逻辑的核心部分，并学习如何有效地协调复杂的游戏状态更新。祝你编码愉快！