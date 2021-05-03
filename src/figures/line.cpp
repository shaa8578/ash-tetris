#include "line.h"

#include <curses.h>

//------------------------------------------------------------------------------
tetris::Line::Line(tetris::FigureExt::Orientation orientation)
    : FigureExt(orientation) {}

//------------------------------------------------------------------------------
int tetris::Line::defaultWidth() const {
  return 4 * GLYPH_WIDTH;
}

//------------------------------------------------------------------------------
int tetris::Line::defaultHeight() const {
  return GLYPH_HEIGHT;
}

//------------------------------------------------------------------------------
void tetris::Line::draw(const Point& pivotPoint, char symbol) {
  const std::string line(currentSize().width, symbol);
  const int top = topRange(pivotPoint.row);
  for (int row_it(pivotPoint.row); row_it > top; --row_it) {
    mvprintw(row_it, pivotPoint.col, line.c_str());
  }
}
