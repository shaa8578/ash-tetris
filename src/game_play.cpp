#include "game_play.h"

#include <curses.h>
#include <stddef.h>

#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "collision_model.h"
#include "figures/line.h"
#include "figures/n_figure.h"
#include "figures/square.h"
#include "figures/t_figure.h"
#include "figures/u_figure.h"

//------------------------------------------------------------------------------
namespace {
/** Сдвиг по умолчанию таймера перерисовки фигуры, мс */
static const std::chrono::milliseconds DEFAULT_TIME_SHIFT(1500);
/** Минимальное значение таймера перерисовки фигур, ms */
static const std::chrono::milliseconds MINIMUM_TIMER(200);
/** Смещение текста относительно правой границы игрового поля */
static const auto TEXT_SHIFT = 2;
/** Отрисовываемый текст набранных очков */
static const char* POINTS_LABEL_STR = "Points: ";
/** Подпись следующей фигуры */
static const char* NEXT_FIGURE_LABEL_STR = "Next: ";

} /* unnamed namespace */
//------------------------------------------------------------------------------
GamePlay::GamePlay()
    : m_working(0x0),
      m_inited(false),
      m_hasToolbox(false),
      m_lineCost(0),
      m_currentLevel(0),
      m_points(0),
      m_pointsPoint(),
      m_nextFigurePoint(),
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
  drawPointsLabel();
  drawPointsValue();

  createNextFigure();
  drawNextFigureLabel();
  drawNextFigure();

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
    refresh();

    createFigure();
    figure_mask = m_currentFigure->collisionMask(
        m_currentPoint->row, m_currentPoint->col - m_clientRange.colLeft,
        m_currentPoint->rotating);
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
        auto begin_checked_row =
            m_currentPoint->row - static_cast<int>(figure_mask.size()) - 1;
        auto removed_rows_count = m_collisionModel->removeFullRows(
            begin_checked_row, m_currentPoint->row + 1);
        if (removed_rows_count > 0) {
          appendPoints(removed_rows_count / tetris::Figure::GLYPH_HEIGHT);
          refreshField(m_currentPoint->row);
          updateTimerShift();
        }
        m_currentFigure.reset();

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
  m_clientRange.width = m_clientRange.colRight - m_clientRange.colLeft - 1;

  m_pointsPoint.row = 1;
  m_pointsPoint.col =
      m_clientRange.colRight + TEXT_SHIFT + strlen(POINTS_LABEL_STR);

  m_nextFigurePoint.row = 3 + 4 * tetris::Figure::GLYPH_HEIGHT;
  m_nextFigurePoint.col =
      m_clientRange.colRight + TEXT_SHIFT + strlen(NEXT_FIGURE_LABEL_STR);

  m_lineCost = m_clientRange.width / tetris::Figure::GLYPH_WIDTH;
}

