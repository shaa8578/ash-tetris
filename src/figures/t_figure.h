#pragma once

#include "n_figure.h"

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
class TFigure : public NFigure {
 public:
  explicit TFigure(Orientation orientation = HORIZONTAL);

  std::vector<size_t> collisionMask(int row, int col,
                                    bool rotating) const override;

 protected:
  int defaultWidth() const override;
  int defaultHeight() const override;

  void drawHorizontal(const Point& pivotPoint, char symbol) override;
  void drawVertical(const Point& pivotPoint, char symbol) override;
  std::vector<size_t> collisionMaskHorizontal(int /*row*/,
                                              int col) const override;
  std::vector<size_t> collisionMaskVertical(int /*row*/,
                                            int col) const override;
  std::vector<size_t> collisionMaskHorizontalInvert(int col) const;
  std::vector<size_t> collisionMaskVerticalInvert(int col) const;
  Size figureSize(Orientation figureOrientation) const;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
