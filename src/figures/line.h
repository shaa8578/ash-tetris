#pragma once

#include <string>

#include "figure_ext.h"

namespace tetris {

/** Фигура Линия */
class Line : public FigureExt {
 public:
  explicit Line(Orientation orientation = HORIZONTAL);

  virtual int width() const override;
  virtual std::vector<size_t> collisionMask(
      const Point& pivotPoint) const override;

 protected:
  virtual int defaultWidth() const override;
  virtual int defaultHeight() const override;

  virtual void draw(const Point& pivotPoint, char symbol) override;

 private:
  std::vector<size_t> collisionMask(const Point& pivotPoint,
                                    int orientationWidth,
                                    int orientationHeight) const;
};

} /* namespace tetris */
