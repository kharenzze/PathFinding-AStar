//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include <game.h>
#include <defines.h>
#include <window.h>
#include <ctime>

int main(int,char*[]) {
  srand(time(NULL));

  Window::instance().init(GAME_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);

  {
    Game game;

    game.init();
    game.start();
    game.shutdown();
  }

  Window::instance().shutdown();

  return 0;
}
