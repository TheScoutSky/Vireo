#include <vireo/platform/renderer.hpp>

#include <utility>

#include "sdl_error.hpp"

namespace vireo {

Renderer::Renderer(Window& window, const RendererOptions& options) {
    handle_ = SDL_CreateRenderer(window.native_handle(), options.driver_index, options.flags);

    if (handle_ == nullptr) {
        detail::throw_sdl_error("SDL_CreateRenderer");
    }
}

Renderer::~Renderer() {
    reset();
}

Renderer::Renderer(Renderer&& other) noexcept : handle_{std::exchange(other.handle_, nullptr)} {}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        reset();
        handle_ = std::exchange(other.handle_, nullptr);
    }

    return *this;
}

SDL_Renderer* Renderer::native_handle() noexcept {
    return handle_;
}

const SDL_Renderer* Renderer::native_handle() const noexcept {
    return handle_;
}

void Renderer::set_draw_color(Color color) {
    if (SDL_SetRenderDrawColor(handle_, color.r, color.g, color.b, color.a) != 0) {
        detail::throw_sdl_error("SDL_SetRenderDrawColor");
    }
}

void Renderer::clear(Color color) {
    set_draw_color(color);

    if (SDL_RenderClear(handle_) != 0) {
        detail::throw_sdl_error("SDL_RenderClear");
    }
}

void Renderer::present() noexcept {
    SDL_RenderPresent(handle_);
}

void Renderer::reset() noexcept {
    if (handle_ != nullptr) {
        SDL_DestroyRenderer(handle_);
        handle_ = nullptr;
    }
}

} // namespace vireo
