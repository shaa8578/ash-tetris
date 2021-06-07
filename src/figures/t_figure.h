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

  virtual std::vector<size_t> collisionMask(int row, int col,
                                            bool rotating) const override;

 protected:
  virtual int defaultWidth() const override;
  virtual int defaultHeight() const override;

  virtual void draw(const Point& pivotPoint, char symbol) override;
  void drawHorizontal(const Point& pivotPoint, char symbol);
  void drawVertical(const Point& pivotPoint, char symbol);
  std::vector<size_t> collisionMaskHorizontal(int col) const;
  std::vector<size_t> collisionMaskHorizontalInvert(int col) const;
  std::vector<size_t> collisionMaskVertical(int col) const;
  std::vector<size_t> collisionMaskVerticalInvert(int col) const;
  Size figureSize(Orientation figureOrientation) const;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
