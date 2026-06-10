#pragma once

#include <SDL.h>

#include <vireo/core/export.hpp>

namespace vireo {

class VIREO_API SdlContext final {
  public:
    explicit SdlContext(Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    ~SdlContext();

    SdlContext(const SdlContext&) = delete;
    SdlContext& operator=(const SdlContext&) = delete;

    SdlContext(SdlContext&& other) noexcept;
    SdlContext& operator=(SdlContext&& other) noexcept;

    [[nodiscard]] bool valid() const noexcept;
    [[nodiscard]] Uint32 flags() const noexcept;

  private:
    void reset() noexcept;

    Uint32 flags_{0};
    bool owns_{false};
};

} // namespace vireo
