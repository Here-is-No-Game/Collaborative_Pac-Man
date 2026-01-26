# 渲染系统技术手册

## 概述

本文档描述了协作式吃豆人游戏项目的渲染系统设计。系统基于Windows GDI，采用像素格为单位的渲染方式，支持不同单位的自定义素材，并为未来的精度升级预留了空间。系统使用双重缓冲技术防止画面闪烁，并采用PNG格式来高效管理和导入素材。

## 窗口布局

游戏窗口被分为三个主要区域：

1. 标题区域：位于窗口顶部，显示 "HING Project-PacMan"。
2. 游戏地图区域：位于窗口中央，显示15x15的游戏地图。
3. 状态区域：位于窗口底部，显示豆子数量、回合数和游戏状态。

## 渲染系统设计

### 1. 像素格渲染

- 游戏世界被划分为15x15的网格
- 每个网格单元被渲染为一个正方形像素格
- 像素格大小可调，默认设置为32x32像素

### 2. 双重缓冲

为防止画面闪烁，实现双重缓冲：

```cpp
class Renderer {
private:
    HWND hwnd;
    HDC hdcWindow;
    HDC hdcBuffer;
    HBITMAP hbmBuffer;
    int width, height;

public:
    Renderer(HWND hwnd, int width, int height);
    ~Renderer();

    void beginDraw();
    void endDraw();
    HDC getBufferDC() const { return hdcBuffer; }
};

void Renderer::beginDraw() {
    // 清除缓冲区
    HBRUSH hbrBkGnd = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = {0, 0, width, height};
    FillRect(hdcBuffer, &rect, hbrBkGnd);
    DeleteObject(hbrBkGnd);
}

void Renderer::endDraw() {
    // 将缓冲区内容复制到窗口
    BitBlt(hdcWindow, 0, 0, width, height, hdcBuffer, 0, 0, SRCCOPY);
}
```

### 3. 素材设计和管理

[保持原有内容不变]

### 4. 地图数据结构

[保持原有内容不变]

### 5. 渲染函数

更新渲染函数以包含新的布局元素：

```cpp
void renderGame(Renderer& renderer, const GameMap& map, SpriteManager& spriteManager,
                int cellSize, const GameState& gameState) {
    renderer.beginDraw();
    HDC hdc = renderer.getBufferDC();

    // 渲染标题
    RECT titleRect = {0, 0, renderer.getWidth(), 30};
    DrawText(hdc, L"HING Project-PacMan", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 渲染游戏地图
    int mapOffsetY = 40;  // 标题下方的偏移
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            CellType cellType = map.getCell(x, y);
            spriteManager.renderSprite(hdc, x * cellSize, y * cellSize + mapOffsetY, cellSize, cellType);
        }
    }

    // 渲染状态信息
    int statusY = mapOffsetY + map.getHeight() * cellSize + 10;
    RECT statusRect = {0, statusY, renderer.getWidth(), renderer.getHeight()};
    std::wstring statusText = L"豆子数量: " + std::to_wstring(gameState.getRemainingDots()) +
                              L" | 回合数: " + std::to_wstring(gameState.getRoundCount()) +
                              L" | 状态: " + gameState.getStatusString();
    DrawText(hdc, statusText.c_str(), -1, &statusRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    renderer.endDraw();
}
```

## 素材导入系统

[保持原有内容不变]

## 精度升级方案

[保持原有内容不变]

## 地图编辑

[保持原有内容不变]

## 后续优化方向

1. 实现sprite动画系统
2. 添加简单的粒子效果（如吃豆人吃豆时的效果）
3. 实现平滑的相机移动（如果未来需要更大的地图）
4. 优化渲染性能，如使用脏矩形技术只更新变化的部分
5. 添加简单的光影效果，增强游戏视觉效果
6. 优化文本渲染，使用更美观的字体和样式
7. 添加简单的UI动画，如分数变化时的效果