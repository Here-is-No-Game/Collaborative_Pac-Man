# 项目结构说明 (ProjectTree.md)

## 📁 项目目录结构

```
Collaborative_Pac-Man/
├── .vscode/                    # VS Code 编辑器配置文件夹（可忽略）
├── .claude/                    # Claude Code 工具配置文件夹（可忽略）
├── build/                      # 编译生成的文件夹（CMake自动创建）
│   ├── pacman_game.exe        # 游戏可执行文件
│   └── ...                    # 其他编译中间文件
│
├── include/                    # 头文件文件夹（.h 文件）
│   ├── config.h               # 游戏配置（地图大小、角色数量等）
│   ├── game_types.h           # 基础数据类型（Position、Direction等）
│   ├── unicode_helper.h       # Unicode编码转换工具
│   ├── game_map.h             # 游戏地图类
│   ├── game_state_manager.h  # 游戏状态管理器
│   ├── visible_area.h         # 可见区域类
│   ├── visibility_system.h    # 可见性计算系统
│   ├── random_map_generator.h # 随机地图生成器
│   ├── game_control_system.h  # 游戏控制系统（暂停/保存等）
│   ├── turn_based_game_loop.h # 回合制游戏循环
│   ├── ai_interface.h         # AI接口（抽象类）
│   ├── pacman_ai.h            # 吃豆人AI
│   ├── monster_ai.h           # 怪物AI
│   ├── management_interface.h # 管理系统接口（抽象类）
│   ├── management_system.h    # 管理系统实现
│   └── renderer.h             # 渲染器
│
├── src/                        # 源代码文件夹（.cpp 文件）
│   ├── main.cpp               # 程序入口（创建窗口、初始化游戏）
│   │
│   ├── core/                  # 核心系统源文件
│   │   ├── game_map.cpp
│   │   ├── game_state_manager.cpp
│   │   ├── visible_area.cpp
│   │   ├── visibility_system.cpp
│   │   ├── random_map_generator.cpp
│   │   ├── game_control_system.cpp
│   │   └── turn_based_game_loop.cpp
│   │
│   ├── agents/                # AI代理源文件
│   │   ├── ai_interface.cpp
│   │   ├── pacman_ai.cpp     # 【学生A修改】
│   │   └── monster_ai.cpp    # 【学生B修改】
│   │
│   ├── management/            # 管理系统源文件
│   │   ├── management_interface.cpp
│   │   └── management_system.cpp  # 【学生C修改】
│   │
│   └── render/                # 渲染系统源文件
│       └── renderer.cpp
│
├── docs/                       # 文档文件夹
│   ├── core_system.md         # 核心系统详细说明
│   ├── agent.md               # AI系统详细说明
│   ├── management_system.md   # 管理系统详细说明
│   ├── rendering_system.md    # 渲染系统详细说明
│   └── 项目需求和架构.md      # 完整项目需求（中文）
│
├── tests/                      # 测试文件夹（暂时为空）
│
├── TODO/                       # 详细的TODO文档（23个文件）
│   ├── README.md              # TODO总览
│   ├── UNICODE_GUIDE.md       # Unicode编码完整指南
│   └── ...                    # 其他TODO文档
│
├── CMakeLists.txt             # CMake构建配置文件
├── README.md                  # 项目说明文档
├── CLAUDE.md                  # Claude Code 使用指南
├── PROGRESS.md                # 项目进度报告
├── COMPLETION_SUMMARY.md      # 项目完成总结
├── STUDENT_GUIDE.md           # 【新】学生技术手册
├── ProjectTree.md             # 【本文件】项目结构说明
└── 项目需求和架构.md          # 项目需求文档（中文）
```

## 📝 文件用途详解

### 🎯 学生需要关注的文件

#### 学生A - 吃豆人AI开发者
**主要修改文件**:
- `src/agents/pacman_ai.cpp` - 实现吃豆人的移动策略
- `include/pacman_ai.h` - 吃豆人AI类定义

