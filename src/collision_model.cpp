#include "collision_model.h"

#include <functional>
#include <iterator>

#include "figure.h"

//------------------------------------------------------------------------------
CollisionModel::CollisionModel(int width, int height) {
  const size_t mask = ~((size_t(1) << width) - 1);
  m_field = std::vector<size_t>(height, mask);
}

//------------------------------------------------------------------------------
bool CollisionModel::isCollision(const tetris::Point& pivotPoint,
                                 std::vector<size_t> figureMask) {
  if (pivotPoint.col < 1) return true;
  if (figureMask.empty()) return false;
  if (size_t(pivotPoint.row) >= m_field.size()) return true;
  if (pivotPoint.row < 0) return false;

  const auto rows_count(figureMask.size());
  auto last = m_field.begin() + size_t(pivotPoint.row);
  auto first = last - rows_count;
  std::vector<size_t> test_rows(first, last);

  for (size_t it(0U); it < rows_count; ++it) {
    auto test = test_rows[it] & figureMask[it];
    if (test > 0) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void CollisionModel::appendMask(const tetris::Point& pivotPoint,
                                std::vector<size_t> figureMask) {
  const auto rows_count(figureMask.size());
  auto begin = pivotPoint.row - static_cast<int>(rows_count);

  for (int row(begin); row < pivotPoint.row; ++row) {
    m_field[row] |= figureMask[row - begin];
  }
}
