#include "n_figure.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
tetris::NFigure::NFigure(tetris::FigureExt::Orientation orientation)
    : FigureExt(orientation) {}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::NFigure::collisionMask(int row, int col,
                                                   bool rotating) const {
  auto result_orientation = (rotating) ? nextOrientation() : orientation();
  std::vector<size_t> result;
  switch (result_orientation) {
    case tetris::FigureExt::HORIZONTAL:
    case tetris::FigureExt::HORIZONTAL_INVERT:
      result = collisionMaskHorizontal(row, col);
      break;
    case tetris::FigureExt::VERTICAL:
    case tetris::FigureExt::VERTICAL_INVERT:
      result = collisionMaskVertical(row, col);
      break;
    default:
      break;
  }
  return result;
}

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
      throwInvalidOrientation(orientation());
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

//------------------------------------------------------------------------------
std::vector<size_t> tetris::NFigure::collisionMaskHorizontal(int row,
                                                             int col) const {
  static const int ORIENTATION_HEIGHT = 2 * Figure::GLYPH_HEIGHT;
  static const int ORIENTATION_WIDTH = 2 * Figure::GLYPH_WIDTH;
  int mask_size = (row - ORIENTATION_HEIGHT < 0) ? row : ORIENTATION_HEIGHT;
  if (mask_size == 0) ++mask_size;

  std::vector<size_t> result;
  switch (mask_size) {
    case 4:
      result.push_back(((size_t(1) << ORIENTATION_WIDTH) - 1) << (col - 1));
      /* FALLTHRU */
    case 3:
      result.push_back(((size_t(1) << ORIENTATION_WIDTH) - 1) << (col - 1));
      /* FALLTHRU */
    case 2:
      result.push_back(((size_t(1) << ORIENTATION_WIDTH) - 1)
                       << (col - 1 + Figure::GLYPH_WIDTH));
      /* FALLTHRU */
    case 1:
      result.push_back(((size_t(1) << ORIENTATION_WIDTH) - 1)
                       << (col - 1 + Figure::GLYPH_WIDTH));
      break;
    default:
      break;
  }
  return result;
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::NFigure::collisionMaskVertical(int row,
                                                           int col) const {
  static const int ORIENTATION_HEIGHT = 3 * Figure::GLYPH_HEIGHT;
  int mask_size = (row - ORIENTATION_HEIGHT < 0) ? row : ORIENTATION_HEIGHT;
  if (mask_size == 0) ++mask_size;

  std::vector<size_t> result;
  switch (mask_size) {
    case 6:
      result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                       << (col - 1 + Figure::GLYPH_WIDTH));
      /* FALLTHRU */
    case 5:
      result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                       << (col - 1 + Figure::GLYPH_WIDTH));
      /* FALLTHRU */
    case 4:
      result.push_back(((size_t(1) << (2 * Figure::GLYPH_WIDTH)) - 1)
                       << (col - 1));
      /* FALLTHRU */
    case 3:
      result.push_back(((size_t(1) << (2 * Figure::GLYPH_WIDTH)) - 1)
                       << (col - 1));
      /* FALLTHRU */
    case 2:
      result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1) << (col - 1));
      /* FALLTHRU */
    case 1:
      result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1) << (col - 1));
      break;
    default:
      break;
  }
  return result;
}
