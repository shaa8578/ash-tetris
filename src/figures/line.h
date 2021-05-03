#pragma once

#include <string>

#include "figure_ext.h"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Фигура Линия */
class Line : public FigureExt {
 public:
  explicit Line(Orientation orientation = HORIZONTAL);

 protected:
  virtual int defaultWidth() const override;
  virtual int defaultHeight() const override;

  virtual void draw(const Point& pivotPoint, char symbol) override;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
