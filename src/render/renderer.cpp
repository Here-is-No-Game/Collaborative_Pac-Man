#include "../../include/renderer.h"
#include "../../include/config.h"
#include "../../include/game_control_system.h"
#include "../../include/unicode_helper.h"

Renderer::Renderer(HWND window, int width, int height)
    : hwnd(window), hdcMem(nullptr), hbmMem(nullptr), hbmOld(nullptr), windowWidth(width), windowHeight(height) {

    // 从配置文件加载颜色
    colorWall = GameConfig::COLOR_WALL;
    colorEmpty = GameConfig::COLOR_EMPTY;
    colorDot = GameConfig::COLOR_DOT;
    colorPacman = GameConfig::COLOR_PACMAN;
    colorMonster = GameConfig::COLOR_MONSTER;

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

void Renderer::render(const GameStateManager &gameState, const GameControlSystem &controlSystem) {
    clear();

    // 渲染标题
    renderTitle();

    // 渲染地图
    renderMap(gameState.getMap());

    // 渲染角色
    renderCharacters(gameState.getCharacters());

    // 渲染信息
    renderInfo(gameState, controlSystem);
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
    int titleHeight = 60; // 标题区域高度（增加到60）
    int horizontalMargin = 20; // 左边距

    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            Position pos(x, y);

            if (map.isWall(pos)) {
                renderCell(x, y, colorWall, titleHeight, horizontalMargin);
            } else if (map.hasDot(pos)) {
                // 渲染白色小圆点
                renderDot(x, y, titleHeight, horizontalMargin);
            }
        }
    }
}

void Renderer::renderCharacters(const std::vector<Character> &characters) {
    int titleHeight = 60; // 标题区域高度（增加到60）
    int horizontalMargin = 20; // 左边距
    for (const auto &character : characters) {
        COLORREF color = (character.type == CharacterType::PACMAN) ? colorPacman : colorMonster;
        renderCell(character.position.x, character.position.y, color, titleHeight, horizontalMargin);
    }
}

void Renderer::renderCell(int x, int y, COLORREF color, int offsetY, int offsetX) {
    int cellSize = GameConfig::CELL_SIZE;
    int margin = GameConfig::CELL_MARGIN;

    RECT rect;
    rect.left = x * cellSize + margin + offsetX;
    rect.top = y * cellSize + margin + offsetY;
    rect.right = (x + 1) * cellSize - margin + offsetX;
    rect.bottom = (y + 1) * cellSize - margin + offsetY;

    HBRUSH hBrush = CreateSolidBrush(color);
    FillRect(hdcMem, &rect, hBrush);
    DeleteObject(hBrush);
}

