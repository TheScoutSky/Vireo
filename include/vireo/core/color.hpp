#pragma once

#include <cstdint>

namespace vireo {

struct Color {
    std::uint8_t r{255};
    std::uint8_t g{255};
    std::uint8_t b{255};
    std::uint8_t a{255};

    [[nodiscard]] static constexpr Color green() noexcept {
        return Color{62, 190, 96, 255};
    }

    [[nodiscard]] static constexpr Color white() noexcept {
        return Color{245, 247, 250, 255};
    }

    [[nodiscard]] static constexpr Color black() noexcept {
        return Color{15, 18, 24, 255};
    }
};

[[nodiscard]] constexpr Color rgba(std::uint8_t red, std::uint8_t green, std::uint8_t blue,
                                   std::uint8_t alpha = 255) noexcept {
    return Color{red, green, blue, alpha};
}

namespace colors {

inline constexpr Color green{62, 190, 96, 255};
inline constexpr Color white{245, 247, 250, 255};
inline constexpr Color black{15, 18, 24, 255};
inline constexpr Color slate{24, 27, 34, 255};
inline constexpr Color red{255, 0, 0, 255};
inline constexpr Color blue{0, 0, 255, 255};


} // namespace colors

} // namespace vireo
