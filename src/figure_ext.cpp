#include "figure_ext.h"

#include <curses.h>

#include <stdexcept>

//------------------------------------------------------------------------------
tetris::FigureExt::FigureExt(Orientation orientation)
    : Figure(), m_orientation(orientation), m_currentSize(nullptr) {}

//------------------------------------------------------------------------------
const tetris::Size& tetris::FigureExt::currentSize() const {
  if (m_currentSize == nullptr) {
    auto current_size = const_cast<PtrSize*>(&m_currentSize);
    current_size->reset(new Size);
    fillCurrentSize(current_size->get());
  }
  return *m_currentSize;
}

//------------------------------------------------------------------------------
int tetris::FigureExt::topRange(int row) {
  auto result = row - currentSize().height;

  /** Верхняя граница отрисовки окна */
  static const int TOP_RANGE = -1;
  if (result < TOP_RANGE) result = TOP_RANGE;

  return result;
}

//------------------------------------------------------------------------------
void tetris::FigureExt::throwInvalidOrientation() const {
  throw std::invalid_argument("Orientation \"" +
                              std::to_string(static_cast<int>(m_orientation)) +
                              "\" not implemented");
}

//------------------------------------------------------------------------------
void tetris::FigureExt::rotateGeometry() {
  rotateOrientation();
  fillCurrentSize(m_currentSize.get());
}

//------------------------------------------------------------------------------
void tetris::FigureExt::rotateOrientation() {
  int current_orientation_idx(m_orientation);
  current_orientation_idx++;
  if (current_orientation_idx >= ORIENTATION_SIZE) {
    current_orientation_idx = 0;
  }
  m_orientation = static_cast<Orientation>(current_orientation_idx);
}

//------------------------------------------------------------------------------
void tetris::FigureExt::fillCurrentSize(Size* fillingSize) const {
  switch (m_orientation) {
    case HORIZONTAL:
    case HORIZONTAL_INVERT:
      fillingSize->width = defaultWidth();
      fillingSize->height = defaultHeight();
      break;
    case VERTICAL:
    case VERTICAL_INVERT:
      fillingSize->width = defaultHeight();
      fillingSize->height = defaultWidth();
      break;
    default:
      throwInvalidOrientation();
  }
}
