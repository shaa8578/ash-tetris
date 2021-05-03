#include "square.h"

#include <curses.h>

//------------------------------------------------------------------------------
const tetris::Size tetris::Square::SQUARE_SIZE = {4, 4};

//------------------------------------------------------------------------------
tetris::Square::Square() : Figure() {}

//------------------------------------------------------------------------------
int tetris::Square::width() const {
  return SQUARE_SIZE.width;
}

//------------------------------------------------------------------------------
void tetris::Square::draw(const tetris::Point& pivotPoint) {
  if (pivotPoint.row < 0) return;

  static const std::string fill_line(SQUARE_SIZE.width, FILL_SYMBOL);
  const int top = solveTopRange(pivotPoint.row);
  for (int row_it(pivotPoint.row); row_it > top; --row_it) {
    mvprintw(row_it, pivotPoint.col, fill_line.c_str());
  }
  refresh();
}

//------------------------------------------------------------------------------
void tetris::Square::rotate(const tetris::Point& pivotPoint) {
  draw(pivotPoint);
}

//------------------------------------------------------------------------------
void tetris::Square::clearTrail(const tetris::Point& new_location,
                                tetris::Figure::MoveDirection direction) {
  switch (direction) {
    case LEFT:
      clearVerticalLine(new_location.row, new_location.col + SQUARE_SIZE.width);
      break;
    case RIGHT:
      clearVerticalLine(new_location.row, new_location.col - 1);
      break;
    case DOWN:
      clearHorizontalLine(new_location.row - SQUARE_SIZE.height,
                          new_location.col);
      break;
  }
}

//------------------------------------------------------------------------------
int tetris::Square::solveTopRange(int row) {
  auto result = row - SQUARE_SIZE.height;

  /** Верхняя граница отрисовки окна */
  static const int TOP_RANGE = -1;
  if (result < TOP_RANGE) result = TOP_RANGE;

  return result;
}

//------------------------------------------------------------------------------
void tetris::Square::clearVerticalLine(int row, int col) {
  const auto top = solveTopRange(row);

  for (int row_it(row); row_it > top; --row_it) {
    mvaddch(row_it, col, EMPTY_SYMBOL);
  }
  refresh();
}

//------------------------------------------------------------------------------
void tetris::Square::clearHorizontalLine(int row, int col) {
  if (row < 0) return;

  static const std::string clear_line(SQUARE_SIZE.width, EMPTY_SYMBOL);
  mvprintw(row, col, clear_line.c_str());
  refresh();
}
