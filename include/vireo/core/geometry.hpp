#pragma once

namespace vireo {

struct Point {
    int x{0};
    int y{0};
};

struct Size {
    int width{0};
    int height{0};
};

struct Rect {
    int x{0};
    int y{0};
    int width{0};
    int height{0};
};

struct FillTag {};

inline constexpr FillTag Fill{};

} // namespace vireo
