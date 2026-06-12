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

    [[nodiscard]] static constexpr Color red() noexcept {
        return Color{239, 68, 68, 255};
    }

    [[nodiscard]] static constexpr Color blue() noexcept {
        return Color{59, 130, 246, 255};
    }

    [[nodiscard]] static constexpr Color transparent() noexcept {
        return Color{0, 0, 0, 0};
    }
};

[[nodiscard]] constexpr Color rgb(std::uint8_t red, std::uint8_t green, std::uint8_t blue) noexcept {
    return Color{red, green, blue, 255};
}

[[nodiscard]] constexpr Color rgba(std::uint8_t red, std::uint8_t green, std::uint8_t blue,
                                   std::uint8_t alpha = 255) noexcept {
    return Color{red, green, blue, alpha};
}

[[nodiscard]] constexpr Color hex(std::uint32_t value, std::uint8_t alpha = 255) noexcept {
    return Color{
        static_cast<std::uint8_t>((value >> 16) & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF),
        static_cast<std::uint8_t>(value & 0xFF),
        alpha,
    };
}

[[nodiscard]] constexpr Color withAlpha(Color color, std::uint8_t alpha) noexcept {
    return Color{color.r, color.g, color.b, alpha};
}

namespace colors {

inline constexpr Color transparent{0, 0, 0, 0};

inline constexpr Color white{245, 247, 250, 255};
inline constexpr Color black{15, 18, 24, 255};
inline constexpr Color gray{107, 114, 128, 255};
inline constexpr Color grey{107, 114, 128, 255};
inline constexpr Color lightGray{209, 213, 219, 255};
inline constexpr Color lightGrey{209, 213, 219, 255};
inline constexpr Color darkGray{55, 65, 81, 255};
inline constexpr Color darkGrey{55, 65, 81, 255};

inline constexpr Color slate{24, 27, 34, 255};
inline constexpr Color charcoal{31, 35, 45, 255};
inline constexpr Color surface{34, 39, 49, 255};
inline constexpr Color surfaceLight{45, 52, 66, 255};
inline constexpr Color border{94, 108, 132, 255};
inline constexpr Color textPrimary{245, 247, 250, 255};
inline constexpr Color textSecondary{170, 178, 196, 255};
inline constexpr Color textMuted{119, 130, 153, 255};

inline constexpr Color red{239, 68, 68, 255};
inline constexpr Color crimson{220, 38, 38, 255};
inline constexpr Color rose{244, 63, 94, 255};
inline constexpr Color pink{236, 72, 153, 255};
inline constexpr Color magenta{217, 70, 239, 255};
inline constexpr Color purple{168, 85, 247, 255};
inline constexpr Color violet{139, 92, 246, 255};
inline constexpr Color indigo{99, 102, 241, 255};
inline constexpr Color blue{59, 130, 246, 255};
inline constexpr Color sky{14, 165, 233, 255};
inline constexpr Color cyan{6, 182, 212, 255};
inline constexpr Color teal{20, 184, 166, 255};
inline constexpr Color emerald{16, 185, 129, 255};
inline constexpr Color green{62, 190, 96, 255};
inline constexpr Color lime{132, 204, 22, 255};
inline constexpr Color yellow{234, 179, 8, 255};
inline constexpr Color amber{245, 158, 11, 255};
inline constexpr Color orange{249, 115, 22, 255};
inline constexpr Color brown{120, 83, 55, 255};

inline constexpr Color success{62, 190, 96, 255};
inline constexpr Color warning{245, 158, 11, 255};
inline constexpr Color danger{239, 68, 68, 255};
inline constexpr Color info{59, 130, 246, 255};

inline constexpr Color redLight{254, 202, 202, 255};
inline constexpr Color orangeLight{254, 215, 170, 255};
inline constexpr Color yellowLight{254, 240, 138, 255};
inline constexpr Color greenLight{187, 247, 208, 255};
inline constexpr Color blueLight{191, 219, 254, 255};
inline constexpr Color purpleLight{221, 214, 254, 255};

inline constexpr Color redDark{127, 29, 29, 255};
inline constexpr Color orangeDark{124, 45, 18, 255};
inline constexpr Color yellowDark{113, 63, 18, 255};
inline constexpr Color greenDark{20, 83, 45, 255};
inline constexpr Color blueDark{30, 58, 138, 255};
inline constexpr Color purpleDark{88, 28, 135, 255};

} // namespace colors

} // namespace vireo
