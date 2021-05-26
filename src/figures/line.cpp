#include "line.h"

#include <curses.h>

//------------------------------------------------------------------------------
tetris::Line::Line(tetris::FigureExt::Orientation orientation)
    : FigureExt(orientation) {}

//------------------------------------------------------------------------------
int tetris::Line::width() const {
  return currentSize().width;
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::Line::collisionMask(
    const tetris::Point& pivotPoint) const {
  auto result_orientation =
      (pivotPoint.rotating) ? nextOrientation() : orientation();
  std::vector<size_t> result;
  switch (result_orientation) {
    case tetris::FigureExt::HORIZONTAL:
    case tetris::FigureExt::HORIZONTAL_INVERT:
      result = collisionMask(pivotPoint, defaultWidth(), defaultHeight());
      break;
    case tetris::FigureExt::VERTICAL:
    case tetris::FigureExt::VERTICAL_INVERT:
      result = collisionMask(pivotPoint, defaultHeight(), defaultWidth());
      break;
    default:
      break;
  }
  return result;
}

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

//------------------------------------------------------------------------------
std::vector<size_t> tetris::Line::collisionMask(const tetris::Point& pivotPoint,
                                                int orientationWidth,
                                                int orientationHeight) const {
  int mask_size = (pivotPoint.row - orientationHeight < 0) ? pivotPoint.row
                                                           : orientationHeight;
  if (mask_size == 0) ++mask_size;

  size_t mask = size_t((1 << orientationWidth) - 1) << pivotPoint.col;
  std::vector<size_t> result(mask_size, mask);
  return result;
}
