#pragma once

#include <SDL.h>

#include <string>
#include <string_view>

#include <vireo/core/export.hpp>
#include <vireo/core/geometry.hpp>

namespace vireo {

struct WindowOptions {
    std::string title{"Vireo"};
    int width{1280};
    int height{720};
    int x{SDL_WINDOWPOS_CENTERED};
    int y{SDL_WINDOWPOS_CENTERED};
    Uint32 flags{SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE};
};

class VIREO_API Window final {
  public:
    explicit Window(const WindowOptions& options = {});
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    [[nodiscard]] SDL_Window* native_handle() noexcept;
    [[nodiscard]] const SDL_Window* native_handle() const noexcept;
    [[nodiscard]] Size size() const noexcept;

    void set_title(std::string_view title);

  private:
    void reset() noexcept;

    SDL_Window* handle_{nullptr};
};

} // namespace vireo
