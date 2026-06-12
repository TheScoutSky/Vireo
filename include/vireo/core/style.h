#ifndef VIREO_STYLE_H
#define VIREO_STYLE_H

#include <string>

#include <vireo/core/color.hpp>

namespace vireo {

enum class Alignment {
    Start,
    Center,
    End,
    Stretch,
};

enum class Justify {
    Start,
    Center,
    End,
    SpaceBetween,
};

enum class ImageFit {
    Stretch,
    Contain,
    Cover,
};

struct EdgeInsets {
    int left{0};
    int top{0};
    int right{0};
    int bottom{0};

    [[nodiscard]] static constexpr EdgeInsets all(int value) noexcept {
        return EdgeInsets{value, value, value, value};
    }

    [[nodiscard]] static constexpr EdgeInsets symmetric(int horizontal, int vertical) noexcept {
        return EdgeInsets{horizontal, vertical, horizontal, vertical};
    }
};

struct PanelStyle {
    Color background{rgba(36, 40, 50)};
    Color border{rgba(82, 90, 108)};
    int borderWidth{1};
    EdgeInsets padding{EdgeInsets::all(0)};
};

struct ButtonStyle {
    Color background{colors::green};
    Color hover{rgba(82, 210, 116)};
    Color pressed{rgba(42, 160, 76)};
    Color border{rgba(13, 16, 22)};
    int borderWidth{1};
    EdgeInsets padding{EdgeInsets::all(0)};
};

struct TextStyle {
    Color color{colors::white};
    std::string fontPath{};
    int fontSize{24};
    int fallbackScale{3};
};

[[nodiscard]] constexpr ButtonStyle buttonStyle(Color background, Color hover, Color pressed,
                                                Color border = rgba(13, 16, 22), int borderWidth = 1) noexcept {
    return ButtonStyle{background, hover, pressed, border, borderWidth, EdgeInsets::all(0)};
}

} // namespace vireo

#endif // VIREO_STYLE_H
