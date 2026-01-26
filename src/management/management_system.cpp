#include "../../include/management_system.h"

bool ManagementSystem::processActions(const std::vector<Action>& actions, GameStateManager& gameState) {
    auto characters = gameState.getCharacters();

    // 确保行动数量与角色数量匹配
    if (actions.size() != characters.size()) {
        return false;
    }

    // 第一步：验证并执行所有移动
    for (size_t i = 0; i < characters.size(); ++i) {
        Position newPos = getNewPosition(characters[i].position, actions[i].direction);

        // 撞墙判定：检查新位置是否有效
        if (isValidMove(gameState, newPos)) {
            characters[i].position = newPos;
        }
        // 如果撞墙，角色保持在原位置
    }

    // 更新角色位置到游戏状态
    gameState.setCharacters(characters);

    // 第二步：豆子判定 - 检查吃豆人是否吃到豆子
    for (const auto& character : characters) {
        if (character.type == CharacterType::PACMAN) {
            // 检查是否吃到豆子
            if (gameState.getMap().hasDot(character.position)) {
                consumeDot(gameState, character.position);
            }
        }
    }

    // 第三步：敌人判定 - 检查吃豆人是否与怪物碰撞
    if (checkPacmanMonsterCollision(gameState)) {
        // 碰撞发生，游戏结束
        return false;
    }

    // 第四步：检查胜利条件
    if (checkWinCondition(gameState)) {
        // 所有豆子被吃完，游戏胜利
        return false;
    }

    // 游戏继续
    return true;
}
