#include "line.h"

#include <curses.h>

//------------------------------------------------------------------------------
const tetris::Size tetris::Line::HORIZONTAL_SIZE = {8, 2};
const tetris::Size tetris::Line::VERTICAL_SIZE = {2, 8};

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
void tetris::Line::draw(const tetris::Point& pivotPoint) {
  if (pivotPoint.row < 0) return;
  drawLine(pivotPoint, FILL_SYMBOL);
  refresh();
}

//------------------------------------------------------------------------------
void tetris::Line::rotate(const tetris::Point& pivotPoint) {
  if (pivotPoint.row < 0) return;
  drawLine(pivotPoint, EMPTY_SYMBOL);
  rotateLine();
  drawLine(pivotPoint, FILL_SYMBOL);
  refresh();
}

//------------------------------------------------------------------------------
void tetris::Line::clearTrail(const tetris::Point& new_location,
                              tetris::Figure::MoveDirection direction) {
  switch (direction) {
    case LEFT:
      clearVerticalLine(new_location.row, rangeRight(new_location.col));
      break;
    case RIGHT:
      clearVerticalLine(new_location.row, new_location.col - 1);
      break;
    case DOWN:
      clearHorizontalLine(topRange(new_location.row), new_location.col);
      break;
  }
}

//------------------------------------------------------------------------------
void tetris::Line::drawLine(const tetris::Point& pivotPoint,
                            const char symbol) {
  const std::string fill_line(m_currentSize->width, symbol);
  const int top = topRange(pivotPoint.row);
  for (int row_it(pivotPoint.row); row_it > top; --row_it) {
    mvprintw(row_it, pivotPoint.col, fill_line.c_str());
  }
}

//------------------------------------------------------------------------------
void tetris::Line::rotateLine() {
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

//------------------------------------------------------------------------------
void tetris::Line::clearVerticalLine(int row, int col) {
  const auto top = topRange(row);

  for (int row_it(row); row_it > top; --row_it) {
    mvaddch(row_it, col, EMPTY_SYMBOL);
  }
  refresh();
}

//------------------------------------------------------------------------------
void tetris::Line::clearHorizontalLine(int row, int col) {
  if (row < 0) return;

  static const std::string clear_line(m_currentSize->width, EMPTY_SYMBOL);
  mvprintw(row, col, clear_line.c_str());
  refresh();
}
