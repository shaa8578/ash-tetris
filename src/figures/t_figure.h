#pragma once

#include "figure_ext.h"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Фигура N
 *
 *        | xx   |        |   xx
 *        | xx   |        |   xx
 *   xx   | xxxx | xxxxxx | xxxx
 *   xx   | xxxx | xxxxxx | xxxx
 * xxxxxx | xx   |   xx   |   xx
 * xxxxxx | xx   |   xx   |   xx
 */
class TFigure : public FigureExt {
 public:
  explicit TFigure(Orientation orientation = HORIZONTAL);

 protected:
  virtual int defaultWidth() const override;
  virtual int defaultHeight() const override;

  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void drawHorizontal(const Point& pivotPoint, char symbol);
  virtual void drawVertical(const Point& pivotPoint, char symbol);
};

//------------------------------------------------------------------------------
} /* namespace tetris */
