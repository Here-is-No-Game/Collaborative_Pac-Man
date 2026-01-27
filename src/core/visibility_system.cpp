#include "../../include/visibility_system.h"
#include <cmath>

VisibilitySystem::VisibilitySystem(int radius)
    : visibilityRadius(radius) {
}

VisibleArea VisibilitySystem::calculateVisibleArea(const Position& center, const GameMap& map, const std::vector<Character>& characters) const {
    int size = 2 * visibilityRadius + 1;
    VisibleArea visibleArea(size, size);

    // 遍历可见区域范围内的所有位置
    for (int dy = -visibilityRadius; dy <= visibilityRadius; ++dy) {
        for (int dx = -visibilityRadius; dx <= visibilityRadius; ++dx) {
            Position targetPos(center.x + dx, center.y + dy);

            // 检查曼哈顿距离
            if (manhattanDistance(center, targetPos) > visibilityRadius) {
                visibleArea.setCell(dx + visibilityRadius, dy + visibilityRadius,
                                   VisibleArea::CellContent::WALL);
                continue;
            }

            // 检查是否在地图范围内
            if (!map.isInBounds(targetPos)) {
                visibleArea.setCell(dx + visibilityRadius, dy + visibilityRadius,
                                   VisibleArea::CellContent::WALL);
                continue;
            }

            // 检查视线是否被阻挡
            if (!isVisible(center, targetPos, map)) {
                visibleArea.setCell(dx + visibilityRadius, dy + visibilityRadius,
                                   VisibleArea::CellContent::WALL);
                continue;
            }

            // 获取该位置的内容
            VisibleArea::CellContent content = getCellContent(targetPos, map, characters);
            visibleArea.setCell(dx + visibilityRadius, dy + visibilityRadius, content);
        }
    }

    return visibleArea;
}

bool VisibilitySystem::isVisible(const Position& from, const Position& to, const GameMap& map) const {
    // Bresenham's line 算法
    int x0 = from.x, y0 = from.y;
    int x1 = to.x, y1 = to.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0, y = y0;

    while (true) {
        // 如果到达目标点，视线畅通
        if (x == x1 && y == y1) {
            return true;
        }

        // 检查当前点是否是墙（但不检查起点和终点）
        if (!(x == x0 && y == y0) && !(x == x1 && y == y1)) {
            Position currentPos(x, y);
            if (map.isWall(currentPos)) {
                return false;  // 被墙阻挡
            }
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

int VisibilitySystem::manhattanDistance(const Position& a, const Position& b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

VisibleArea::CellContent VisibilitySystem::getCellContent(const Position& pos, const GameMap& map, const std::vector<Character>& characters) const {
    // 首先检查是否有角色在这个位置
    for (const auto& character : characters) {
        if (character.position == pos && character.isAlive) {
            if (character.type == CharacterType::PACMAN) {
                return VisibleArea::CellContent::PACMAN;
            } else {
                return VisibleArea::CellContent::MONSTER;
            }
        }
    }

    // 然后检查地图单元格类型
    CellType cellType = map.getCell(pos);
    switch (cellType) {
        case CellType::EMPTY:
            return VisibleArea::CellContent::EMPTY;
        case CellType::WALL:
            return VisibleArea::CellContent::WALL;
        case CellType::DOT:
            return VisibleArea::CellContent::DOT;
        default:
            return VisibleArea::CellContent::EMPTY;
    }
}
