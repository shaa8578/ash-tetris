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
  UFigure();

 protected:
  virtual void drawHorizontal(const Point& pivotPoint, char symbol) override;
  virtual void drawVertical(const Point& pivotPoint, char symbol) override;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
