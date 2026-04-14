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
  TTF_Quit();
  SDL_Quit();
}

void AppEntry::EventLoop() {
  running_ = true;
  SDL_Event ev;
  SDL_StartTextInput(win_main_);
  while (running_) {
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_EVENT_QUIT: {
        running_ = false;
        break;
      }
      case SDL_EVENT_KEY_DOWN: {
        switch (ev.key.key) {
        case SDLK_ESCAPE: {
          running_ = false;
          break;
        }
        case SDLK_BACKSPACE: {
          if (content_.size()) {
            content_.pop_back();
            LOG_I("current content: {}", content_);
          }
          break;
        }
        }
        break;
      }
      case SDL_EVENT_TEXT_INPUT: {
        content_.append(ev.text.text);
        LOG_I("current content: {}", content_);
        break;
      }
      case SDL_EVENT_MOUSE_MOTION: {
        LOG_I("Mouse moved to: ({},{})", ev.motion.x, ev.motion.y);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        LOG_I("Mouse button {} down at ({},{})", ev.button.button, ev.button.x,
              ev.button.y);
        break;
      }
      }
    }
    // fill white background color
    SDL_SetRenderDrawColor(win_renderer_, 255, 255, 255, 255);
    SDL_RenderClear(win_renderer_);
    Update();
    SDL_RenderPresent(win_renderer_);
  }
  SDL_StopTextInput(win_main_);
  LOG_I("closed window, process done.");
}

void AppEntry::Update() {
  SDL_Color black_color{0, 0, 0, 255};
  SDL_Surface *surface = TTF_RenderText_Blended(win_font_, content_.c_str(),
                                                content_.size(), black_color);
  // gpu texture
  SDL_Texture *texture = SDL_CreateTextureFromSurface(win_renderer_, surface);

  float text_width, text_height;
  SDL_GetTextureSize(texture, &text_width, &text_height);
  SDL_FRect textbox{50, 50, text_width, text_height};
  SDL_RenderTexture(win_renderer_, texture, NULL, &textbox);

  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface);
}

void AppEntry::__init_sdl() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    LOG_E("SDL_Init failed. error: {}", SDL_GetError());
    goto failed_exit;
  }
  if (!TTF_Init()) {
    LOG_E("TTF_Init failed. error: {}", SDL_GetError());
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
  win_font_ = TTF_OpenFont("FreeSans.ttf", 24);
  if (win_font_ == nullptr) {
    LOG_E("TTF_OpenFont failed. error: {}", SDL_GetError());
    goto failed_exit;
  }
  return;
failed_exit:
  exit(EXIT_FAILURE);
}
