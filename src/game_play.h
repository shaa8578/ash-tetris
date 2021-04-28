#pragma once

//------------------------------------------------------------------------------
/** Основное игровое пространство */
class GamePlay {
  /** Размеры игровой области */
  struct GamePlaySize {
    int xMin;
    int xMax;
    int yMin;
    int yMax;

    bool hasToolBox;

    GamePlaySize();
  };

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
  /** Размеры поля игры */
  GamePlaySize m_size;
};
