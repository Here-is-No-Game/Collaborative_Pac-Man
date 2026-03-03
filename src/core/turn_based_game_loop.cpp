#include "../../include/turn_based_game_loop.h"

TurnBasedGameLoop::TurnBasedGameLoop(const GameMap &map, const std::vector<Character> &characters)
    : gameState(map, characters), pacmanVisibilitySystem(GameConfig::PACMAN_VISIBILITY_RADIUS),
      monsterVisibilitySystem(GameConfig::MONSTER_VISIBILITY_RADIUS), isRunning(false), currentTurn(0) {

    // 为每个角色初始化AI代理槽位
    aiAgents.resize(characters.size());
}

void TurnBasedGameLoop::setAIAgent(int characterIndex, std::unique_ptr<AIInterface> ai) {
    if (characterIndex >= 0 && characterIndex < static_cast<int>(aiAgents.size())) {
        aiAgents[characterIndex] = std::move(ai);
    }
}

void TurnBasedGameLoop::setManagementSystem(std::unique_ptr<ManagementInterface> management) {
    managementSystem = std::move(management);
}

void TurnBasedGameLoop::start() {
    isRunning = true;
    currentTurn = 0;
}

void TurnBasedGameLoop::stop() { isRunning = false; }

bool TurnBasedGameLoop::executeTurn() {
    if (!isRunning || controlSystem.getPaused()) {
        return true; // 游戏暂停或未运行，但不结束
    }

    if (!managementSystem) {
        return false; // 没有管理系统，无法继续
    }

    // 第一步：收集所有AI的决策
    std::vector<Action> actions = collectAIActions();

    // 第二步：管理系统处理行动
    bool continueGame = managementSystem->processActions(actions, gameState);

    // 第三步：更新回合数
    currentTurn++;
    gameState.incrementTurnCount();

    // 记录执行后的状态（用于回放）
    controlSystem.recordState(gameState);

    // 返回游戏是否继续
    return continueGame;
}

std::vector<Action> TurnBasedGameLoop::collectAIActions() {
    std::vector<Action> actions;
    const auto &characters = gameState.getCharacters();

    for (size_t i = 0; i < characters.size(); ++i) {
        Action action{Direction::STAY}; // 默认行动

        // 如果有对应的AI代理，获取其决策
        if (i < aiAgents.size() && aiAgents[i]) {
            // 根据角色类型选择对应的视野系统
            VisibilitySystem &visSystem =
                (characters[i].type == CharacterType::PACMAN) ? pacmanVisibilitySystem : monsterVisibilitySystem;

            // 计算可见区域
            VisibleArea visibleArea =
                visSystem.calculateVisibleArea(characters[i].position, gameState.getMap(), characters);

            // 获取AI决策
            action = aiAgents[i]->getAction(visibleArea);
        }

        actions.push_back(action);
    }

    return actions;
}

void TurnBasedGameLoop::setGameState(const GameStateManager &state) { gameState = state; }