//------------------------------------------------------------------------------
void GamePlay::initCollisionModel() {
  m_collisionModel.reset(
      new CollisionModel(m_clientRange.width, m_clientRange.rowBottom));
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
void GamePlay::drawPointsLabel() {
  mvprintw(m_pointsPoint.row, m_clientRange.colRight + TEXT_SHIFT,
           POINTS_LABEL_STR);
}

//------------------------------------------------------------------------------
void GamePlay::drawPointsValue() {
  static const int POINTS_FILLER_SIZE = COLS - m_pointsPoint.col;
  static std::string points_filler(POINTS_FILLER_SIZE,
                                   tetris::Figure::EMPTY_SYMBOL);
  mvprintw(m_pointsPoint.row, m_pointsPoint.col, points_filler.c_str());
  mvprintw(m_pointsPoint.row, m_pointsPoint.col,
           std::to_string(m_points).c_str());
}

//------------------------------------------------------------------------------
void GamePlay::createNextFigure() {
  int figure_id(0);
  /** Исключаем повторение предыдущей фигуры */
  static int last_figure_id(-1);
  do {
    figure_id = std::rand() % 15;
  } while (figure_id == last_figure_id);
  last_figure_id = figure_id;

  switch (figure_id) {
    case 1:
      m_nextFigure.reset(new tetris::Line(tetris::FigureExt::VERTICAL));
      break;
    case 2:
      m_nextFigure.reset(new tetris::Line(tetris::FigureExt::HORIZONTAL));
      break;
    case 3:
      m_nextFigure.reset(new tetris::NFigure(tetris::FigureExt::HORIZONTAL));
      break;
    case 4:
      m_nextFigure.reset(new tetris::NFigure(tetris::FigureExt::VERTICAL));
      break;
    case 5:
      m_nextFigure.reset(
          new tetris::NFigure(tetris::FigureExt::HORIZONTAL_INVERT));
      break;
    case 6:
      m_nextFigure.reset(
          new tetris::NFigure(tetris::FigureExt::VERTICAL_INVERT));
      break;
    case 7:
      m_nextFigure.reset(new tetris::UFigure(tetris::FigureExt::HORIZONTAL));
      break;
    case 8:
      m_nextFigure.reset(new tetris::UFigure(tetris::FigureExt::VERTICAL));
      break;
    case 9:
      m_nextFigure.reset(
          new tetris::UFigure(tetris::FigureExt::HORIZONTAL_INVERT));
      break;
    case 10:
      m_nextFigure.reset(
          new tetris::UFigure(tetris::FigureExt::VERTICAL_INVERT));
      break;
    case 11:
      m_nextFigure.reset(new tetris::TFigure(tetris::FigureExt::HORIZONTAL));
      break;
    case 12:
      m_nextFigure.reset(new tetris::TFigure(tetris::FigureExt::VERTICAL));
      break;
    case 13:
      m_nextFigure.reset(
          new tetris::TFigure(tetris::FigureExt::HORIZONTAL_INVERT));
      break;
    case 14:
      m_nextFigure.reset(
          new tetris::TFigure(tetris::FigureExt::VERTICAL_INVERT));
      break;
    case 0:
    default:
      m_nextFigure.reset(new tetris::Square);
      break;
  }
}

//------------------------------------------------------------------------------
void GamePlay::drawNextFigureLabel() {
  mvprintw(m_nextFigurePoint.row, m_clientRange.colRight + TEXT_SHIFT,
           NEXT_FIGURE_LABEL_STR);
}

//------------------------------------------------------------------------------
void GamePlay::drawNextFigure() {
  m_nextFigure->draw(m_nextFigurePoint);
}

//------------------------------------------------------------------------------
void GamePlay::clearNextFigure() {
  m_nextFigure->clear(m_nextFigurePoint);
}

//------------------------------------------------------------------------------
void GamePlay::createFigure() {
  if (m_currentFigure != nullptr) return;

  clearNextFigure();
  m_currentFigure = m_nextFigure;
  createNextFigure();
  drawNextFigure();

  static const int CLIENT_AREA_CENTER =
      (m_clientRange.colRight - m_clientRange.colLeft) / 2;
  auto col = CLIENT_AREA_CENTER - m_currentFigure->width() / 2;
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
void GamePlay::appendPoints(int gliphRows) {
  m_points += float(gliphRows * m_lineCost) * pointPactor(gliphRows);
  drawPointsValue();
}

//------------------------------------------------------------------------------
float GamePlay::pointPactor(int gliphRows) const {
  switch (gliphRows) {
    case 4:
      return 2.0f;
    case 3:
      return 1.5f;
    default:
      break;
  }
  return 1.0f;
}

//------------------------------------------------------------------------------
void GamePlay::updateTimerShift() {
  if (m_points == 0) return;

  if (m_timerShift <= MINIMUM_TIMER) return;

  /** Количество очков для перехода на следующий уровень */
  static const unsigned int POINTS_FOR_NEXT_LEVEL = 200;

  auto next_level = m_currentLevel + 1;

  int next_points = static_cast<int>(next_level * POINTS_FOR_NEXT_LEVEL);
  if (static_cast<int>(m_points) - next_points >= 0) {
    /** Шаг уменьшения смещения, мс */
    static const std::chrono::milliseconds TIME_SHIFT_STEP(100);
    m_timerShift -= TIME_SHIFT_STEP;
    if (m_timerShift < MINIMUM_TIMER) {
      m_timerShift = MINIMUM_TIMER;
    }
  }
}

//------------------------------------------------------------------------------
void GamePlay::refreshField(int endRow) {
  static const size_t FULL_LINE_MASK = (size_t(1) << m_clientRange.width) - 1;
  for (int row(0); row < endRow + 1; ++row) {
    auto collision_row(m_collisionModel->value(row));
    /* Строка пустая */
    if ((collision_row & FULL_LINE_MASK) == 0) {
      drawFullLine(row, tetris::Figure::EMPTY_SYMBOL);
      continue;
    }
    /* Строка полностью заполнена */
    if ((collision_row ^ ~0) == 0) {
      drawFullLine(row, tetris::Figure::FILL_SYMBOL);
      continue;
    }
    /* Остальные случаи */
    drawLine(row, collision_row);
  }
}

//------------------------------------------------------------------------------
void GamePlay::drawFullLine(int row, char symbol) {
  std::string line(m_clientRange.width, symbol);
  mvprintw(row, m_clientRange.colLeft + 1, line.c_str());
}

//------------------------------------------------------------------------------
void GamePlay::drawLine(int row, size_t mask) {
  std::string line(m_clientRange.width, tetris::Figure::EMPTY_SYMBOL);
  for (int col(0); col < m_clientRange.width; ++col) {
    if (((mask >> col) & 0x1) > 0) {
      line[col] = tetris::Figure::FILL_SYMBOL;
    }
  }
  mvprintw(row, m_clientRange.colLeft + 1, line.c_str());
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
