#pragma once

#include "figure_ext.h"

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
class NFigure : public FigureExt {
 public:
  explicit NFigure(Orientation orientation = HORIZONTAL);

 protected:
  virtual int defaultWidth() const override;
  virtual int defaultHeight() const override;

  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void drawHorizontal(const Point& pivotPoint, char symbol);
  virtual void drawVertical(const Point& pivotPoint, char symbol);
};

//------------------------------------------------------------------------------
} /* namespace tetris */
