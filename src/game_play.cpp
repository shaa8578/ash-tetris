#include "game_play.h"

#include <ncurses.h>

#include <cstring>
#include <stdexcept>

//------------------------------------------------------------------------------
GamePlay::GamePlaySize::GamePlaySize() {
  memset(this, 0x0, sizeof(GamePlay::GamePlaySize));
}

//------------------------------------------------------------------------------
GamePlay::GamePlay() : m_working(0x0), m_inited(false) {}

//------------------------------------------------------------------------------
GamePlay::~GamePlay() {
  quit();
}

//------------------------------------------------------------------------------
void GamePlay::init() {
  initWindow();
  initGeometryParams();
  drawGameArea();
  if (m_size.hasToolBox) {
    drawHelp();
  }
  m_inited = true;
}

//------------------------------------------------------------------------------
int GamePlay::exec() {
  /** Кнопка Escape */
  static const char KEY_ESCAPE = 033; /* 27 */
  /** Кнопка пробел */
  static const char KEY_SPACE = 32;
  /** Прописная буква a */
  static const char KEY_A_LITTLE = 'a';
  /** Заглавная буква A */
  static const char KEY_A_BIG = 'A';
  /** Прописная буква s */
  static const char KEY_S_LITTLE = 's';
  /** Заглавная буква S */
  static const char KEY_S_BIG = 'S';
  /** Прописная буква d */
  static const char KEY_D_LITTLE = 'd';
  /** Заглавная буква D */
  static const char KEY_D_BIG = 'D';

  setWorking();

  char print_sumbol = '*';

  int prev_x = 10, prev_y = 0, caret_y = 0, caret_x = 10, c;

  while (m_working) {
    mvaddch(prev_y, prev_x, ' ');
    mvaddch(caret_y, caret_x, print_sumbol);
    move(caret_y, caret_x);
    refresh();
    prev_x = caret_x;
    prev_y = caret_y;
    c = getch();
    switch (c) {
      case KEY_ESCAPE:
        unsetWorking();
        break;
      case 'w':
      case 'W':
      case KEY_UP:
        if (caret_y > m_size.yMin) caret_y--;
        break;
      case KEY_S_BIG:
      case KEY_S_LITTLE:
      case KEY_DOWN:
        if (caret_y < m_size.yMax - 1) caret_y++;
        break;
      case KEY_A_BIG:
      case KEY_A_LITTLE:
      case KEY_LEFT:
        if (caret_x > m_size.xMin + 1) caret_x--;
        break;
      case KEY_D_BIG:
      case KEY_D_LITTLE:
      case KEY_RIGHT:
        if (caret_x < m_size.xMax - 1) caret_x++;
        break;
      case KEY_SPACE:
        print_sumbol = (print_sumbol == '*') ? '#' : '*';
        break;
    }
  }

  quit();
  return 0;
}

//------------------------------------------------------------------------------
void GamePlay::quit() {
  unsetWorking();
  if (m_inited) {
    resetWindow();
    m_inited = false;
  }
}

//------------------------------------------------------------------------------
void GamePlay::initWindow() {
  if (!initscr()) throw std::domain_error("Ошибка инициализации консоли");

  cbreak();
  noecho();
  nonl();
  meta(stdscr, TRUE);
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);

  clear();
}

//------------------------------------------------------------------------------
void GamePlay::initGeometryParams() {
  /** Минимальная ширина экрана, символ */
  const int MINIMUM_PLAY_WIDTH = 40;
  /** Минимальная ширина панели инструментов */
  const int TOOL_BOX_WIDTH = 20;

  m_size.hasToolBox = COLS >= (MINIMUM_PLAY_WIDTH + TOOL_BOX_WIDTH);

  m_size.xMin = 0;
  m_size.yMin = 0;
  m_size.xMax = (COLS < MINIMUM_PLAY_WIDTH) ? COLS : COLS / 2;
  /** NOTE: В разных терминала по разному определятеся количество строк
   *  поэтому уменьшаем на единицу для гарантированной отрисовки рамки */
  m_size.yMax = LINES - 1;

  if ((m_size.xMax & 0x01) < 1) {
    m_size.xMax--;
  }
}

//------------------------------------------------------------------------------
void GamePlay::drawGameArea() {
  /* Вертикальные линии */
  for (int row_it(m_size.yMin); row_it < m_size.yMax; ++row_it) {
    mvaddch(row_it, m_size.xMin, '|');
    mvaddch(row_it, m_size.xMax, '|');
  }
  /* Горизонтальные линии */
  for (int col_it(m_size.xMin + 1); col_it < m_size.xMax; ++col_it) {
    mvaddch(m_size.yMax, col_it, '-');
  }
  /* Углы рамки */
  mvaddch(m_size.yMax, m_size.xMin, '+');
  mvaddch(m_size.yMax, m_size.xMax, '+');

  refresh();
}

//------------------------------------------------------------------------------
void GamePlay::drawHelp() {
  const auto TEXT_SHIFT = 2;
  const auto HOR_LINE_Y = m_size.yMax - 5;
  for (int col_it(m_size.xMax + 1); col_it < COLS; ++col_it) {
    mvaddch(HOR_LINE_Y, col_it, '-');
  }
  mvprintw(m_size.yMax - 4, m_size.xMax + TEXT_SHIFT, "Move: a,s,d, or arrows");
  mvprintw(m_size.yMax - 3, m_size.xMax + TEXT_SHIFT, "Flip: space");
  mvprintw(m_size.yMax - 2, m_size.xMax + TEXT_SHIFT, "Exit: Escape");
}

//------------------------------------------------------------------------------
void GamePlay::resetWindow() {
  bkgdset(COLOR_PAIR(0));
  clear();
  refresh();
  endwin();
}
