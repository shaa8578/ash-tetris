#include "figure.h"

#include <curses.h>

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
