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
  void appendMask(const tetris::Point& pivotPoint,
                  std::vector<size_t> figureMask);
  int removeFullRows(int begin, int end);
  size_t value(int row);

 protected:
  void removeRow(int rowNo);

 private:
  size_t m_defaultMask;
  std::vector<size_t> m_field;
};
