#include <vireo/platform/sdl_context.hpp>

#include <SDL_ttf.h>

#include <utility>

#include "sdl_error.hpp"

namespace vireo {

SdlContext::SdlContext(Uint32 flags) {
    if (SDL_InitSubSystem(flags) != 0) {
        detail::throw_sdl_error("SDL_InitSubSystem");
    }

    if (TTF_WasInit() == 0) {
        if (TTF_Init() != 0) {
            SDL_QuitSubSystem(flags);
            detail::throw_sdl_error("TTF_Init");
        }
        owns_ttf_ = true;
    }

    flags_ = flags;
    owns_ = true;
}

SdlContext::~SdlContext() {
    reset();
}

SdlContext::SdlContext(SdlContext&& other) noexcept
    : flags_{std::exchange(other.flags_, 0)}, owns_{std::exchange(other.owns_, false)},
      owns_ttf_{std::exchange(other.owns_ttf_, false)} {}

SdlContext& SdlContext::operator=(SdlContext&& other) noexcept {
    if (this != &other) {
        reset();
        flags_ = std::exchange(other.flags_, 0);
        owns_ = std::exchange(other.owns_, false);
        owns_ttf_ = std::exchange(other.owns_ttf_, false);
    }

    return *this;
}

bool SdlContext::valid() const noexcept {
    return owns_;
}

Uint32 SdlContext::flags() const noexcept {
    return flags_;
}

void SdlContext::reset() noexcept {
    if (owns_) {
        if (owns_ttf_) {
            TTF_Quit();
            owns_ttf_ = false;
        }
        SDL_QuitSubSystem(flags_);
        flags_ = 0;
        owns_ = false;
    }
}

} // namespace vireo
