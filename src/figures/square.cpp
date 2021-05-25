#include "square.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
const tetris::Size tetris::Square::SQUARE_SIZE = {2 * GLYPH_WIDTH,
                                                  2 * GLYPH_HEIGHT};

//------------------------------------------------------------------------------
tetris::Square::Square() : Figure() {}

//------------------------------------------------------------------------------
int tetris::Square::width() const {
  return SQUARE_SIZE.width;
}

//------------------------------------------------------------------------------
int tetris::Square::rangeRight(int currentCol) const {
  return currentCol + SQUARE_SIZE.width;
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::Square::collisionMask(
    const tetris::Point& pivotPoint) const {
  auto current_size = SQUARE_SIZE.height;
  auto row = pivotPoint.row - SQUARE_SIZE.height;
  if (row < 0) {
    current_size += row;
  }
  if (current_size < 1) {
    return std::vector<size_t>();
  }
  row = 0;
  std::vector<size_t> result(current_size, 0UL);
  result.reserve(current_size);
  for (; row < current_size; ++row) {
    result[row] = ((size_t(SQUARE_SIZE.width) << 2) - 1) << pivotPoint.col;
  }
  return result;
}

//------------------------------------------------------------------------------
void tetris::Square::draw(const tetris::Point& pivotPoint, char symbol) {
  const std::string line(SQUARE_SIZE.width, symbol);
  const int top = topRange(pivotPoint.row);
  for (int row_it(pivotPoint.row); row_it > top; --row_it) {
    mvprintw(row_it, pivotPoint.col, line.c_str());
  }
}

//------------------------------------------------------------------------------
int tetris::Square::topRange(int row) {
  auto result = row - SQUARE_SIZE.height;

  /** Верхняя граница отрисовки окна */
  static const int TOP_RANGE = -1;
  if (result < TOP_RANGE) result = TOP_RANGE;

  return result;
}
