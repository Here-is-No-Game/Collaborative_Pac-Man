#include "../../include/game_map.h"
#include <fstream>
#include <sstream>

GameMap::GameMap() : width(GameConfig::MAP_WIDTH), height(GameConfig::MAP_HEIGHT), totalDots(0) { initialize(); }

GameMap::GameMap(int w, int h) : width(w), height(h), totalDots(0) { initialize(); }

void GameMap::initialize() {
    grid.resize(height);
    for (int y = 0; y < height; ++y) {
        grid[y].resize(width, CellType::WALL);
    }
}

void GameMap::clear() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = CellType::WALL;
        }
    }
    totalDots = 0;
}

CellType GameMap::getCell(int x, int y) const {
    if (!isInBounds(x, y)) {
        return CellType::WALL;
    }
    return grid[y][x];
}

CellType GameMap::getCell(const Position &pos) const { return getCell(pos.x, pos.y); }

void GameMap::setCell(int x, int y, CellType type) {
    if (isInBounds(x, y)) {
        grid[y][x] = type;
    }
}

void GameMap::setCell(const Position &pos, CellType type) { setCell(pos.x, pos.y, type); }

bool GameMap::isInBounds(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }

bool GameMap::isInBounds(const Position &pos) const { return isInBounds(pos.x, pos.y); }

bool GameMap::isWall(const Position &pos) const { return getCell(pos) == CellType::WALL; }

bool GameMap::isEmpty(const Position &pos) const { return getCell(pos) == CellType::EMPTY; }

bool GameMap::hasDot(const Position &pos) const { return getCell(pos) == CellType::DOT; }

int GameMap::countDots() const {
    int count = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == CellType::DOT) {
                count++;
            }
        }
    }
    return count;
}

int GameMap::countEmptyCells() const {
    int count = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == CellType::EMPTY) {
                count++;
            }
        }
    }
    return count;
}

bool GameMap::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return loadFromString(buffer.str());
}

bool GameMap::saveToFile(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char c;
            switch (grid[y][x]) {
            case CellType::EMPTY:
                c = ' ';
                break;
            case CellType::WALL:
                c = '#';
                break;
            case CellType::DOT:
                c = '.';
                break;
            default:
                c = '?';
                break;
            }
            file << c;
        }
        file << '\n';
    }

    return true;
}

bool GameMap::loadFromString(const std::string &mapData) {
    std::istringstream stream(mapData);
    std::string line;

    // 读取宽度和高度
    if (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        lineStream >> width >> height;
        initialize();
    }

    int y = 0;
    while (std::getline(stream, line) && y < height) {
        for (int x = 0; x < width && x < (int)line.length(); ++x) {
            char c = line[x];
            CellType type;
            switch (c) {
            case ' ':
                type = CellType::EMPTY;
                break;
            case '#':
                type = CellType::WALL;
                break;
            case '.':
                type = CellType::DOT;
                break;
            default:
                type = CellType::WALL;
                break;
            }
            grid[y][x] = type;
        }
        y++;
    }

    totalDots = countDots();
    return true;
}

bool GameMap::validate() const {
    // 检查地图尺寸
    if (width <= 0 || height <= 0) {
        return false;
    }

    // 检查边界是否全是墙
    for (int x = 0; x < width; ++x) {
        if (grid[0][x] != CellType::WALL || grid[height - 1][x] != CellType::WALL) {
            return false;
        }
    }
    for (int y = 0; y < height; ++y) {
        if (grid[y][0] != CellType::WALL || grid[y][width - 1] != CellType::WALL) {
            return false;
        }
    }

    // 检查是否有足够的空地
    int emptyCount = countEmptyCells();
    if (emptyCount < GameConfig::PACMAN_COUNT + GameConfig::MONSTER_COUNT) {
        return false;
    }

    return true;
}

GameMap GameMap::clone() const {
    GameMap newMap(width, height);
    newMap.grid = this->grid;
    newMap.totalDots = this->totalDots;
    return newMap;
}

std::string GameMap::saveToString() const {
    std::stringstream ss;

    // 保存宽度和高度
    ss << width << " " << height << "\n";

    // 保存地图数据
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char c;
            switch (grid[y][x]) {
            case CellType::EMPTY:
                c = ' ';
                break;
            case CellType::WALL:
                c = '#';
                break;
            case CellType::DOT:
                c = '.';
                break;
            default:
                c = '?';
                break;
            }
            ss << c;
        }
        ss << '\n';
    }

    return ss.str();
}
