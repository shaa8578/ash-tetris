#include "game_play.h"

#include <curses.h>
#include <stddef.h>

#include <cstring>
#include <stdexcept>

#include "collision_model.h"
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
  initCollisionModel();
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

  /** Маска предыдущего перемещения фигуры */
  std::vector<size_t> figure_mask;
  /** Признак коллизии по вертикали */
  bool was_vertical_collizion(false);

  while (m_working) {
    createFigure();
    figure_mask = m_currentFigure->collisionMask(*m_currentPoint);
    if (m_collisionModel->isCollision(*m_currentPoint, figure_mask)) {
      was_vertical_collizion =
          (m_currentPoint->row - m_previousPoint->row) != 0;
      *m_currentPoint = *m_previousPoint;
    } else {
      moveFigure();
    }

    if (isElapsedTimeout()) {
      if (was_vertical_collizion) {
        m_collisionModel->appendMask(*m_currentPoint, figure_mask);
        auto begin_checked_row = m_currentPoint->row - static_cast<int>(figure_mask.size());
        auto removed_rows_count = m_collisionModel->removeFullRows(begin_checked_row, m_currentPoint->row+1);
        if(removed_rows_count > 0){
          // TODO game_play.cpp: Добавить очков
          refreshField(m_currentPoint->row);
        }
        m_currentFigure.reset(nullptr);

        if (m_previousPoint->row <= static_cast<int>(figure_mask.size())) {
          break;
        }
        was_vertical_collizion = false;
        continue;
      }
      autoMoving();
      continue;
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
  static const int MINIMUM_PLAY_WIDTH = 40;
  /** Максимальная ширина экрана, символ */
  static const int MAXIMUM_PLAY_WIDTH = 64;
  /** Минимальная ширина панели инструментов */
  static const int TOOL_BOX_WIDTH = 20;

  m_hasToolbox = COLS >= (MINIMUM_PLAY_WIDTH + TOOL_BOX_WIDTH);

  m_clientRange.colLeft = 0;
  m_clientRange.rowTop = 0;
  m_clientRange.colRight = (COLS >= MINIMUM_PLAY_WIDTH * 2) ? COLS / 2 : COLS;
  /* Делаем ограничение для упрощения модели коллизий */
  if (m_clientRange.colRight > MAXIMUM_PLAY_WIDTH) {
    m_clientRange.colRight = MAXIMUM_PLAY_WIDTH;
  }
  /** NOTE: В разных терминала по разному определятеся количество строк
   *  поэтому уменьшаем на единицу для гарантированной отрисовки рамки */
  m_clientRange.rowBottom = LINES - 1;

  if ((m_clientRange.colRight & 0x01) < 1) {
    m_clientRange.colRight--;
  }
}

//------------------------------------------------------------------------------
void GamePlay::initCollisionModel() {
  m_collisionModel.reset(
      new CollisionModel(m_clientRange.colRight, m_clientRange.rowBottom));
}

//------------------------------------------------------------------------------
void GamePlay::initPreviousPoint() {
  m_previousPoint.reset(new tetris::Point);
}

//------------------------------------------------------------------------------
void GamePlay::initCurrentPoint() {
  m_currentPoint.reset(new tetris::Point);
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
void GamePlay::createFigure() {
  if (m_currentFigure != nullptr) return;

//    m_currentFigure.reset(new tetris::Square);
  m_currentFigure.reset(new tetris::Line(tetris::FigureExt::VERTICAL));
  //  m_currentFigure.reset(new tetris::NFigure);
  //  m_currentFigure.reset(new tetris::UFigure);
  //  m_currentFigure.reset(new tetris::TFigure);
  auto col = (m_clientRange.colRight - m_clientRange.colLeft) / 2 -
             m_currentFigure->width() / 2;
  if ((col & 0x01) < 1) {
    col--;
  }
  *m_currentPoint = *m_previousPoint = {/*row*/ 1, col, /*rotating*/ false};
}

//------------------------------------------------------------------------------
bool GamePlay::isElapsedTimeout() {
  auto current_time(std::chrono::system_clock::now());
  if (current_time >= m_timer) {
    m_timer = current_time + m_timerShift;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void GamePlay::refreshField(int endRow){
  // TODO game_play.cpp: refreshField()
}

//------------------------------------------------------------------------------
void GamePlay::autoMoving() {
  ++m_currentPoint->row;
}

//------------------------------------------------------------------------------
void GamePlay::moveFigure() {
  if (m_currentPoint->rotating) {
    m_currentFigure->rotate(*m_currentPoint);
    m_currentPoint->rotating = false;
  } else {
    m_currentFigure->clear(*m_previousPoint);
    m_currentFigure->draw(*m_currentPoint);
  }
  refresh();
  *m_previousPoint = *m_currentPoint;
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
      m_currentPoint->row++;
      break;
    case KEY_A_BIG:
    case KEY_A_LITTLE:
    case KEY_LEFT:
      m_currentPoint->col -= tetris::Figure::GLYPH_WIDTH;
      break;
    case KEY_D_BIG:
    case KEY_D_LITTLE:
    case KEY_RIGHT:
      m_currentPoint->col += tetris::Figure::GLYPH_WIDTH;
      break;
    case KEY_SPACE:
      m_currentPoint->rotating = true;
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
