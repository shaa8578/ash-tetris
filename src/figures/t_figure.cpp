#include "t_figure.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
tetris::TFigure::TFigure(Orientation orientation) : FigureExt(orientation) {}

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
      throwInvalidOrientation();
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
