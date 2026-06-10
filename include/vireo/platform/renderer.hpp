#pragma once

#include <SDL.h>

#include <vireo/core/color.hpp>
#include <vireo/core/export.hpp>
#include <vireo/platform/window.hpp>

namespace vireo {

struct RendererOptions {
    int driver_index{-1};
    Uint32 flags{SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};
};

class VIREO_API Renderer final {
  public:
    explicit Renderer(Window& window, const RendererOptions& options = {});
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    [[nodiscard]] SDL_Renderer* native_handle() noexcept;
    [[nodiscard]] const SDL_Renderer* native_handle() const noexcept;

    void set_draw_color(Color color);
    void clear(Color color = rgba(20, 22, 28));
    void present() noexcept;

  private:
    void reset() noexcept;

    SDL_Renderer* handle_{nullptr};
};

} // namespace vireo
