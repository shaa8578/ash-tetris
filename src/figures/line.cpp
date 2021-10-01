#include "line.h"

#include <curses.h>

//------------------------------------------------------------------------------
tetris::Line::Line(tetris::FigureExt::Orientation orientation)
    : FigureExt(orientation) {}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::Line::collisionMask(int row, int col,
                                                bool rotating) const {
  auto result_orientation = (rotating) ? nextOrientation() : orientation();
  std::vector<size_t> result;
  switch (result_orientation) {
    case tetris::FigureExt::HORIZONTAL:
    case tetris::FigureExt::HORIZONTAL_INVERT:
      result = collisionMask(row, col, defaultWidth(), defaultHeight());
      break;
    case tetris::FigureExt::VERTICAL:
    case tetris::FigureExt::VERTICAL_INVERT:
      result = collisionMask(row, col, defaultHeight(), defaultWidth());
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
std::vector<size_t> tetris::Line::collisionMask(int row, int col,
                                                int orientationWidth,
                                                int orientationHeight) const {
  int mask_size = (row - orientationHeight < 0) ? row : orientationHeight;
  if (mask_size == 0) ++mask_size;

  size_t mask = ((size_t(1) << orientationWidth) - 1) << (col - 1);
  std::vector<size_t> result(mask_size, mask);
  return result;
}
