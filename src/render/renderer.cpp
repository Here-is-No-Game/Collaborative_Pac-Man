#include "../../include/renderer.h"
#include "../../include/config.h"
#include "../../include/unicode_helper.h"

Renderer::Renderer(HWND window, int width, int height)
    : hwnd(window), hdcMem(nullptr), hbmMem(nullptr), hbmOld(nullptr), windowWidth(width), windowHeight(height) {

    // 定义颜色
    colorWall = RGB(0, 100, 0);     // 暗绿色墙壁
    colorEmpty = RGB(0, 0, 0);      // 黑色背景
    colorDot = RGB(255, 255, 0);    // 黄色豆子
    colorPacman = RGB(255, 165, 0); // 橙色吃豆人
    colorMonster = RGB(255, 0, 0);  // 红色怪物

    initDoubleBuffer();
}

Renderer::~Renderer() { cleanupDoubleBuffer(); }

void Renderer::initDoubleBuffer() {
    HDC hdc = GetDC(hwnd);

    // 创建内存设备上下文
    hdcMem = CreateCompatibleDC(hdc);

    // 创建内存位图
    hbmMem = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);

    // 选择位图到内存DC
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

    ReleaseDC(hwnd, hdc);
}

void Renderer::cleanupDoubleBuffer() {
    if (hdcMem) {
        if (hbmOld) {
            SelectObject(hdcMem, hbmOld);
        }
        if (hbmMem) {
            DeleteObject(hbmMem);
        }
        DeleteDC(hdcMem);
    }
}

void Renderer::clear() {
    // 用黑色填充背景
    RECT rect = {0, 0, windowWidth, windowHeight};
    HBRUSH hBrush = CreateSolidBrush(colorEmpty);
    FillRect(hdcMem, &rect, hBrush);
    DeleteObject(hBrush);
}

void Renderer::render(const GameStateManager &gameState) {
    clear();

    // 渲染地图
    renderMap(gameState.getMap());

    // 渲染角色
    renderCharacters(gameState.getCharacters());

    // 渲染信息
    renderInfo(gameState);
}

void Renderer::present() {
    // 将内存DC的内容复制到窗口DC
    HDC hdc = GetDC(hwnd);
    BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcMem, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
}

void Renderer::resize(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    cleanupDoubleBuffer();
    initDoubleBuffer();
}

void Renderer::renderMap(const GameMap &map) {
    int cellSize = GameConfig::CELL_SIZE;

    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            Position pos(x, y);
            COLORREF color = colorEmpty;

            if (map.isWall(pos)) {
                color = colorWall;
            } else if (map.hasDot(pos)) {
                color = colorDot;
            }

            if (color != colorEmpty) {
                renderCell(x, y, color);
            }
        }
    }
}

void Renderer::renderCharacters(const std::vector<Character> &characters) {
    for (const auto &character : characters) {
        COLORREF color = (character.type == CharacterType::PACMAN) ? colorPacman : colorMonster;
        renderCell(character.position.x, character.position.y, color);
    }
}

void Renderer::renderCell(int x, int y, COLORREF color) {
    int cellSize = GameConfig::CELL_SIZE;
    int margin = 2; // 单元格之间的间距

    RECT rect;
    rect.left = x * cellSize + margin;
    rect.top = y * cellSize + margin;
    rect.right = (x + 1) * cellSize - margin;
    rect.bottom = (y + 1) * cellSize - margin;

    HBRUSH hBrush = CreateSolidBrush(color);
    FillRect(hdcMem, &rect, hBrush);
    DeleteObject(hBrush);
}

void Renderer::renderInfo(const GameStateManager &gameState) {
    int cellSize = GameConfig::CELL_SIZE;
    int mapHeight = gameState.getMap().getHeight();
    int infoY = mapHeight * cellSize + 10;

    // 设置文本颜色和背景
    SetTextColor(hdcMem, RGB(255, 255, 255));
    SetBkColor(hdcMem, RGB(0, 0, 0));

    // 显示分数
    std::string scoreText = "Score: " + std::to_string(gameState.getScore());
    std::wstring wScoreText = Utf8ToWide(scoreText);
    TextOutW(hdcMem, 10, infoY, wScoreText.c_str(), static_cast<int>(wScoreText.length()));

    // 显示剩余豆子
    std::string dotsText = "Dots: " + std::to_string(gameState.getRemainingDots());
    std::wstring wDotsText = Utf8ToWide(dotsText);
    TextOutW(hdcMem, 150, infoY, wDotsText.c_str(), static_cast<int>(wDotsText.length()));

    // 显示回合数
    std::string turnText = "Turn: " + std::to_string(gameState.getTurnCount());
    std::wstring wTurnText = Utf8ToWide(turnText);
    TextOutW(hdcMem, 280, infoY, wTurnText.c_str(), static_cast<int>(wTurnText.length()));
}
