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

  virtual int width() const override;
  virtual std::vector<size_t> collisionMask(int row, int col,
                                            bool) const override;

 protected:
  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void rotateGeometry() override {}

  static int topRange(int row);
};

//------------------------------------------------------------------------------
} /* namespace tetris */