**参考文档**:
- `STUDENT_GUIDE.md` - 学生技术手册
- `docs/agent.md` - AI系统详细说明

#### 学生B - 怪物AI开发者
**主要修改文件**:
- `src/agents/monster_ai.cpp` - 实现怪物的移动策略
- `include/monster_ai.h` - 怪物AI类定义

**参考文档**:
- `STUDENT_GUIDE.md` - 学生技术手册
- `docs/agent.md` - AI系统详细说明

#### 学生C - 管理系统开发者
**主要修改文件**:
- `src/management/management_system.cpp` - 实现游戏规则判定
- `include/management_system.h` - 管理系统类定义

**参考文档**:
- `STUDENT_GUIDE.md` - 学生技术手册
- `docs/management_system.md` - 管理系统详细说明

### 📚 重要文档说明

| 文档名称 | 用途 | 适合人群 |
|---------|------|---------|
| `STUDENT_GUIDE.md` | 学生技术手册（通俗易懂） | 所有学生 |
| `README.md` | 项目快速入门 | 所有人 |
| `ProjectTree.md` | 项目结构说明（本文件） | 所有人 |
| `PROGRESS.md` | 项目进度报告 | 教师/助教 |
| `COMPLETION_SUMMARY.md` | 项目完成总结 | 教师/助教 |
| `CLAUDE.md` | Claude Code 使用指南 | 开发者 |
| `docs/` 文件夹 | 详细技术文档 | 需要深入了解的学生 |
| `TODO/` 文件夹 | 开发任务清单 | 开发者 |

### 🔧 配置和构建文件

| 文件名 | 用途 | 是否需要修改 |
|--------|------|-------------|
| `CMakeLists.txt` | CMake构建配置 | ❌ 不需要 |
| `.vscode/` | VS Code编辑器配置 | ❌ 不需要 |
| `.claude/` | Claude Code配置 | ❌ 不需要 |
| `build/` | 编译输出目录 | ❌ 自动生成 |

### 🎮 如何使用

1. **编译项目**:
   ```bash
   cmake -B build -G Ninja
   cmake --build build
   ```

2. **运行游戏**:
   ```bash
   build\pacman_game.exe
   ```

3. **修改代码**:
   - 学生只需要修改各自负责的 `.cpp` 文件
   - 修改后重新编译即可

4. **查看文档**:
   - 先看 `STUDENT_GUIDE.md` 了解基础知识
   - 再看 `docs/` 文件夹中的详细文档

## 🚫 不要修改的文件

以下文件是框架代码，学生**不应该修改**：

- `include/config.h` - 游戏配置（除非需要调整参数）
- `src/main.cpp` - 主程序入口
- `src/core/` 文件夹中的所有文件 - 核心系统
- `src/render/` 文件夹中的所有文件 - 渲染系统
- `include/` 中除了 AI 和管理系统以外的头文件

## 📦 文件夹用途总结

| 文件夹 | 用途 | 学生是否需要关注 |
|--------|------|-----------------|
| `include/` | 头文件（类定义） | ⚠️ 只关注自己负责的 |
| `src/` | 源代码（类实现） | ⚠️ 只关注自己负责的 |
| `docs/` | 技术文档 | ✅ 需要阅读 |
| `build/` | 编译输出 | ❌ 自动生成 |
| `tests/` | 测试代码 | ❌ 暂时为空 |
| `TODO/` | 开发任务 | ⚠️ 可选阅读 |
| `.vscode/` | 编辑器配置 | ❌ 不需要 |
| `.claude/` | 工具配置 | ❌ 不需要 |

## 💡 提示

- 所有源文件必须保存为 **UTF-8 编码**
- 修改代码后需要重新编译才能看到效果
- 遇到问题先查看 `STUDENT_GUIDE.md`
- 不确定的地方可以参考 `docs/` 文件夹中的详细文档
