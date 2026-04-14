#include "app_entry.h"

#include <cstdlib>

#include "helper.h"

AppEntry::AppEntry(std::string_view title, int width, int height)
    : win_title_(title), win_width_(width), win_height_(height) {
  __init_sdl();
}

AppEntry::~AppEntry() {
  running_ = false;
  if (win_font_) {
    TTF_CloseFont(win_font_);
    win_font_ = nullptr;
  }
  if (win_renderer_) {
    SDL_DestroyRenderer(win_renderer_);
    win_renderer_ = nullptr;
  }
  if (win_main_) {
    SDL_DestroyWindow(win_main_);
    win_main_ = nullptr;
  }
  TTF_Quit();
  SDL_Quit();
}

void AppEntry::EventLoop() {
  running_ = true;
  SDL_Event ev;
  SDL_StartTextInput(win_main_);
  while (running_) {
    uint64_t start_time = SDL_GetPerformanceCounter();
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
          }
          break;
        }
        case SDLK_RETURN: {
          content_.push_back('\n');
          break;
        }
        }
        break;
      }
      case SDL_EVENT_TEXT_INPUT: {
        content_.append(ev.text.text);
        break;
      }
      case SDL_EVENT_MOUSE_MOTION: {
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        break;
      }
      case SDL_EVENT_MOUSE_WHEEL: {
        // up +
        // down -
        scroll_offset_ -= ev.wheel.y * line_height_;
        break;
      }
      }
    }
    // fill white background color
    SDL_SetRenderDrawColor(win_renderer_, 255, 255, 255, 255);
    SDL_RenderClear(win_renderer_);
    Update();
    SDL_RenderPresent(win_renderer_);

    uint64_t current_time = SDL_GetPerformanceCounter();
    double delta_time = static_cast<double>(current_time - start_time) /
                        SDL_GetPerformanceFrequency();
    // LOG_I("delta_time: {}s, fps: {:.2f}", delta_time, 1.0f / frame_interval);
    if (delta_time < 1.0f / fps_) {
      // LOG_I("sleep time: {}s", 1.0f / fps_ - delta_time);
      SDL_Delay((Uint32)((1.0f / fps_ - delta_time) * 1000));
    }
  }
  SDL_StopTextInput(win_main_);
  LOG_I("closed window, process done.");
}

void AppEntry::Update() {
  // fill background color
  SDL_FRect editor_rect{10, 10, win_width_ - 20.0f, win_height_ - 20.0f};

  SDL_SetRenderDrawColor(win_renderer_, editor_background_color_.r,
                         editor_background_color_.g, editor_background_color_.b,
                         editor_background_color_.a);
  SDL_RenderFillRect(win_renderer_, &editor_rect);

  if (content_.empty())
    return;

  SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(
      win_font_, content_.c_str(), content_.size(), text_color_,
      win_width_ - 40);
  // gpu texture
  SDL_Texture *texture = SDL_CreateTextureFromSurface(win_renderer_, surface);
  SDL_DestroySurface(surface);

  float text_width, text_height;
  SDL_GetTextureSize(texture, &text_width, &text_height);

  // limit scroll height
  float max_scroll_height = 0.0f;

  SDL_Rect content_rect{(int)editor_rect.x + 6, (int)editor_rect.y + 6,
                        (int)editor_rect.w - 12, (int)editor_rect.h - 12};

  if (text_height > content_rect.h - line_height_) {
    max_scroll_height = text_height - line_height_;
  }

  if (scroll_offset_ < 0.0f)
    scroll_offset_ = 0.0f;
  else if (scroll_offset_ > max_scroll_height)
    scroll_offset_ = max_scroll_height;

  SDL_SetRenderClipRect(win_renderer_, &content_rect);

  SDL_FRect text_rect{editor_rect.x + 6.0f,
                      editor_rect.y + 6.0f - scroll_offset_, text_width,
                      text_height};

  SDL_RenderTexture(win_renderer_, texture, NULL, &text_rect);

  SDL_SetRenderClipRect(win_renderer_, NULL);

  SDL_DestroyTexture(texture);
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
  win_font_ = TTF_OpenFont("FreeSans.ttf", 16);
  if (win_font_ == nullptr) {
    LOG_E("TTF_OpenFont failed. error: {}", SDL_GetError());
    goto failed_exit;
  }
  return;
failed_exit:
  exit(EXIT_FAILURE);
}
