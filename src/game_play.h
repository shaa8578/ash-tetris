#pragma once

#include <chrono>
#include <memory>

#include "figure.h"

//------------------------------------------------------------------------------
using Clock = std::chrono::system_clock;

//------------------------------------------------------------------------------
namespace tetris {

/** Границы косольной области */
struct Range {
  /** Номер верхней строки */
  int rowTop;
  /** Номер нижней строки */
  int rowBottom;
  /** Номер левого символа */
  int colLeft;
  /** Номер правого символа */
  int colRight;
  /** Длина игрового поля */
  int width;
};
} /* namespace tetris */

class CollisionModel;

//------------------------------------------------------------------------------
/** Основное игровое пространство */
class GamePlay {
 public:
  GamePlay();
  ~GamePlay();

  inline bool isInited() const { return m_inited; }

  void init();
  int exec();
  void quit();

 protected:
  inline void setWorking() { m_working |= char(0x01); }
  inline void unsetWorking() { m_working = char(0x0); }

  void initWindow();
  void initGeometryParams();
  void initCollisionModel();
  void initPreviousPoint();
  void initCurrentPoint();
  void initTimers();
  void drawGameArea();
  void drawHelp();
  void drawPointsLabel();
  void drawPointsValue();

  void createNextFigure();
  void drawNextFigureLabel();
  void drawNextFigure();
  void clearNextFigure();

  void createFigure();
  bool isElapsedTimeout();
  void appendPoints(int gliphRows);
  float pointPactor(int gliphRows) const;
  void updateTimerShift();
  void refreshField(int endRow);
  void drawFullLine(int row, char symbol);
  void drawLine(int row, size_t mask);
  void autoMoving();
  void moveFigure();
  void userMoving();

  void resetWindow();

 private:
  /** Признак работы главного цикла */
  volatile char m_working : 1;
  /** Признак инициализации игры */
  bool m_inited;
  /** Если true, то есть место под панель инструментов */
  bool m_hasToolbox;
  /** Количество очков за одну удалённую линию */
  unsigned int m_lineCost;
  /** Текущий уровень сложности */
  unsigned int m_currentLevel;
  /** Количество игровых очков */
  unsigned int m_points;
  /** Точка вставки количества очков */
  tetris::Point m_pointsPoint;
  /** Точка вставки следующей фигуры */
  tetris::Point m_nextFigurePoint;
  /** Шаг сдвига таймера */
  std::chrono::milliseconds m_timerShift;
  /** Время срабатывания таймера */
  Clock::time_point m_timer;
  /** Размеры поля игры */
  tetris::Range m_clientRange;
  /** Следующая фигура */
  std::shared_ptr<tetris::Figure> m_nextFigure;
  /** Текущая фигура */
  std::shared_ptr<tetris::Figure> m_currentFigure;
  /** Текущая позиция фигуры */
  std::unique_ptr<tetris::Point> m_currentPoint;
  /** Предыдущая позиция фигуры */
  std::unique_ptr<tetris::Point> m_previousPoint;
  /** Модель коллизий */
  std::unique_ptr<CollisionModel> m_collisionModel;
};
