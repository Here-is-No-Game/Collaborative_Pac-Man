#include "../../include/visible_area.h"

VisibleArea::VisibleArea(int w, int h) : width(w), height(h), centerPosition(w / 2, h / 2) {
    grid.resize(height);
    for (int y = 0; y < height; ++y) {
        grid[y].resize(width, CellContent::EMPTY);
    }
}

VisibleArea::CellContent VisibleArea::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return CellContent::WALL;
}

void VisibleArea::setCell(int x, int y, CellContent content) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = content;
    }
}
