#include "game_play.h"

#include <curses.h>

#include <cstring>
#include <stdexcept>

#include "figure.h"
#include "figures/line.h"
#include "figures/n_figure.h"
#include "figures/square.h"
#include "figures/u_figure.h"

//------------------------------------------------------------------------------
GamePlay::GamePlay() : m_working(0x0), m_inited(false), m_hasToolbox(false) {
  memset(&m_clientRange, 0x0, sizeof(tetris::Range));
}

//------------------------------------------------------------------------------
GamePlay::~GamePlay() {
  quit();
}

//------------------------------------------------------------------------------
void GamePlay::init() {
  initWindow();
  initGeometryParams();
  drawGameArea();
  if (m_hasToolbox) {
    drawHelp();
  }

  refresh();
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

  tetris::Point caret = {0, 10};
  tetris::Point prev_caret = {-1, 1};
  int input_symbol;

  //  m_current_figure.reset(new tetris::Square);
  //  m_current_figure.reset(new tetris::Line);
  //  m_current_figure.reset(new tetris::NFigure);
  m_current_figure.reset(new tetris::UFigure);

  while (m_working) {
    m_current_figure->clear(prev_caret);
    m_current_figure->draw(caret);
    refresh();
    prev_caret = caret;
    input_symbol = getch();
    switch (input_symbol) {
      case KEY_ESCAPE:
        unsetWorking();
        break;
      case KEY_S_BIG:
      case KEY_S_LITTLE:
      case KEY_DOWN:
        if (caret.row < m_clientRange.rowBottom - 1) caret.row++;
        break;
      case KEY_A_BIG:
      case KEY_A_LITTLE:
      case KEY_LEFT:
        if (caret.col > m_clientRange.colLeft + 1) caret.col--;
        break;
      case KEY_D_BIG:
      case KEY_D_LITTLE:
      case KEY_RIGHT:
        if (m_current_figure->rangeRight(caret.col) < m_clientRange.colRight) {
          caret.col++;
        }
        break;
      case KEY_SPACE:
        if (m_current_figure->rangeRightRotated(caret.col - 1) <
            m_clientRange.colRight) {
          m_current_figure->rotate(caret);
        }
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

  m_hasToolbox = COLS >= (MINIMUM_PLAY_WIDTH + TOOL_BOX_WIDTH);

  m_clientRange.colLeft = 0;
  m_clientRange.rowTop = 0;
  m_clientRange.colRight = (COLS < MINIMUM_PLAY_WIDTH) ? COLS : COLS / 2;
  /** NOTE: В разных терминала по разному определятеся количество строк
   *  поэтому уменьшаем на единицу для гарантированной отрисовки рамки */
  m_clientRange.rowBottom = LINES - 1;

  if ((m_clientRange.colRight & 0x01) < 1) {
    m_clientRange.colRight--;
  }
}

//------------------------------------------------------------------------------
void GamePlay::drawGameArea() {
  /* Вертикальные линии */
  for (int row_it(m_clientRange.rowTop); row_it < m_clientRange.rowBottom;
       ++row_it) {
    mvaddch(row_it, m_clientRange.colLeft, '|');
    mvaddch(row_it, m_clientRange.colRight, '|');
  }
  /* Горизонтальные линии */
  for (int col_it(m_clientRange.colLeft + 1); col_it < m_clientRange.colRight;
       ++col_it) {
    mvaddch(m_clientRange.rowBottom, col_it, '-');
  }
  /* Углы рамки */
  mvaddch(m_clientRange.rowBottom, m_clientRange.colLeft, '+');
  mvaddch(m_clientRange.rowBottom, m_clientRange.colRight, '+');
}

//------------------------------------------------------------------------------
void GamePlay::drawHelp() {
  const auto TEXT_SHIFT = 2;
  const auto HOR_LINE_ROW = m_clientRange.rowBottom - 5;
  for (int col_it(m_clientRange.colRight + 1); col_it < COLS; ++col_it) {
    mvaddch(HOR_LINE_ROW, col_it, '-');
  }
  mvprintw(m_clientRange.rowBottom - 4, m_clientRange.colRight + TEXT_SHIFT,
           "Move: [a],[s],[d], or arrows");
  mvprintw(m_clientRange.rowBottom - 3, m_clientRange.colRight + TEXT_SHIFT,
           "Rotate: [Space]");
  mvprintw(m_clientRange.rowBottom - 2, m_clientRange.colRight + TEXT_SHIFT,
           "Exit: [Escape]");
}

//------------------------------------------------------------------------------
void GamePlay::resetWindow() {
  bkgdset(COLOR_PAIR(0));
  clear();
  refresh();
  endwin();
}
