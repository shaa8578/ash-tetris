#include "collision_model.h"

#include <cstring>
#include <functional>
#include <iterator>

#include "figure.h"

//------------------------------------------------------------------------------
CollisionModel::CollisionModel(int width, int height)
    : m_defaultMask(~((size_t(1) << width) - 1)),
      m_field(height, m_defaultMask) {}

//------------------------------------------------------------------------------
bool CollisionModel::isCollision(const tetris::Point& pivotPoint,
                                 std::vector<size_t> figureMask) {
  if (pivotPoint.col < 1) return true;
  if (figureMask.empty()) return false;
  if (size_t(pivotPoint.row) >= m_field.size()) return true;
  if (pivotPoint.row < 0) return false;

  const auto rows_count(figureMask.size());
  auto last = m_field.begin() + size_t(pivotPoint.row) + 1;
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
    m_field[row + 1] |= figureMask[row - begin];
  }
}

//------------------------------------------------------------------------------
int CollisionModel::removeFullRows(int begin, int end) {
  int removed_rows_count(0);

  for (; begin < end; ++begin) {
    if ((m_field[begin] ^ ~0) == 0) {
      removeRow(begin);
      ++removed_rows_count;
    }
  }

  return removed_rows_count;
}

//------------------------------------------------------------------------------
size_t CollisionModel::value(int row) {
  return m_field[row];
}

//------------------------------------------------------------------------------
void CollisionModel::removeRow(int rowNo) {
  std::memmove(static_cast<void*>(&m_field[1]), static_cast<void*>(&m_field[0]),
               sizeof(size_t) * rowNo);
  m_field[0] = m_defaultMask;
}
