# Styling

Vireo styling is currently based on small value objects.
There is no global theme system yet; instead, components receive colors and style structs directly.

## Color

`Color` stores RGBA channels as bytes:

```cpp
struct Color {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
};
```

Create colors with helpers:

```cpp
vireo::rgb(255, 120, 40);
vireo::rgba(255, 120, 40, 180);
vireo::hex(0x3B82F6);
vireo::hex(0x3B82F6, 128);
vireo::withAlpha(vireo::colors::blue, 120);
```

## Named Colors

Vireo provides named colors in `vireo::colors`:

```cpp
vireo::colors::white;
vireo::colors::black;
vireo::colors::gray;
vireo::colors::slate;
vireo::colors::surface;
vireo::colors::border;
```

Semantic colors:

```cpp
vireo::colors::success;
vireo::colors::warning;
vireo::colors::danger;
vireo::colors::info;
```

Accent colors:

```cpp
vireo::colors::red;
vireo::colors::blue;
vireo::colors::green;
vireo::colors::purple;
vireo::colors::orange;
vireo::colors::cyan;
```

Text colors:

```cpp
vireo::colors::textPrimary;
vireo::colors::textSecondary;
vireo::colors::textMuted;
```

## Edge Insets

`EdgeInsets` represents four-sided spacing:

```cpp
vireo::EdgeInsets padding{8, 12, 8, 12};
vireo::EdgeInsets all = vireo::EdgeInsets::all(12);
vireo::EdgeInsets symmetric = vireo::EdgeInsets::symmetric(16, 8);
```

Order:

```text
left, top, right, bottom
```

## Text Style

`TextStyle` controls text color and font rendering:

```cpp
vireo::TextStyle style;
style.color = vireo::colors::white;
style.fontPath = "/System/Library/Fonts/SFNS.ttf";
style.fontSize = 32;
style.fallbackScale = 3;

auto label = vireo::Text("Hello", style);
```

Fields:

| Field | Meaning |
|-------|---------|
| `color` | Text color. |
| `fontPath` | Path to a TTF font. Empty means use the built-in fallback renderer. |
| `fontSize` | SDL_ttf font size. |
| `fallbackScale` | Pixel size multiplier for the fallback renderer. |

## Button Style

`ButtonStyle` controls button colors, border, and padding:

```cpp
vireo::ButtonStyle primary;
primary.background = vireo::colors::green;
primary.hover = vireo::rgba(88, 218, 124);
primary.pressed = vireo::rgba(44, 154, 78);
primary.border = vireo::rgba(13, 16, 22);
primary.borderWidth = 2;
primary.padding = vireo::EdgeInsets::all(8);

auto button = vireo::Button({0, 0, 220, 56}, primary);
```

Shortcut helper:

```cpp
auto style = vireo::buttonStyle(
    vireo::colors::green,
    vireo::rgba(88, 218, 124),
    vireo::rgba(44, 154, 78)
);
```

## Panel Style

`PanelStyle` controls panel background, border, and padding:

```cpp
vireo::PanelStyle card;
card.background = vireo::colors::surface;
card.border = vireo::colors::border;
card.borderWidth = 2;
card.padding = vireo::EdgeInsets::all(16);

auto panel = vireo::Panel({0, 0, 360, 240}, card);
```

## Layout Styling

Layout components use `Alignment` and `Justify`:

```cpp
vireo::VStack({0, 0, 320, 300}, 12, 16,
              vireo::Alignment::Center,
              vireo::Justify::Center)
```

See [Layout](Layout.md) for the exact axis behavior.

## Current Limitations

Vireo does not yet have:

- global theme objects;
- CSS-like cascading;
- rounded corners;
- shadows;
- animation styling;
- automatic dark/light mode switching.

For now, styles are explicit C++ values.
