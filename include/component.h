#ifndef JGM_INCLUDE_COMPONENT_H_
#define JGM_INCLUDE_COMPONENT_H_

#include <SDL3/SDL.h>

namespace juantgd {

class Component {
public:
  Component() = default;
  virtual ~Component() = default;

  virtual void RenderComponent(SDL_Renderer *renderer) = 0;

protected:
  SDL_FRect view_rect;
};

} // namespace juantgd

#endif
