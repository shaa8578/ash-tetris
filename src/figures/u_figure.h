#pragma once

#include "n_figure.h"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Фигура u
 *
 *  xx   |
 *  xx   |
 *  xxxx |   xxxx
 *  xxxx |   xxxx
 *    xx | xxxx
 *    xx | xxxx
 */
class UFigure : public NFigure {
 public:
  explicit UFigure(Orientation orientation = HORIZONTAL);

 protected:
  virtual void drawHorizontal(const Point& pivotPoint, char symbol) override;
  virtual void drawVertical(const Point& pivotPoint, char symbol) override;
  virtual std::vector<size_t> collisionMaskHorizontal(int row, int col) const;
  virtual std::vector<size_t> collisionMaskVertical(int row, int col) const;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
