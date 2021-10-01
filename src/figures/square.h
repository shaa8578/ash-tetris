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

  int width() const override;
  std::vector<size_t> collisionMask(int row, int col, bool) const override;

 protected:
  const Size& currentSize() const override;
  void rotateGeometry() override {}
  int topRange(int row) override;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
