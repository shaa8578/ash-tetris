#include <signal.h>
#include <unistd.h>

#include <clocale>
#include <cstring>
#include <iostream>
#include <stdexcept>

#if defined(USING_SRAND)
#include <ctime>
#endif /* USING_SRAND */

#include "game_play.h"

//------------------------------------------------------------------------------
/** Игровое пространство */
GamePlay game;

//------------------------------------------------------------------------------
void abortByThrowing();
void createUnexpectException();
void onQuitSignalReceived(int signo);
void initOsSignals();

//------------------------------------------------------------------------------
int main(/*int argc, char* argv[]*/) {
  setlocale(LC_ALL, "Russian");

  std::set_unexpected(createUnexpectException);
  std::set_terminate(abortByThrowing);

#if defined(USING_SRAND)
  srand(time(0));
#endif /* USING_SRAND */

  try {
    initOsSignals();
    game.init();
  } catch (std::exception& ex) {
    game.quit();
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  return game.exec();
}

//------------------------------------------------------------------------------
void abortByThrowing() {
  game.quit();
  std::cerr << "Аварийное завершение из-за необрабатываемого исключения"
            << std::endl;
  abort();
}

//------------------------------------------------------------------------------
void createUnexpectException() {
  game.quit();
  std::cerr << "Вызвано необрабатываемое исключение" << std::endl;
  throw;
}

//------------------------------------------------------------------------------
void onQuitSignalReceived(int signo) {
  game.quit();
  std::cout << "Колучен код завершения " << signo << std::endl;
  exit(0);
}

//------------------------------------------------------------------------------
void initOsSignals() {
  if (signal(SIGINT, &onQuitSignalReceived) == SIG_ERR) {
    throw std::domain_error("Ошибка подписки на сигнал SIGINT");
  }
  if (signal(SIGQUIT, &onQuitSignalReceived) == SIG_ERR) {
    throw std::domain_error("Ошибка подписки на сигнал SIGQUIT");
  }
  if (signal(SIGTERM, &onQuitSignalReceived) == SIG_ERR) {
    throw std::domain_error("Ошибка подписки на сигнал SIGTERM");
  }
}
