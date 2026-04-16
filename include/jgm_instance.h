#ifndef JGM_INCLUDE_INSTANCE_H_
#define JGM_INCLUDE_INSTANCE_H_

#include <memory>
#include <string>
#include <string_view>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace juantgd {
class JGMInstance {
public:
  static std::shared_ptr<JGMInstance>
  CreateEngine(std::string_view title, int width = 1280, int height = 720) {
    if (instance_ == nullptr) {
      instance_ =
          std::shared_ptr<JGMInstance>(new JGMInstance(title, width, height));
    }
    return instance_;
  }

  ~JGMInstance() {
    if (renderer_) {
      SDL_DestroyRenderer(renderer_);
      renderer_ = nullptr;
    }
    if (window_) {
      SDL_DestroyWindow(window_);
      window_ = nullptr;
    }
    TTF_Quit();
    SDL_Quit();
  }

  void EventLoop();

  JGMInstance(const JGMInstance &) = delete;
  JGMInstance &operator=(const JGMInstance &) = delete;
  JGMInstance(JGMInstance &&) = default;
  JGMInstance &operator=(JGMInstance &&) = default;

private:
  JGMInstance(std::string_view title, int width, int height);

  void __init_instance();
  void __update_gui();
  void __event_handle(const SDL_Event *event);

  SDL_Window *window_{nullptr};
  SDL_Renderer *renderer_{nullptr};

  std::string title_;
  int width_;
  int height_;
  int fps_{60};

  static std::shared_ptr<JGMInstance> instance_;

  bool running_{false};
};
} // namespace juantgd

#endif
