#pragma once

#include <stddef.h>

#include <vector>

namespace tetris {

//------------------------------------------------------------------------------
/** Размер фигуры */
struct Size {
  /** Ширина фигуры, символ */
  int width;
  /** Высота фигуры, символ */
  int height;
};

//------------------------------------------------------------------------------
/** Нулевая точка отрисовки фигуры */
struct Point {
  /** Порядковый номер строки с нуля */
  int row;
  /** Порядковый номер символа с нуля */
  int col;
  /** Признак вращения вигуры */
  bool rotating;
};

//------------------------------------------------------------------------------
/** Интерфейс фигур, используемых в игре */
class Figure {
 public:
  /** Ширина одной ячейки фигуры */
  static const int GLYPH_WIDTH = 2;
  /** Высота одной ячейки фигуры */
  static const int GLYPH_HEIGHT = 2;

  /** Символ заполнитель */
  static const char FILL_SYMBOL = '#';
  /** Пустышка фона */
  static const char EMPTY_SYMBOL = ' ';

  virtual ~Figure() = default;

  virtual int width() const = 0;
  virtual std::vector<size_t> collisionMask(int row, int col,
                                            bool rotating) const = 0;

  void draw(const Point& pivotPoint);
  void clear(const Point& pivotPoint);
  void rotate(const Point& pivotPoint);

 protected:
  virtual void draw(const Point& pivotPoint, char symbol) = 0;
  virtual void rotateGeometry() = 0;
};

//------------------------------------------------------------------------------
} /* namespace tetris */
