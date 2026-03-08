#include "../../include/pacman_ai.h"

#include <ctime>

#include <climits>     

  

// 构造函数：使用系统时间初始化随机数引擎

PacmanAI::PacmanAI() {

          randomEngine.seed(static_cast<unsigned int>(std::time(nullptr)));

}

  

// 带种子的构造函数：支持固定种子（方便调试）

PacmanAI::PacmanAI(unsigned int seed) {

          randomEngine.seed(seed);

}

  

// 核心AI决策方法：实现基类的抽象接口，随机选择有效移动方向

Action PacmanAI::getAction(const VisibleArea &visibleArea) {

          // 获取所有不会撞墙的有效移动方向

          std::vector<Direction> validMoves = getValidMoves(visibleArea);

  

          // 随机选择一个有效方向（核心逻辑）

          if (!validMoves.empty()) {

                    // ===== 新增：过滤回头方向（不走回头路）=====

                    std::vector<Direction> filteredMoves = filterBackwardMoves(validMoves);

                    // 若过滤后无有效方向（如被墙堵死），则恢复原有效方向

                    if (filteredMoves.empty()) {

                              filteredMoves = validMoves;

                    }

  

                    // 1. 定位吃豆人当前位置（可视区域中心）

                    int pacmanX = visibleArea.getWidth() / 2;

                    int pacmanY = visibleArea.getHeight() / 2;

  

                    // 2. 优先向最近的食物移动（贪心策略）

                    Direction foodDir = getNearestFoodDirection(visibleArea, pacmanX, pacmanY, filteredMoves);

                    if (foodDir != Direction::STAY) {

                              lastDirection = foodDir; // 记录本次移动方向，用于下次防回头

                              return Action{foodDir}; // 找到食物则向食物移动

                    }

  

                    // 3. 兜底：无食物/无法向食物移动时，执行原有随机逻辑（从过滤后的方向选）

                    int index = randomEngine() % filteredMoves.size();

                    lastDirection = filteredMoves[index]; // 记录本次移动方向

                    return Action{filteredMoves[index]};

          }

  

          // 无有效移动时返回「停留」动作

          lastDirection = Direction::STAY; // 重置上一步方向

          return Action{Direction::STAY};

}

  

// ===== 新增核心方法：过滤回头方向（不走回头路）=====

std::vector<Direction> PacmanAI::filterBackwardMoves(const std::vector<Direction> &validMoves) const {

          std::vector<Direction> filtered;

          // 遍历所有有效方向，排除与上一步相反的方向

          for (Direction dir : validMoves) {

                    // 核心逻辑：判断当前方向是否是上一步的反方向

                    bool isBackward = false;

                    if (lastDirection == Direction::UP && dir == Direction::DOWN) {

                              isBackward = true;

                    } else if (lastDirection == Direction::DOWN && dir == Direction::UP) {

                              isBackward = true;

                    } else if (lastDirection == Direction::LEFT && dir == Direction::RIGHT) {

                              isBackward = true;

                    } else if (lastDirection == Direction::RIGHT && dir == Direction::LEFT) {

                              isBackward = true;

                    }

                    // 非回头方向才保留

                    if (!isBackward) {

                              filtered.push_back(dir);

                    }

          }

          return filtered;

}

  

// 辅助方法：筛选所有合法的移动方向（避开墙体）

std::vector<Direction> PacmanAI::getValidMoves(const VisibleArea &visibleArea) const {

          std::vector<Direction> validMoves;

  

          // 遍历上下左右四个基础方向

          Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

          for (Direction dir : directions) {

                    if (canMove(visibleArea, dir)) {

                              validMoves.push_back(dir);

                    }

          }

  

          // 若所有方向都不可移动，添加「停留」作为保底

          if (validMoves.empty()) {

                    validMoves.push_back(Direction::STAY);

          }

  

          return validMoves;

}

  

// 辅助方法：校验单个方向是否可移动（边界+墙体检查）

