#pragma once

#include "figure.h"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Фигура N
 *
 *    xx |
 *    xx |
 *  xxxx | xxxx
 *  xxxx | xxxx
 *  xx   |   xxxx
 *  xx   |   xxxx
 */
class NFigure : public Figure {
  /** Горизонтальный размер */
  static const Size HORIZONTAL_SIZE;
  /** Вертикальный размер */
  static const Size VERTICAL_SIZE;

 public:
  NFigure();

  virtual int rangeRight(int currentCol) const override;
  virtual int rangeRightRotated(int currentCol) const override;

 protected:
  virtual void rotateGeometry() override;
  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void drawHorizontal(const Point& pivotPoint, char symbol);
  virtual void drawVertical(const Point& pivotPoint, char symbol);
  int topRange(int row);

  const Size* currentSize() const { return m_currentSize; }

 private:
  /** Направление отрисовки фигуры */
  Figure::Orientation m_orientation;
  /** Размер фигуры для текущего направления отрисовки */
  const Size* m_currentSize;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
