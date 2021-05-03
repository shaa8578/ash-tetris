#pragma once

#include <string>

#include "figure.hpp"

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
  virtual void draw(const Point& pivotPoint) override;
  virtual void rotate(const Point& pivotPoint) override;
  virtual void clearTrail(const Point& new_location,
                          MoveDirection direction) override;

 protected:
  static int topRange(int row);
  static void clearVerticalLine(int row, int col);
  static void clearHorizontalLine(int row, int col);
};

//------------------------------------------------------------------------------
} /* namespace tetris */
