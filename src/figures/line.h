#pragma once

#include <string>

#include "figure.h"

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

 protected:
  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void rotateGeometry() override;
  int topRange(int row);

 private:
  /** Направление отрисовки фигуры */
  Figure::Orientation m_orientation;
  /** Размер фигуры для текущего направления отрисовки */
  const Size* m_currentSize;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
