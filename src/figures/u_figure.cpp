#include "u_figure.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
tetris::UFigure::UFigure() : NFigure() {}

//------------------------------------------------------------------------------
void tetris::UFigure::drawHorizontal(const tetris::Point& pivotPoint,
                                     const char symbol) {
  const std::string line(currentSize().height, symbol);

  auto& row(pivotPoint.row);
  auto& col(pivotPoint.col);
  auto col_shift = col + GLYPH_WIDTH;
  mvprintw(row, col, line.c_str());
  mvprintw(row - 1, col, line.c_str());
  mvprintw(row - 2, col_shift, line.c_str());
  mvprintw(row - 3, col_shift, line.c_str());
}

//------------------------------------------------------------------------------
void tetris::UFigure::drawVertical(const tetris::Point& pivotPoint,
                                   const char symbol) {
  const std::string line(currentSize().width / 2, symbol);
  auto& row(pivotPoint.row);
  auto& col(pivotPoint.col);
  auto row_shift(row - currentSize().width);
  auto col_shift = col + GLYPH_WIDTH;
  for (int row_it(row); row_it > row_shift; --row_it)
    mvprintw(row_it, col_shift, line.c_str());

  auto top(topRange(row));
  for (int row_it(row - GLYPH_WIDTH); row_it > top; --row_it)
    mvprintw(row_it, col, line.c_str());
}
