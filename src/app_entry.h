#pragma once

#include <string>
#include <string_view>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class AppEntry {
public:
  AppEntry(std::string_view title, int width = 640, int height = 480);
  ~AppEntry();

  AppEntry(const AppEntry &) = delete;
  AppEntry &operator=(const AppEntry &) = delete;
  AppEntry(AppEntry &&) = default;
  AppEntry &operator=(AppEntry &&) = default;

  void EventLoop();

  void Update();

  void Shutdown() { running_ = false; }

private:
  void __init_sdl();

  SDL_Window *win_main_{nullptr};
  SDL_Renderer *win_renderer_{nullptr};
  TTF_Font *win_font_{nullptr};

  std::string win_title_;
  int win_width_;
  int win_height_;

  bool running_{false};
  std::string content_;
};
