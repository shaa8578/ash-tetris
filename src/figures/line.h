#pragma once

#include <string>

#include "figure_ext.h"

namespace tetris {

/** Фигура Линия */
class Line : public FigureExt {
 public:
  explicit Line(Orientation orientation = HORIZONTAL);

  std::vector<size_t> collisionMask(int row, int col,
                                    bool rotating) const override;

 protected:
  int defaultWidth() const override;
  int defaultHeight() const override;

 private:
  std::vector<size_t> collisionMask(int row, int col, int orientationWidth,
                                    int orientationHeight) const;
};

} /* namespace tetris */
