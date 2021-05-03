#include "line.h"

#include <curses.h>

//------------------------------------------------------------------------------
// clang-format off
const tetris::Size tetris::Line::HORIZONTAL_SIZE =
    {4 * GLYPH_WIDTH, GLYPH_HEIGHT};
const tetris::Size tetris::Line::VERTICAL_SIZE =
    {GLYPH_WIDTH, 4 * GLYPH_HEIGHT};
// clang-format on

//------------------------------------------------------------------------------
tetris::Line::Line()
    : Figure(),
      m_orientation(VERTICAL),
      m_currentSize(&tetris::Line::VERTICAL_SIZE) {}

//------------------------------------------------------------------------------
int tetris::Line::rangeRight(int currentCol) const {
  return currentCol + m_currentSize->width;
}

//------------------------------------------------------------------------------
int tetris::Line::rangeRightRotated(int currentCol) const {
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
void tetris::Line::draw(const Point& pivotPoint, char symbol) {
  const std::string line(m_currentSize->width, symbol);
  const int top = topRange(pivotPoint.row);
  for (int row_it(pivotPoint.row); row_it > top; --row_it) {
    mvprintw(row_it, pivotPoint.col, line.c_str());
  }
}

//------------------------------------------------------------------------------
void tetris::Line::rotateGeometry() {
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
int tetris::Line::topRange(int row) {
  auto result = row - m_currentSize->height;

  /** Верхняя граница отрисовки окна */
  static const int TOP_RANGE = -1;
  if (result < TOP_RANGE) result = TOP_RANGE;

  return result;
}
