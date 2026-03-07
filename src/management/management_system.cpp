#include "../../include/management_system.h"

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
