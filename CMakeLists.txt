cmake_minimum_required(VERSION 3.15)
project(PacmanGame VERSION 1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# 主程序可执行文件
add_executable(pacman_game WIN32 src/main.cpp ${ALL_SOURCES})

# 链接Windows库
target_link_libraries(pacman_game PRIVATE
    gdi32
    gdiplus
    user32
    kernel32
)

# 测试配置
enable_testing()

# 尝试查找Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # 如果找不到GTest，使用FetchContent下载
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# 收集测试文件
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

if(TEST_SOURCES)
    # 创建测试可执行文件
    add_executable(run_tests ${TEST_SOURCES} ${ALL_SOURCES})

    # 链接GTest和Windows库
    target_link_libraries(run_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        gdi32
        gdiplus
        user32
        kernel32
    )

    # 添加测试
    include(GoogleTest)
    gtest_discover_tests(run_tests)
endif()

# 输出信息
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
