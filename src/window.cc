//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include <window.h>

#include <cstdio>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

bool Window::init(const char* name, const uint32_t width, const uint32_t height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  } else {
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      printf("Warning: Linear texture filtering not enabled!");
    }

    window_ = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window_) {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      return false;
    } else {
      SDL_SetWindowTitle(window_, name);

      renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (!renderer_) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
      } else {
        SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
          printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
          return false;
        }

        if (TTF_Init() == -1) {
          printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
          return false;
        }
      }
    }
  }

  return true;
}

void Window::shutdown() {
  SDL_DestroyRenderer(renderer_);
  renderer_ = nullptr;
  SDL_DestroyWindow(window_);
  window_ = nullptr;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}