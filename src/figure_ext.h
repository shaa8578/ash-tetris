#pragma once

#include <memory>

#include "figure.h"

//------------------------------------------------------------------------------
namespace tetris {

//------------------------------------------------------------------------------
/** Расширенный интерфейс фигур, используемых в игре */
class FigureExt : public Figure {
  /** Псевдоним указателя на размер фигуры */
  using PtrSize = std::unique_ptr<Size>;

 public:
  /** Ориентация фигуры */
  enum Orientation {
    HORIZONTAL, /**< Горизонтальная отрисовка фигуры */
    VERTICAL, /**< Вертикальная отрисовка фигуры */
    HORIZONTAL_INVERT, /**< Горизонтальная обратная отрисовка фигуры */
    VERTICAL_INVERT, /**< Вертикальная обратная отрисовка фигуры */
    ORIENTATION_SIZE /**< Количество возможных типов ориентации */
  };

  explicit FigureExt(Orientation orientation = HORIZONTAL);
  int width() const override;

 protected:
  virtual int defaultWidth() const = 0;
  virtual int defaultHeight() const = 0;

  const Size& currentSize() const override;
  int topRange(int row) override;
  void rotateGeometry() override;

  static void throwInvalidOrientation(Orientation errOrientation);

  Orientation orientation() const { return m_orientation; }
  Orientation nextOrientation() const;

 private:
  void rotateOrientation();
  void fillCurrentSize(Size* fillingSize) const;

 private:
  /** Направление отрисовки фигуры */
  Orientation m_orientation;
  /** Размер фигуры для текущего направления отрисовки */
  PtrSize m_currentSize;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
