#pragma once

#include <cmath>

namespace GameConfig {
// 地图配置
constexpr int MAP_WIDTH = 15;
constexpr int MAP_HEIGHT = 15;

// 可视范围配置
constexpr int VISIBILITY_RADIUS = 3; // 必须为奇数

// 游戏角色配置
constexpr int PACMAN_COUNT = 1;
constexpr int MONSTER_COUNT = 1;

// 游戏规则配置
constexpr int DOTS_TO_WIN = 100;

// 渲染配置
constexpr int CELL_SIZE = 32;  // 像素
constexpr int FRAME_RATE = 60; // 帧/秒
constexpr int DOT_RADIUS = 4;  // 豆子圆点半径（像素）
constexpr int CELL_MARGIN = 2; // 单元格之间的间距（像素）

// 颜色配置 (注意：Windows使用BGR字节序，格式为0x00BBGGRR)
constexpr unsigned long COLOR_WALL = 0x00006400;    // 暗绿色墙壁 RGB(0, 100, 0)
constexpr unsigned long COLOR_EMPTY = 0x00000000;   // 黑色背景 RGB(0, 0, 0)
constexpr unsigned long COLOR_DOT = 0x00FFFFFF;     // 白色豆子 RGB(255, 255, 255)
constexpr unsigned long COLOR_PACMAN = 0x0003EEEC;  // 亮黄色吃豆人 RGB(236, 238, 3)
constexpr unsigned long COLOR_MONSTER = 0x000000FF; // 红色怪物 RGB(255, 0, 0)

// AI 配置
constexpr int AI_THINK_TIME_MS = 100; // AI 决策时间限制（毫秒）

// 随机地图生成配置
constexpr float DOT_RATIO = 0.3f;                  // 豆子占空比
constexpr int MIN_DISTANCE_BETWEEN_CHARACTERS = 5; // 角色之间的最小距离
constexpr int OPEN_AREA_PROBABILITY = 5;          // 生成小型空白区域的概率（百分比）

// 分数配置
constexpr int POINTS_PER_DOT = 10;
} // namespace GameConfig
