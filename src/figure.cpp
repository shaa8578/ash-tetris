#include "figure.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
void tetris::Figure::draw(const tetris::Point& pivotPoint) {
  if (pivotPoint.row < 0) return;
  draw(pivotPoint, FILL_SYMBOL);
}

//------------------------------------------------------------------------------
void tetris::Figure::clear(const tetris::Point& pivotPoint) {
  if (pivotPoint.row < 0) return;
  draw(pivotPoint, EMPTY_SYMBOL);
}

//------------------------------------------------------------------------------
void tetris::Figure::rotate(const tetris::Point& pivotPoint) {
  if (pivotPoint.row < 0) return;
  draw(pivotPoint, EMPTY_SYMBOL);
  rotateGeometry();
  draw(pivotPoint, FILL_SYMBOL);
}

//------------------------------------------------------------------------------
void tetris::Figure::draw(const tetris::Point& pivotPoint, char symbol) {
  const std::string line(currentSize().width, symbol);
  const int top = topRange(pivotPoint.row);
  for (int row_it(pivotPoint.row); row_it > top; --row_it) {
    mvprintw(row_it, pivotPoint.col, line.c_str());
  }
}
