#pragma once

#include <SDL.h>

#include <stdexcept>
#include <string>
#include <string_view>

namespace vireo::detail {

[[noreturn]] inline void throw_sdl_error(std::string_view operation) {
    std::string message{operation};
    message += " failed";

    const char* error = SDL_GetError();
    if (error != nullptr && error[0] != '\0') {
        message += ": ";
        message += error;
    }

    throw std::runtime_error(message);
}

} // namespace vireo::detail
