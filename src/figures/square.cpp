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
std::vector<size_t> tetris::Square::collisionMask(int row, int col,
                                                  bool) const {
  auto current_size = SQUARE_SIZE.height;
  auto first_row = row - SQUARE_SIZE.height;
  if (first_row < 0) {
    current_size += first_row;
  }
  if (current_size < 1) {
    return std::vector<size_t>();
  }
  first_row = 0;
  std::vector<size_t> result(current_size, 0UL);
  result.reserve(current_size);
  for (; first_row < current_size; ++first_row) {
    result[first_row] = ((size_t(1) << SQUARE_SIZE.width) - 1) << (col - 1);
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
