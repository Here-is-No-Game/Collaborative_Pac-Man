#pragma once

#include "game_state_manager.h"
#include <windows.h>
#include <string>

// 简单的GDI渲染器 - 使用双缓冲技术
class Renderer {
private:
    HWND hwnd;
    HDC hdcMem;           // 内存设备上下文（双缓冲）
    HBITMAP hbmMem;       // 内存位图
    HBITMAP hbmOld;       // 旧位图
    int windowWidth;
    int windowHeight;

    // 颜色定义
    COLORREF colorWall;
    COLORREF colorEmpty;
    COLORREF colorDot;
    COLORREF colorPacman;
    COLORREF colorMonster;

public:
    Renderer(HWND window, int width, int height);
    ~Renderer();

    // 渲染游戏状态
    void render(const GameStateManager& gameState);

    // 清空屏幕
    void clear();

    // 显示缓冲区内容
    void present();

    // 调整大小
    void resize(int width, int height);

private:
    // 初始化双缓冲
    void initDoubleBuffer();

    // 清理双缓冲
    void cleanupDoubleBuffer();

    // 渲染地图
    void renderMap(const GameMap& map);

    // 渲染角色
    void renderCharacters(const std::vector<Character>& characters);

    // 渲染单个单元格
    void renderCell(int x, int y, COLORREF color);

    // 渲染文本信息
    void renderInfo(const GameStateManager& gameState);
};
