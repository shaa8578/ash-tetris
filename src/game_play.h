#pragma once

#include <memory>

//------------------------------------------------------------------------------
namespace tetris {
class Figure;

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
  void drawGameArea();
  void drawHelp();
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
  /** Текущая фигура */
  std::unique_ptr<tetris::Figure> m_current_figure;
};