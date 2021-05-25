#pragma once

#include <chrono>
#include <memory>

//------------------------------------------------------------------------------
using Clock = std::chrono::system_clock;

//------------------------------------------------------------------------------
namespace tetris {
class Figure;
struct Point;

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

  void createFigure();
  bool isElapsedTimeout();
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
  /** Размеры поля игры */
  tetris::Range m_clientRange;
  /** Шаг сдвига таймера */
  std::chrono::milliseconds m_timerShift;
  /** Время срабатывания таймера */
  Clock::time_point m_timer;
  /** Текущая фигура */
  std::unique_ptr<tetris::Figure> m_currentFigure;
  /** Текущая позиция фигуры */
  std::unique_ptr<tetris::Point> m_currentPoint;
  /** Предыдущая позиция фигуры */
  std::unique_ptr<tetris::Point> m_previousPoint;
  /** Модель коллизий */
  std::unique_ptr<CollisionModel> m_collisionModel;
};
