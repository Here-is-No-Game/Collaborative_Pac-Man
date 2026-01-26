#pragma once

#include <cmath>

namespace GameConfig {
    // 地图配置
    constexpr int MAP_WIDTH = 15;
    constexpr int MAP_HEIGHT = 15;

    // 可视范围配置
    constexpr int VISIBILITY_RADIUS = 3;  // 必须为奇数

    // 游戏角色配置
    constexpr int PACMAN_COUNT = 1;
    constexpr int MONSTER_COUNT = 1;

    // 游戏规则配置
    constexpr int DOTS_TO_WIN = 100;

    // 渲染配置
    constexpr int CELL_SIZE = 32;  // 像素
    constexpr int FRAME_RATE = 60;  // 帧/秒

    // AI 配置
    constexpr int AI_THINK_TIME_MS = 100;  // AI 决策时间限制（毫秒）

    // 随机地图生成配置
    constexpr float DOT_RATIO = 0.3f;  // 豆子占空比
    constexpr int MIN_DISTANCE_BETWEEN_CHARACTERS = 5;  // 角色之间的最小距离

    // 分数配置
    constexpr int POINTS_PER_DOT = 10;
}
