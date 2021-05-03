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
  /** Возмноные направления перемещения фигуры */
  enum MoveDirection {
    LEFT,  /**< Перемещение влево */
    RIGHT, /**< Перемещение вправо */
    DOWN   /**< Перемещение вниз */
  };

  /** Ориентация фигуры */
  enum Orientation {
    HORIZONTAL, /**< Горизонтальная отрисовка фигуры */
    VERTICAL /**< Вертикальная отрисовка фигуры */
  };

  virtual ~Figure() = default;

  virtual int rangeRight(int currentCol) const = 0;
  virtual int rangeRightRotated(int currentCol) const = 0;
  virtual void draw(const Point& pivotPoint) = 0;
  virtual void rotate(const Point& pivotPoint) = 0;
  virtual void clearTrail(const Point& new_location,
                          MoveDirection direction) = 0;

 protected:
  /** Символ заполнитель */
  static const char FILL_SYMBOL = '#';
  /** Пустышка фона */
  static const char EMPTY_SYMBOL = ' ';
};

//------------------------------------------------------------------------------
} /* namespace tetris */
