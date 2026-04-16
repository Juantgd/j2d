#include "jgm_instance.h"

#include "helper.h"

namespace juantgd {

std::shared_ptr<JGMInstance> JGMInstance::instance_ = nullptr;

JGMInstance::JGMInstance(std::string_view title, int width, int height)
    : title_(title), width_(width), height_(height) {
  __init_instance();
}

void JGMInstance::EventLoop() {
  LOG_I("game event loop starting...");
  running_ = true;
  SDL_Event event;
  while (running_) {
    Uint64 start_time = SDL_GetPerformanceCounter();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running_ = false;
        break;
      }
      __event_handle(&event);
    }
    __update_gui();
    double delta_time =
        static_cast<double>(SDL_GetPerformanceCounter() - start_time) /
        SDL_GetPerformanceFrequency();
    if (delta_time < 1.0f / fps_) {
      SDL_Delay((Uint32)((1.0f / fps_ - delta_time) * 1000));
    }
  }
}

void JGMInstance::__init_instance() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    LOG_E("SDL_Init failed. error: {}", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if (!TTF_Init()) {
    LOG_E("TTF_Init failed. error: {}", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  window_ =
      SDL_CreateWindow(title_.c_str(), width_, height_, SDL_WINDOW_RESIZABLE);
  if (window_ == nullptr) {
    LOG_E("SDL_CreateWindow failed. error: {}", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  renderer_ = SDL_CreateRenderer(window_, NULL);
  if (renderer_ == nullptr) {
    LOG_E("SDL_CreateRenderer failed. error: {}", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}

} // namespace juantgd