bool PacmanAI::canMove(const VisibleArea &visibleArea, Direction dir) const {

          // 计算吃豆人当前位置（可视区域的中心）

          int centerX = visibleArea.getWidth() / 2;

          int centerY = visibleArea.getHeight() / 2;

  

          // 获取方向对应的坐标偏移量

          int dx, dy;

          getDirectionOffset(dir, dx, dy);

  

          // 计算移动后的新坐标

          int newX = centerX + dx;

          int newY = centerY + dy;

  

          // 第一步：检查新坐标是否在可视区域边界内

          if (newX < 0 || newX >= visibleArea.getWidth() || newY < 0 || newY >= visibleArea.getHeight()) {

                    return false;

          }

  

          // 第二步：检查新坐标是否是可通行区域（空/食物，非墙体）

          return visibleArea.getCell(newX, newY) == VisibleArea::CellContent::EMPTY ||

                             visibleArea.getCell(newX, newY) == VisibleArea::CellContent::DOT;

}

  

// 辅助方法：将方向枚举转换为坐标偏移量（核心映射逻辑）

void PacmanAI::getDirectionOffset(Direction dir, int &dx, int &dy) const {

          dx = 0;

          dy = 0;

  

          switch (dir) {

                    case Direction::UP:          dy = -1; break;     // 向上：y轴减1

                    case Direction::DOWN:     dy = 1;     break;     // 向下：y轴加1

                    case Direction::LEFT:     dx = -1; break;     // 向左：x轴减1

                    case Direction::RIGHT: dx = 1;     break;     // 向右：x轴加1

                    case Direction::STAY:     break;                          // 停留：无偏移

          }

}

  

// 辅助方法：找到最近食物的移动方向（贪心策略）

Direction PacmanAI::getNearestFoodDirection(const VisibleArea &visibleArea, int pacmanX, int pacmanY, const std::vector<Direction> &validMoves) const {

          int nearestFoodX = -1, nearestFoodY = -1;

          int minFoodDist = INT_MAX;

  

          // 遍历可视区域，查找最近的食物（DOT）

          for (int x = 0; x < visibleArea.getWidth(); ++x) {

                    for (int y = 0; y < visibleArea.getHeight(); ++y) {

                              if (visibleArea.getCell(x, y) == VisibleArea::CellContent::DOT) {

                                        // 计算曼哈顿距离（极简距离计算）

                                        int dist = abs(x - pacmanX) + abs(y - pacmanY);

                                        if (dist < minFoodDist) {

                                                  minFoodDist = dist;

                                                  nearestFoodX = x;

                                                  nearestFoodY = y;

                                        }

                              }

                    }

          }

  

          // 无食物则返回STAY

          if (nearestFoodX == -1 || nearestFoodY == -1) {

                    return Direction::STAY;

          }

  

          // 计算向食物移动的优先方向（先x轴后y轴，极简贪心）

          Direction targetDir = Direction::STAY;

          if (pacmanX < nearestFoodX) {

                    targetDir = Direction::RIGHT; // 食物在右边，向右

          } else if (pacmanX > nearestFoodX) {

                    targetDir = Direction::LEFT;     // 食物在左边，向左

          } else if (pacmanY < nearestFoodY) {

                    targetDir = Direction::DOWN;     // 食物在下边，向下

          } else if (pacmanY > nearestFoodY) {

                    targetDir = Direction::UP;          // 食物在上边，向上

          }

  

          // 检查目标方向是否有效（避免撞墙）

          for (Direction dir : validMoves) {

                    if (dir == targetDir) {

                              return targetDir;

                    }

          }

  

          // 目标方向无效则选次优方向（仅x/y轴）

          if (pacmanX != nearestFoodX) {

                    // 优先x轴，若目标x方向无效则选y轴

                    if (pacmanY < nearestFoodY && canMove(visibleArea, Direction::DOWN)) {

                              return Direction::DOWN;

                    } else if (pacmanY > nearestFoodY && canMove(visibleArea, Direction::UP)) {

                              return Direction::UP;

                    }

          } else if (pacmanY != nearestFoodY) {

                    // 优先y轴，若目标y方向无效则选x轴

                    if (pacmanX < nearestFoodX && canMove(visibleArea, Direction::RIGHT)) {

                              return Direction::RIGHT;

                    } else if (pacmanX > nearestFoodX && canMove(visibleArea, Direction::LEFT)) {

                              return Direction::LEFT;

                    }

          }

  

          return Direction::STAY;

}