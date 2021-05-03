#include "n_figure.h"

#include <curses.h>

#include <string>

//------------------------------------------------------------------------------
// clang-format off
const tetris::Size tetris::NFigure::HORIZONTAL_SIZE =
    {3 * GLYPH_WIDTH, 2 * GLYPH_HEIGHT};
const tetris::Size tetris::NFigure::VERTICAL_SIZE =
    {2 * GLYPH_WIDTH, 3 * GLYPH_HEIGHT};
// clang-format on

//------------------------------------------------------------------------------
tetris::NFigure::NFigure()
    : Figure(),
      m_orientation(HORIZONTAL),
      m_currentSize(&tetris::NFigure::HORIZONTAL_SIZE) {}

//------------------------------------------------------------------------------
int tetris::NFigure::rangeRight(int currentCol) const {
  return currentCol + m_currentSize->width;
}

//------------------------------------------------------------------------------
int tetris::NFigure::rangeRightRotated(int currentCol) const {
  int rotated_width(0);
  switch (m_orientation) {
    case HORIZONTAL:
      rotated_width = VERTICAL_SIZE.width;
      break;
    case VERTICAL:
      rotated_width = HORIZONTAL_SIZE.width;
      break;
  }
  return currentCol + rotated_width;
}

//------------------------------------------------------------------------------
void tetris::NFigure::rotateGeometry() {
  switch (m_orientation) {
    case HORIZONTAL:
      m_orientation = VERTICAL;
      m_currentSize = &VERTICAL_SIZE;
      break;
    case VERTICAL:
      m_orientation = HORIZONTAL;
      m_currentSize = &HORIZONTAL_SIZE;
      break;
  }
}

//------------------------------------------------------------------------------
void tetris::NFigure::draw(const tetris::Point& pivotPoint, const char symbol) {
  switch (m_orientation) {
    case HORIZONTAL:
      drawHorizontal(pivotPoint, symbol);
      break;
    case VERTICAL:
      drawVertical(pivotPoint, symbol);
      break;
  }
}

//------------------------------------------------------------------------------
void tetris::NFigure::drawHorizontal(const tetris::Point& pivotPoint,
                                     const char symbol) {
  const std::string line(m_currentSize->height, symbol);

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
  const std::string line(m_currentSize->width / 2, symbol);
  auto& row(pivotPoint.row);
  auto& col(pivotPoint.col);
  auto row_shift(row - m_currentSize->width);
  for (int row_it(row); row_it > row_shift; --row_it)
    mvprintw(row_it, col, line.c_str());

  auto top(topRange(row));
  auto col_shift = col + GLYPH_WIDTH;
  for (int row_it(row - GLYPH_WIDTH); row_it > top; --row_it)
    mvprintw(row_it, col_shift, line.c_str());
}

//------------------------------------------------------------------------------
int tetris::NFigure::topRange(int row) {
  auto result = row - m_currentSize->height;

  /** Верхняя граница отрисовки окна */
  static const int TOP_RANGE = -1;
  if (result < TOP_RANGE) result = TOP_RANGE;

  return result;
}
