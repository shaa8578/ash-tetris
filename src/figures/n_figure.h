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

  virtual std::vector<size_t> collisionMask(int row, int col,
                                            bool rotating) const override;

 protected:
  virtual int defaultWidth() const override;
  virtual int defaultHeight() const override;

  virtual void draw(const Point& pivotPoint, char symbol) override;
  virtual void drawHorizontal(const Point& pivotPoint, char symbol);
  virtual void drawVertical(const Point& pivotPoint, char symbol);
  virtual std::vector<size_t> collisionMaskHorizontal(int row, int col) const;
  virtual std::vector<size_t> collisionMaskVertical(int row, int col) const;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
