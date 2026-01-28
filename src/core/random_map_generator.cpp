#include "../../include/random_map_generator.h"
#include <algorithm>
#include <ctime>

RandomMapGenerator::RandomMapGenerator(int w, int h, float ratio)
    : width(w), height(h), dotRatio(ratio), currentMap(nullptr) {
    randomEngine.seed(static_cast<unsigned int>(std::time(nullptr)));
}

void RandomMapGenerator::setSeed(unsigned int seed) { randomEngine.seed(seed); }

GameMap RandomMapGenerator::generateMap() {
    GameMap map(width, height);
    currentMap = &map;

    // 初始化为全墙
    map.initialize();

    // 创建边界
    for (int x = 0; x < width; ++x) {
        map.setCell(x, 0, CellType::WALL);
        map.setCell(x, height - 1, CellType::WALL);
    }
    for (int y = 0; y < height; ++y) {
        map.setCell(0, y, CellType::WALL);
        map.setCell(width - 1, y, CellType::WALL);
    }

    // 使用深度优先搜索生成迷宫（一格宽的通道）
    generateMaze(1, 1);

    // 放置豆子
    placeDots();

    map.setTotalDots(map.countDots());

    return map;
}

void RandomMapGenerator::generateMaze(int startX, int startY) {
    // 深度优先搜索生成迷宫
    std::vector<Position> stack;
    Position start(startX, startY);
    stack.push_back(start);
    currentMap->setCell(start, CellType::EMPTY);

    // 四个方向：上、下、左、右
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    while (!stack.empty()) {
        Position current = stack.back();

        // 获取所有未访问的邻居（距离2格的位置）
        std::vector<int> validDirections;
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i] * 2;
            int ny = current.y + dy[i] * 2;

            // 检查是否在边界内且未访问
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1) {
                Position neighbor(nx, ny);
                if (currentMap->isWall(neighbor)) {
                    validDirections.push_back(i);
                }
            }
        }

        if (!validDirections.empty()) {
            // 随机选择一个方向
            int dirIndex = validDirections[randomEngine() % validDirections.size()];
            int nx = current.x + dx[dirIndex] * 2;
            int ny = current.y + dy[dirIndex] * 2;

            // 打通当前位置和邻居之间的墙
            int mx = current.x + dx[dirIndex];
            int my = current.y + dy[dirIndex];
            currentMap->setCell(mx, my, CellType::EMPTY);
            currentMap->setCell(nx, ny, CellType::EMPTY);

            // 小概率创建小型开放区域
            if ((randomEngine() % 100) < GameConfig::OPEN_AREA_PROBABILITY) {
                createSmallOpenArea(nx, ny);
            }

            // 将邻居加入栈
            stack.push_back(Position(nx, ny));
        } else {
            // 没有未访问的邻居，回溯
            stack.pop_back();
        }
    }
}

void RandomMapGenerator::createSmallOpenArea(int centerX, int centerY) {
    // 随机选择开放区域大小：2x2 或 3x3
    int size = (randomEngine() % 2 == 0) ? 2 : 3;

    // 计算开放区域的范围
    int halfSize = size / 2;

    for (int dy = -halfSize; dy <= halfSize; ++dy) {
        for (int dx = -halfSize; dx <= halfSize; ++dx) {
            int x = centerX + dx;
            int y = centerY + dy;

            // 确保在边界内
            if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
                currentMap->setCell(x, y, CellType::EMPTY);
            }
        }
    }
}

void RandomMapGenerator::placeDots() {
    int dotCount = 0;
    int targetDots = static_cast<int>((width - 2) * (height - 2) * dotRatio);

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            Position pos(x, y);
            if (currentMap->isEmpty(pos)) {
                // 按照dotRatio概率放置豆子
                if ((randomEngine() % 100) < (dotRatio * 100)) {
                    currentMap->setCell(pos, CellType::DOT);
                    dotCount++;
                }
            }
        }
    }

    // 确保至少有足够的豆子
    while (dotCount < std::min(targetDots, GameConfig::DOTS_TO_WIN)) {
        int x = 1 + (randomEngine() % (width - 2));
        int y = 1 + (randomEngine() % (height - 2));
        Position pos(x, y);

        if (currentMap->isEmpty(pos)) {
            currentMap->setCell(pos, CellType::DOT);
            dotCount++;
        }
    }
}

std::vector<Position> RandomMapGenerator::generateCharacterPositions(int characterCount) {
    std::vector<Position> positions;
    int attempts = 0;
    const int maxAttempts = 10000; // 增加尝试次数

    while (positions.size() < static_cast<size_t>(characterCount) && attempts < maxAttempts) {
        int x = 1 + (randomEngine() % (width - 2));
        int y = 1 + (randomEngine() % (height - 2));
        Position pos(x, y);

        if (isValidCharacterPosition(pos, positions)) {
            positions.push_back(pos);
        }

        attempts++;
    }

    return positions;
}

bool RandomMapGenerator::isValidCharacterPosition(const Position &pos,
                                                  const std::vector<Position> &existingPositions) const {
    // 检查是否是空地或有豆子的位置
    if (!currentMap->isEmpty(pos) && !currentMap->hasDot(pos)) {
        return false;
    }

    // 检查与其他角色的距离（降低最小距离要求）
    int minDistance = (existingPositions.size() < 2) ? GameConfig::MIN_DISTANCE_BETWEEN_CHARACTERS : 3;
    for (const auto &existing : existingPositions) {
        if (pos.manhattanDistance(existing) < minDistance) {
            return false;
        }
    }

    // 检查周围是否有足够的活动空间（降低要求）
    int emptyCount = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            Position neighbor(pos.x + dx, pos.y + dy);
            if (currentMap->isEmpty(neighbor) || currentMap->hasDot(neighbor)) {
                emptyCount++;
            }
        }
    }

    return emptyCount >= 3; // 降低到至少3个可移动的格子
}

bool RandomMapGenerator::isInBounds(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }
