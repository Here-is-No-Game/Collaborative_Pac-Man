#include "../../include/monster_ai.h"
#include <ctime>

MonsterAI::MonsterAI() { randomEngine.seed(static_cast<unsigned int>(std::time(nullptr))); }

MonsterAI::MonsterAI(unsigned int seed) { randomEngine.seed(seed); }

Action MonsterAI::getAction(const Character &character, const VisibleArea &visibleArea) {
    // 获取所有有效的移动方向
    std::vector<Direction> validMoves = getValidMoves(visibleArea);

    // 随机选择一个方向
    if (!validMoves.empty()) {
        int index = randomEngine() % validMoves.size();
        return Action{validMoves[index]};
    }

    // 如果没有有效移动，停留
    return Action{Direction::STAY};
}
