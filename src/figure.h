#pragma once

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
};

//------------------------------------------------------------------------------
/** Интерфейс фигур, используемых в игре */
class Figure {
 public:
  /** Ширина одной ячейки фигуры */
  static const int GLYPH_WIDTH = 2;
  /** Высота одной ячейки фигуры */
  static const int GLYPH_HEIGHT = 2;

  virtual ~Figure() = default;

  virtual int rangeRight(int locationCol) const = 0;
  virtual int rangeRightRotated(int locationCol) const = 0;

  void draw(const Point& pivotPoint);
  void clear(const Point& pivotPoint);
  void rotate(const Point& pivotPoint);

 protected:
  virtual void draw(const Point& pivotPoint, char symbol) = 0;
  virtual void rotateGeometry() = 0;

  /** Символ заполнитель */
  static const char FILL_SYMBOL = '#';
  /** Пустышка фона */
  static const char EMPTY_SYMBOL = ' ';
};

//------------------------------------------------------------------------------
} /* namespace tetris */
