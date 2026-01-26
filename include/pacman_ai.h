#pragma once

#include "ai_interface.h"
#include <random>

// 简单的吃豆人AI - 随机移动示例
// 这是给学生A的参考实现，学生可以在此基础上改进策略
class PacmanAI : public AIInterface {
private:
    std::mt19937 randomEngine;

public:
    PacmanAI();
    explicit PacmanAI(unsigned int seed);

    // 实现AI决策：随机选择一个有效的移动方向
    Action getAction(const Character& character, const VisibleArea& visibleArea) override;
};
