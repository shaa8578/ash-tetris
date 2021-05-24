#include "collision_model.h"

#include <functional>
#include <iterator>

#include "figure.h"

//------------------------------------------------------------------------------
CollisionModel::CollisionModel(int width, int height) : m_field(height, 0UL) {
  const size_t mask = ~((size_t(1) << width) - 1);
  for (size_t col(height); col > 0; --col) {
    m_field[col - 1] = mask;
  }
}

//------------------------------------------------------------------------------
bool CollisionModel::isCollision(const tetris::Point& pivotPoint,
                                 std::vector<size_t> figureMask) {
  if (pivotPoint.col < 1) return true;
  if (figureMask.empty()) return false;
  if (size_t(pivotPoint.row) >= m_field.size()) return true;
  if (pivotPoint.row < 1) return false;

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
