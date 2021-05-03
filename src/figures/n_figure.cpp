#include "n_figure.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
tetris::NFigure::NFigure(tetris::FigureExt::Orientation orientation)
    : FigureExt(orientation) {}

//------------------------------------------------------------------------------
int tetris::NFigure::defaultWidth() const {
  return 3 * GLYPH_WIDTH;
}

//------------------------------------------------------------------------------
int tetris::NFigure::defaultHeight() const {
  return 2 * GLYPH_HEIGHT;
}

//------------------------------------------------------------------------------
void tetris::NFigure::draw(const tetris::Point& pivotPoint, const char symbol) {
  switch (orientation()) {
    case HORIZONTAL:
    case HORIZONTAL_INVERT:
      drawHorizontal(pivotPoint, symbol);
      break;
    case VERTICAL:
    case VERTICAL_INVERT:
      drawVertical(pivotPoint, symbol);
      break;
    default:
      throwInvalidOrientation();
      break;
  }
}

//------------------------------------------------------------------------------
void tetris::NFigure::drawHorizontal(const tetris::Point& pivotPoint,
                                     const char symbol) {
  const std::string line(currentSize().height, symbol);

  auto& row(pivotPoint.row);
  auto& col(pivotPoint.col);
  auto col_shift = col + GLYPH_WIDTH;
  mvprintw(row, col_shift, line.c_str());
  mvprintw(row - 1, col_shift, line.c_str());
  mvprintw(row - 2, col, line.c_str());
  mvprintw(row - 3, col, line.c_str());
}

//------------------------------------------------------------------------------
void tetris::NFigure::drawVertical(const tetris::Point& pivotPoint,
                                   const char symbol) {
  const std::string line(currentSize().width / 2, symbol);
  auto& row(pivotPoint.row);
  auto& col(pivotPoint.col);
  auto row_shift(row - currentSize().width);
  for (int row_it(row); row_it > row_shift; --row_it)
    mvprintw(row_it, col, line.c_str());

  auto top(topRange(row));
  auto col_shift = col + GLYPH_WIDTH;
  for (int row_it(row - GLYPH_WIDTH); row_it > top; --row_it)
    mvprintw(row_it, col_shift, line.c_str());
}