void Renderer::renderDot(int x, int y, int offsetY, int offsetX) {
    int cellSize = GameConfig::CELL_SIZE;
    int centerX = x * cellSize + cellSize / 2 + offsetX;
    int centerY = y * cellSize + cellSize / 2 + offsetY;
    int radius = GameConfig::DOT_RADIUS;

    HBRUSH hBrush = CreateSolidBrush(colorDot);
    HPEN hPen = CreatePen(PS_SOLID, 1, colorDot);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdcMem, hBrush);
    HPEN hOldPen = (HPEN)SelectObject(hdcMem, hPen);

    Ellipse(hdcMem, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

    SelectObject(hdcMem, hOldBrush);
    SelectObject(hdcMem, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void Renderer::renderInfo(const GameStateManager &gameState, const GameControlSystem &controlSystem) {
    int cellSize = GameConfig::CELL_SIZE;
    int mapHeight = gameState.getMap().getHeight();
    int mapWidth = gameState.getMap().getWidth();
    int titleHeight = 60; // 标题区域高度
    int horizontalMargin = 20; // 左边距

    // 设置文本颜色和背景
    SetTextColor(hdcMem, RGB(255, 255, 255));
    SetBkColor(hdcMem, RGB(0, 0, 0));

    // 在标题和地图之间显示 Dots 和 Turn（标题下方，地图上方）
    int topInfoY = 35; // 标题下方位置

    // 显示剩余豆子（左侧，与地图左边缘对齐）
    std::string dotsText = "Dots: " + std::to_string(gameState.getRemainingDots());
    std::wstring wDotsText = Utf8ToWide(dotsText);
    TextOutW(hdcMem, horizontalMargin, topInfoY, wDotsText.c_str(), static_cast<int>(wDotsText.length()));

    // 获取播放状态符号和颜色
    std::wstring statusSymbol;
    COLORREF statusColor;
    auto status = controlSystem.getPlaybackStatus();
    switch (status) {
        case GameControlSystem::PlaybackStatus::PLAYING:
            statusSymbol = L"=>"; // 播放
            statusColor = RGB(0, 255, 0); // 绿色
            break;
        case GameControlSystem::PlaybackStatus::PAUSED:
            statusSymbol = L"||"; // 暂停
            statusColor = RGB(255, 0, 0); // 红色
            break;
        case GameControlSystem::PlaybackStatus::STEPPED_BACKWARD:
            statusSymbol = L"<-"; // 后退
            statusColor = RGB(255, 255, 0); // 黄色
            break;
        case GameControlSystem::PlaybackStatus::STEPPED_FORWARD:
            statusSymbol = L"->"; // 前进
            statusColor = RGB(255, 255, 0); // 黄色
            break;
    }

    // 显示回合数（右侧，与地图右边缘对齐）
    std::string turnText = "Turn: " + std::to_string(gameState.getTurnCount());
    std::wstring wTurnText = Utf8ToWide(turnText);

    // 先显示状态符号（带颜色）
    SIZE statusSize;
    GetTextExtentPoint32W(hdcMem, statusSymbol.c_str(), static_cast<int>(statusSymbol.length()), &statusSize);

    // 计算Turn文本的宽度
    SIZE turnTextSize;
    GetTextExtentPoint32W(hdcMem, wTurnText.c_str(), static_cast<int>(wTurnText.length()), &turnTextSize);

    // 计算总宽度（状态符号 + 空格 + Turn文本）
    int totalWidth = statusSize.cx + 8 + turnTextSize.cx; // 8像素空格
    int turnX = horizontalMargin + mapWidth * cellSize - totalWidth;

    // 显示状态符号（带颜色）
    SetTextColor(hdcMem, statusColor);
    TextOutW(hdcMem, turnX, topInfoY, statusSymbol.c_str(), static_cast<int>(statusSymbol.length()));

    // 显示Turn文本（白色）
    SetTextColor(hdcMem, RGB(255, 255, 255));
    TextOutW(hdcMem, turnX + statusSize.cx + 8, topInfoY, wTurnText.c_str(), static_cast<int>(wTurnText.length()));

    // 在地图下方显示两个阵营分数
    int bottomInfoY = titleHeight + mapHeight * cellSize + 10;

    // 显示吃豆人分数（左侧，与地图左边缘对齐）
    std::string pacmanScoreText = "Pacman: " + std::to_string(gameState.getPacmanScore());
    std::wstring wPacmanScoreText = Utf8ToWide(pacmanScoreText);
    TextOutW(hdcMem, horizontalMargin, bottomInfoY, wPacmanScoreText.c_str(), static_cast<int>(wPacmanScoreText.length()));

    // 显示怪物分数（右侧，与地图右边缘对齐）
    std::string monsterScoreText = "Monster: " + std::to_string(gameState.getMonsterScore());
    std::wstring wMonsterScoreText = Utf8ToWide(monsterScoreText);

    // 计算文本宽度以右对齐
    SIZE monsterTextSize;
    GetTextExtentPoint32W(hdcMem, wMonsterScoreText.c_str(), static_cast<int>(wMonsterScoreText.length()), &monsterTextSize);
    int monsterX = horizontalMargin + mapWidth * cellSize - monsterTextSize.cx;
    TextOutW(hdcMem, monsterX, bottomInfoY, wMonsterScoreText.c_str(), static_cast<int>(wMonsterScoreText.length()));
}

void Renderer::renderTitle() {
    // 设置更鲜艳的海蓝色 (RGB: 0, 150, 200)
    SetTextColor(hdcMem, RGB(0, 150, 200));
    SetBkColor(hdcMem, RGB(0, 0, 0));

    // 创建更大的字体
    HFONT hFont = CreateFontW(
        24,                        // 字体高度（更大）
        0,                         // 字体宽度
        0,                         // 文本倾斜角度
        0,                         // 基线倾斜角度
        FW_BOLD,                   // 字体粗细
        FALSE,                     // 斜体
        FALSE,                     // 下划线
        FALSE,                     // 删除线
        DEFAULT_CHARSET,           // 字符集
        OUT_DEFAULT_PRECIS,        // 输出精度
        CLIP_DEFAULT_PRECIS,       // 裁剪精度
        DEFAULT_QUALITY,           // 输出质量
        DEFAULT_PITCH | FF_SWISS,  // 字体间距和字体族
        L"Arial"                   // 字体名称
    );

    HFONT hOldFont = (HFONT)SelectObject(hdcMem, hFont);

    // 显示标题（居中）
    std::wstring title = L"HING Project-PacmanAI";

    // 获取文本尺寸以计算居中位置
    SIZE textSize;
    GetTextExtentPoint32W(hdcMem, title.c_str(), static_cast<int>(title.length()), &textSize);

    int centerX = (windowWidth - textSize.cx) / 2;
    int centerY = 8; // 标题上方留更多空间

    TextOutW(hdcMem, centerX, centerY, title.c_str(), static_cast<int>(title.length()));

    // 恢复原字体
    SelectObject(hdcMem, hOldFont);
    DeleteObject(hFont);
}
