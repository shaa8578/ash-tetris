#pragma once

#include <string>

#include "figure.hpp"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Фигура Линия */
class Line : public Figure {
  /** Горизонтальный размер линии */
  static const Size HORIZONTAL_SIZE;
  /** Вертикальный размер линии */
  static const Size VERTICAL_SIZE;

 public:
  Line();

  virtual int rangeRight(int currentCol) const override;
  virtual int rangeRightRotated(int currentCol) const override;
  virtual void draw(const Point& pivotPoint) override;
  virtual void rotate(const Point& pivotPoint) override;
  virtual void clearTrail(const Point& new_location,
                          MoveDirection direction) override;

 protected:
  void drawLine(const Point& pivotPoint, const char symbol);
  void rotateLine();
  int topRange(int row);
  void clearVerticalLine(int row, int col);
  void clearHorizontalLine(int row, int col);

 private:
  /** Направление отрисовки фигуры */
  Figure::Orientation m_orientation;
  /** Размер фигуры для текущего направления отрисовки */
  const Size* m_currentSize;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
