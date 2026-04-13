#include "app_entry.h"

#include <cstdlib>

#include "helper.h"

AppEntry::AppEntry(std::string_view title, int width, int height)
    : win_title_(title), win_width_(width), win_height_(height) {
  __init_sdl();
}

AppEntry::~AppEntry() {
  running_ = false;
  if (win_renderer_) {
    SDL_DestroyRenderer(win_renderer_);
  }
  if (win_main_) {
    SDL_DestroyWindow(win_main_);
  }
  SDL_Quit();
}

void AppEntry::EventLoop() {
  running_ = true;
  SDL_Event ev;
  SDL_FRect color_rect{300, 300, 400, 300};
  while (running_) {
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_EVENT_QUIT: {
        running_ = false;
        break;
      }
      }
    }
    SDL_SetRenderDrawColor(win_renderer_, 0, 0, 0, 255);
    SDL_RenderClear(win_renderer_);
    SDL_SetRenderDrawColor(win_renderer_, 0, 255, 0, 255);
    SDL_RenderFillRect(win_renderer_, &color_rect);
    SDL_RenderPresent(win_renderer_);
    SDL_Delay(100);
  }
  LOG_I("closed window, process done.");
}

void AppEntry::__init_sdl() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    LOG_E("SDL_Init failed. error: {}", SDL_GetError());
    goto failed_exit;
  }
  win_main_ = SDL_CreateWindow(win_title_.c_str(), win_width_, win_height_, 0);
  if (win_main_ == nullptr) {
    LOG_E("SDL_CreateWindow failed. error: {}", SDL_GetError());
    goto failed_exit;
  }
  win_renderer_ = SDL_CreateRenderer(win_main_, NULL);
  if (win_main_ == nullptr) {
    LOG_E("SDL_CreateRenderer failed. error: {}", SDL_GetError());
    goto failed_exit;
  }
  return;
failed_exit:
  exit(EXIT_FAILURE);
}
