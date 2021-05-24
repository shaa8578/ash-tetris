#pragma once

#include <stddef.h>

#include <vector>

//------------------------------------------------------------------------------
namespace tetris {
struct Point;
} /* namespace tetris */

//------------------------------------------------------------------------------
/** Модель коллизий */
class CollisionModel {
 public:
  CollisionModel(int width, int height);

  bool isCollision(const tetris::Point& pivotPoint,
                   std::vector<size_t> figureMask);

 private:
  std::vector<size_t> m_field;
};
