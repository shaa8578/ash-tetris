#include "t_figure.h"

#include <curses.h>

#include <iterator>
#include <string>

//------------------------------------------------------------------------------
tetris::TFigure::TFigure(Orientation orientation) : FigureExt(orientation) {}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::TFigure::collisionMask(int row, int col,
                                                   bool rotating) const {
  auto figure_orientation = (rotating) ? nextOrientation() : orientation();
  std::vector<size_t> mask;
  switch (figure_orientation) {
    case tetris::FigureExt::HORIZONTAL:
      mask = collisionMaskHorizontal(col);
      break;
    case tetris::FigureExt::HORIZONTAL_INVERT:
      mask = collisionMaskHorizontalInvert(col);
      break;
    case tetris::FigureExt::VERTICAL:
      mask = collisionMaskVertical(col);
      break;
    case tetris::FigureExt::VERTICAL_INVERT:
      mask = collisionMaskVerticalInvert(col);
      break;
    default:
      throwInvalidOrientation(figure_orientation);
  }

  Size figure_size = figureSize(figure_orientation);
  int mask_shift =
      ((row - figure_size.height) < 0) ? (figure_size.height - row) : 0;

  auto copy_mask_begin(mask.begin());
  std::advance(copy_mask_begin, mask_shift);

  auto copy_mask_end(mask.end());

  std::vector<size_t> result(copy_mask_begin, copy_mask_end);
  return result;
}

//------------------------------------------------------------------------------
int tetris::TFigure::defaultWidth() const {
  return 3 * GLYPH_WIDTH;
}

//------------------------------------------------------------------------------
int tetris::TFigure::defaultHeight() const {
  return 2 * GLYPH_HEIGHT;
}

//------------------------------------------------------------------------------
void tetris::TFigure::draw(const tetris::Point& pivotPoint, const char symbol) {
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
void tetris::TFigure::drawHorizontal(const tetris::Point& pivotPoint,
                                     const char symbol) {
  const std::string big_line(currentSize().width, symbol);
  const std::string short_line(GLYPH_WIDTH, symbol);

  const auto& row(pivotPoint.row);
  const auto& col(pivotPoint.col);
  const auto col_shift = col + GLYPH_WIDTH;

  if (orientation() == HORIZONTAL) {
    mvprintw(row - 0, col, big_line.c_str());
    mvprintw(row - 1, col, big_line.c_str());
    mvprintw(row - 2, col_shift, short_line.c_str());
    mvprintw(row - 3, col_shift, short_line.c_str());
  } else {
    mvprintw(row - 0, col_shift, short_line.c_str());
    mvprintw(row - 1, col_shift, short_line.c_str());
    mvprintw(row - 2, col, big_line.c_str());
    mvprintw(row - 3, col, big_line.c_str());
  }
}

//------------------------------------------------------------------------------
void tetris::TFigure::drawVertical(const tetris::Point& pivotPoint,
                                   const char symbol) {
  const std::string short_line(GLYPH_WIDTH, symbol);
  const auto& row(pivotPoint.row);
  const auto& col(pivotPoint.col);
  const auto top(topRange(row));
  const auto col_shift = col + GLYPH_WIDTH;
  const auto row_shift(row - GLYPH_HEIGHT);

  if (orientation() == VERTICAL) {
    for (int row_it(row); row_it > top; --row_it)
      mvprintw(row_it, col, short_line.c_str());
    mvprintw(row_shift - 0, col_shift, short_line.c_str());
    mvprintw(row_shift - 1, col_shift, short_line.c_str());
  } else {
    for (int row_it(row); row_it > top; --row_it)
      mvprintw(row_it, col_shift, short_line.c_str());
    mvprintw(row_shift - 0, col, short_line.c_str());
    mvprintw(row_shift - 1, col, short_line.c_str());
  }
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::TFigure::collisionMaskHorizontal(int col) const {
  std::vector<size_t> result;
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  result.push_back(((size_t(1) << defaultWidth()) - 1) << (col - 1));
  result.push_back(((size_t(1) << defaultWidth()) - 1) << (col - 1));
  return result;
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::TFigure::collisionMaskHorizontalInvert(
    int col) const {
  std::vector<size_t> result;
  result.push_back(((size_t(1) << defaultWidth()) - 1) << (col - 1));
  result.push_back(((size_t(1) << defaultWidth()) - 1) << (col - 1));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  return result;
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::TFigure::collisionMaskVertical(int col) const {
  std::vector<size_t> result;
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1) << (col - 1));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1) << (col - 1));
  result.push_back(((size_t(1) << defaultHeight()) - 1) << (col - 1));
  result.push_back(((size_t(1) << defaultHeight()) - 1) << (col - 1));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1) << (col - 1));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1) << (col - 1));
  return result;
}

//------------------------------------------------------------------------------
std::vector<size_t> tetris::TFigure::collisionMaskVerticalInvert(
    int col) const {
  std::vector<size_t> result;
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  result.push_back(((size_t(1) << defaultHeight()) - 1) << (col - 1));
  result.push_back(((size_t(1) << defaultHeight()) - 1) << (col - 1));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  result.push_back(((size_t(1) << Figure::GLYPH_WIDTH) - 1)
                   << (col - 1 + Figure::GLYPH_WIDTH));
  return result;
}

//------------------------------------------------------------------------------
tetris::Size tetris::TFigure::figureSize(
    tetris::FigureExt::Orientation figureOrientation) const {
  Size result;
  switch (figureOrientation) {
    case tetris::FigureExt::HORIZONTAL:
    case tetris::FigureExt::HORIZONTAL_INVERT:
      result.width = defaultWidth();
      result.height = defaultHeight();
      break;
    case tetris::FigureExt::VERTICAL:
    case tetris::FigureExt::VERTICAL_INVERT:
      result.width = defaultHeight();
      result.height = defaultWidth();
      break;
    default:
      result.width = 0;
      result.height = 0;
      break;
  }
  return result;
}
