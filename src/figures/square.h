#pragma once

#include "figure.h"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Фигура квадрата */
class Square : public Figure {
  /** Размер квадрата */
  static const Size SQUARE_SIZE;

 public:
  Square();

  virtual int rangeRight(int currentCol) const override;
  inline virtual int rangeRightRotated(int currentCol) const override {
    return rangeRight(currentCol);
  }

 protected:
  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void rotateGeometry() override {}

  static int topRange(int row);
};

//------------------------------------------------------------------------------
} /* namespace tetris */
