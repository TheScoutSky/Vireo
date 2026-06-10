#include <vireo/platform/window.hpp>

#include <utility>

#include "sdl_error.hpp"

namespace vireo {

Window::Window(const WindowOptions& options) {
    handle_ =
        SDL_CreateWindow(options.title.c_str(), options.x, options.y, options.width, options.height, options.flags);

    if (handle_ == nullptr) {
        detail::throw_sdl_error("SDL_CreateWindow");
    }
}

Window::~Window() {
    reset();
}

Window::Window(Window&& other) noexcept : handle_{std::exchange(other.handle_, nullptr)} {}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        reset();
        handle_ = std::exchange(other.handle_, nullptr);
    }

    return *this;
}

SDL_Window* Window::native_handle() noexcept {
    return handle_;
}

const SDL_Window* Window::native_handle() const noexcept {
    return handle_;
}

Size Window::size() const noexcept {
    Size result{};

    if (handle_ != nullptr) {
        SDL_GetWindowSize(handle_, &result.width, &result.height);
    }

    return result;
}

void Window::set_title(std::string_view title) {
    if (handle_ != nullptr) {
        SDL_SetWindowTitle(handle_, std::string{title}.c_str());
    }
}

void Window::reset() noexcept {
    if (handle_ != nullptr) {
        SDL_DestroyWindow(handle_);
        handle_ = nullptr;
    }
}

} // namespace vireo
