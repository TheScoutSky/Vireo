# Vireo

Vireo is a lightweight SDL2 UI framework foundation.

## Dependencies

Vireo links these SDL2 packages publicly, so applications using `vireo::vireo` get the same dependencies through CMake:

- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer
- SDL2_net

## Build

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## TTF Text

`Text` uses the built-in debug pixel font by default. Set a `TextStyle::fontPath` to render through SDL_ttf:

```cpp
vireo::TextStyle title;
title.fontPath = "/System/Library/Fonts/SFNS.ttf";
title.fontSize = 42;
title.color = vireo::colors::white;

auto label = vireo::Text("Hello TTF", title);
```

## Colors

Vireo includes named colors, semantic UI colors, and helpers:

```cpp
vireo::colors::green;
vireo::colors::danger;
vireo::colors::surface;
vireo::colors::textSecondary;

vireo::rgb(255, 120, 40);
vireo::rgba(255, 120, 40, 180);
vireo::hex(0x3B82F6);
vireo::withAlpha(vireo::colors::blue, 120);
```
