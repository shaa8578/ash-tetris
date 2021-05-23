#include "game_play.h"

#include <curses.h>

#include <cstring>
#include <stdexcept>

#include "figure.h"
#include "figures/line.h"
#include "figures/n_figure.h"
#include "figures/square.h"
#include "figures/t_figure.h"
#include "figures/u_figure.h"

//------------------------------------------------------------------------------
namespace {
/** Сдвиг по умолчанию таймера перерисовки фигуры, мс */
static const std::chrono::milliseconds DEFAULT_TIME_SHIFT(1500);

} /* unnamed namespace */
//------------------------------------------------------------------------------
GamePlay::GamePlay()
    : m_working(0x0),
      m_inited(false),
      m_hasToolbox(false),
      m_timerShift(DEFAULT_TIME_SHIFT) {
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
  initPreviousPoint();
  initCurrentPoint();
  initTimers();

  drawGameArea();
  drawHelp();
  refresh();

  m_inited = true;
}

//------------------------------------------------------------------------------
int GamePlay::exec() {
  if (isInited() == false) {
    quit();
    return EXIT_FAILURE;
  }
  setWorking();

  //  m_current_figure.reset(new tetris::Square);
  //  m_current_figure.reset(new tetris::Line(tetris::FigureExt::VERTICAL));
  //  m_current_figure.reset(new tetris::NFigure);
  //  m_current_figure.reset(new tetris::UFigure);
  m_current_figure.reset(new tetris::TFigure);

  while (m_working) {
    autoMoving();
    if (canMoving()) {
      moveFigure();
    } else {
      *m_current_point = *m_previous_point;
    }

    userMoving();
  }

  quit();
  return EXIT_SUCCESS;
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
void GamePlay::initPreviousPoint() {
  m_previous_point.reset(new tetris::Point);
  m_previous_point->col = -1;
  m_previous_point->row = -1;
}

//------------------------------------------------------------------------------
void GamePlay::initCurrentPoint() {
  m_current_point.reset(new tetris::Point);
  m_current_point->col = 10;
  m_current_point->row = -1;
}

//------------------------------------------------------------------------------
void GamePlay::initTimers() {
  /* Время ожидания действия пользователя, милисекунда */
  timeout(100);

  /* Инициализация таймера автосдвига фигуры */
  m_timer = Clock::now();
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
  if (m_hasToolbox == false) return;

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
void GamePlay::autoMoving() {
  auto current_time(std::chrono::system_clock::now());
  if (current_time >= m_timer) {
    m_timer = current_time + m_timerShift;
    ++m_current_point->row;
  }
}

//------------------------------------------------------------------------------
bool GamePlay::canMoving() {
  bool result(false);
  if (m_current_point->rotating) {
    result = (m_current_figure->rangeRightRotated(m_current_point->col - 1) <
              m_clientRange.colRight);

  } else {
    result = (m_current_point->row < m_clientRange.rowBottom) &&
             (m_current_point->col > m_clientRange.colLeft) &&
             (m_current_figure->rangeRight(m_current_point->col) <
              m_clientRange.colRight);
  }
  return result;
}

//------------------------------------------------------------------------------
void GamePlay::moveFigure() {
  if (m_current_point->rotating) {
    m_current_figure->rotate(*m_current_point);
    m_current_point->rotating = false;
  } else {
    m_current_figure->clear(*m_previous_point);
    m_current_figure->draw(*m_current_point);
  }
  refresh();
  *m_previous_point = *m_current_point;
}

//------------------------------------------------------------------------------
void GamePlay::userMoving() {
  /** Кнопка Escape */
  static const char KEY_ESCAPE = 033; /* 27 */
  /** Кнопка пробел */
  static const char KEY_SPACE = 040; /* 32 */
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

  int input_symbol = getch();
  switch (input_symbol) {
    case KEY_ESCAPE:
      unsetWorking();
      break;
    case KEY_S_BIG:
    case KEY_S_LITTLE:
    case KEY_DOWN:
      m_current_point->row++;
      break;
    case KEY_A_BIG:
    case KEY_A_LITTLE:
    case KEY_LEFT:
      m_current_point->col--;
      break;
    case KEY_D_BIG:
    case KEY_D_LITTLE:
    case KEY_RIGHT:
      m_current_point->col++;
      break;
    case KEY_SPACE:
      m_current_point->rotating = true;
      break;
    default:
      break;
  }
}

//------------------------------------------------------------------------------
void GamePlay::resetWindow() {
  bkgdset(COLOR_PAIR(0));
  clear();
  refresh();
  endwin();
}
