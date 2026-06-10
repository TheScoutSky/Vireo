#pragma once

#include <SDL.h>

#include <vireo/core/export.hpp>

namespace vireo {

class Renderer;

class VIREO_API Widget {
  public:
    virtual ~Widget() = default;

    virtual bool handle_event(const SDL_Event& event);
    virtual void update(double delta_seconds);
    virtual void render(Renderer& renderer) = 0;
};

} // namespace vireo
