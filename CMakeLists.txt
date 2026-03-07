cmake_minimum_required(VERSION 3.15)
project(PacmanGame VERSION 1.0)

# MSVC编译器配置 - 解决Unicode编码问题
if(MSVC)
    add_compile_options(/W4)
    add_compile_options(/utf-8)  # 源文件和执行字符集都使用UTF-8
    add_compile_definitions(_UNICODE UNICODE)  # 使用Unicode版本的Windows API
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 收集源文件
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE AGENT_SOURCES "src/agents/*.cpp")
file(GLOB_RECURSE MANAGEMENT_SOURCES "src/management/*.cpp")
file(GLOB_RECURSE RENDER_SOURCES "src/render/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.cpp")

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${AGENT_SOURCES}
    ${MANAGEMENT_SOURCES}
    ${RENDER_SOURCES}
    ${UTILS_SOURCES}
)

# 声明可执行文件
add_executable(pacman_game WIN32)

# 增加源码
target_sources(pacman_game
    PRIVATE
        src/main.cpp
        ${ALL_SOURCES}
)

# 设置cpp标准
target_compile_features(pacman_game PRIVATE cxx_std_17)

# 链接Windows库
target_link_libraries(pacman_game
    PRIVATE
        gdi32
        gdiplus
        user32
        kernel32
)

# 输出信息
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")