#include "../include/config.h"
#include "../include/game_map.h"
#include "../include/management_system.h"
#include "../include/monster_ai.h"
#include "../include/pacman_ai.h"
#include "../include/random_map_generator.h"
#include "../include/renderer.h"
#include "../include/turn_based_game_loop.h"
#include "../include/unicode_helper.h"
#include <algorithm>
#include <memory>
#include <random>
#include <windows.h>

// 全局变量
std::unique_ptr<TurnBasedGameLoop> gameLoop;
std::unique_ptr<Renderer> renderer;
HWND g_hwnd = nullptr;
bool gameRunning = true;
int frameCounter = 0;
const int FRAMES_PER_TURN = 30; // 每30帧执行一个回合（约0.5秒一回合）

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 初始化游戏
void InitializeGame(HWND hwnd);

// 游戏更新
void UpdateGame();

// 游戏渲染
void RenderGame();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 注册窗口类
    const wchar_t CLASS_NAME[] = L"PacmanGameWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassW(&wc);

    // 计算客户区大小（左右各增加20像素边距）
    int horizontalMargin = 40; // 左右各20像素
    int clientWidth = GameConfig::MAP_WIDTH * GameConfig::CELL_SIZE + horizontalMargin;
    int clientHeight = GameConfig::MAP_HEIGHT * GameConfig::CELL_SIZE + 60 + 50; // 60像素标题区域 + 50像素信息区域

    // 计算窗口大小（包含边框和标题栏）
    RECT rect = {0, 0, clientWidth, clientHeight};
    DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; // 不可拉伸的窗口样式
    AdjustWindowRect(&rect, windowStyle, FALSE);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    // 创建窗口
    g_hwnd = CreateWindowExW(0, CLASS_NAME, L"Collaborative Pac-Man", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                             windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (g_hwnd == nullptr) {
        return 0;
    }

    ShowWindow(g_hwnd, nCmdShow);

    // 初始化游戏
    InitializeGame(g_hwnd);

    // 设置定时器（60 FPS）
    SetTimer(g_hwnd, 1, 1000 / GameConfig::FRAME_RATE, nullptr);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;

    case WM_TIMER:
        if (wParam == 1 && gameRunning) {
            UpdateGame();
            RenderGame();
        }
        return 0;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_SPACE:
            // 暂停/恢复
            if (gameLoop) {
                auto &controlSystem = gameLoop->getControlSystem();
                if (controlSystem.getPaused()) {
                    // 如果当前是暂停状态，恢复游戏
                    controlSystem.resume();
                } else {
                    // 如果当前是运行状态，暂停游戏
                    controlSystem.pause();
                }
            }
            break;
        case VK_LEFT:
            // 左箭头：后退一回合
            if (gameLoop) {
                auto &controlSystem = gameLoop->getControlSystem();
                if (controlSystem.canStepBackward()) {
                    GameStateManager previousState = controlSystem.stepBackward();
                    gameLoop->setGameState(previousState);
                    // stepBackward已经设置了状态，不需要再调用pause
                }
            }
            break;
        case VK_RIGHT:
            // 右箭头：前进一回合
            if (gameLoop) {
                auto &controlSystem = gameLoop->getControlSystem();
                if (controlSystem.canStepForward()) {
                    GameStateManager nextState = controlSystem.stepForward();
                    gameLoop->setGameState(nextState);
                    // stepForward已经设置了状态，不需要再调用pause
                }
            }
            break;
        case 'R':
            // R键：从第一回合重新开始
            if (gameLoop) {
                auto &controlSystem = gameLoop->getControlSystem();
                if (controlSystem.hasHistory()) {
                    GameStateManager firstState = controlSystem.restartFromBeginning();
                    gameLoop->setGameState(firstState);
                    // 恢复游戏运行
                    controlSystem.resume();
                }
            }
            break;
        case VK_ESCAPE:
            // 退出游戏
            PostQuitMessage(0);
            break;
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void InitializeGame(HWND hwnd) {
    // 生成随机地图
    RandomMapGenerator mapGenerator(GameConfig::MAP_WIDTH, GameConfig::MAP_HEIGHT, GameConfig::DOT_RATIO);
    GameMap map = mapGenerator.generateMap();

    // 创建角色
    std::vector<Character> characters;

    // 手动查找空地位置来放置角色
    std::vector<Position> validPositions;
    for (int y = 1; y < map.getHeight() - 1; ++y) {
        for (int x = 1; x < map.getWidth() - 1; ++x) {
            Position pos(x, y);
            if (map.isEmpty(pos)) {
                validPositions.push_back(pos);
            }
        }
    }

    // 检查是否有足够的位置（1个吃豆人 + N个怪物）
    int totalCharacters = 1 + GameConfig::MONSTER_COUNT;
    if (validPositions.size() < static_cast<size_t>(totalCharacters)) {
        std::wstring errorMsg = L"Not enough empty spaces on map. Found: " + std::to_wstring(validPositions.size()) +
                                L", Required: " + std::to_wstring(totalCharacters);
        MessageBoxW(g_hwnd, errorMsg.c_str(), L"Initialization Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(1);
        return;
    }

    // 随机选择位置
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(validPositions.begin(), validPositions.end(), gen);

    int posIndex = 0;

    // 创建吃豆人
    Character pacman;
    pacman.type = CharacterType::PACMAN;
    pacman.position = validPositions[posIndex++];
    pacman.id = 0;
    characters.push_back(pacman);

    // 创建怪物
    for (int i = 0; i < GameConfig::MONSTER_COUNT; ++i) {
        Character monster;
        monster.type = CharacterType::MONSTER;
        monster.position = validPositions[posIndex++];
        monster.id = 1 + i;
        characters.push_back(monster);
    }

    // 创建游戏循环
    gameLoop = std::make_unique<TurnBasedGameLoop>(map, characters);

    // 设置AI代理
    gameLoop->setAIAgent(0, std::make_unique<PacmanAI>());  // 吃豆人
    for (int i = 0; i < GameConfig::MONSTER_COUNT; ++i) {
        gameLoop->setAIAgent(1 + i, std::make_unique<MonsterAI>());  // 怪物
    }

    // 设置管理系统
    gameLoop->setManagementSystem(std::make_unique<ManagementSystem>());

    // 启动游戏
    gameLoop->start();

    // 记录初始状态（第0回合）
    gameLoop->getControlSystem().recordState(gameLoop->getGameState());

    // 创建渲染器
    int horizontalMargin = 40; // 左右各20像素
    int windowWidth = GameConfig::MAP_WIDTH * GameConfig::CELL_SIZE + horizontalMargin;
    int windowHeight = GameConfig::MAP_HEIGHT * GameConfig::CELL_SIZE + 60 + 50; // 60像素标题区域 + 50像素信息区域
    renderer = std::make_unique<Renderer>(hwnd, windowWidth, windowHeight);
}

void UpdateGame() {
    if (gameLoop && gameRunning) {
        // 增加帧计数器
        frameCounter++;

        // 每FRAMES_PER_TURN帧执行一个回合
        if (frameCounter >= FRAMES_PER_TURN) {
            frameCounter = 0;

            // 执行一个回合
            bool continueGame = gameLoop->executeTurn();

            if (!continueGame) {
                // 游戏结束
                gameRunning = false;

                // 显示游戏结束消息
                const auto &gameState = gameLoop->getGameState();
                std::string message;

                if (gameState.getRemainingDots() == 0) {
                    message = "You Win!";
                } else {
                    message = "Game Over!";
                }

                std::wstring wMessage = Utf8ToWide(message);
                MessageBoxW(g_hwnd, wMessage.c_str(), L"Game Over", MB_OK);
                PostQuitMessage(0);
            }
        }
    }
}

void RenderGame() {
    if (renderer && gameLoop) {
        renderer->render(gameLoop->getGameState(), gameLoop->getControlSystem());
        renderer->present();
    }
}
