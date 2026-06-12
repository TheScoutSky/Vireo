# Components

Components are the visible and interactive building blocks of a Vireo UI.
Most public component types are builders that produce a concrete runtime `Component` when `.build()` is called.

## Text

`Text` renders a string:

```cpp
vireo::Text("Hello");
vireo::Text("Hello", {24, 22, 200, 40}, vireo::colors::white, 3);
```

Constructors:

```cpp
vireo::Text(
    std::string value,
    vireo::Rect rect = {24, 22, 0, 0},
    vireo::Color color = vireo::colors::white,
    int scale = 3
);

vireo::Text(
    std::string value,
    vireo::TextStyle style,
    vireo::Rect rect = {24, 22, 0, 0}
);
```

If `rect.width` or `rect.height` is `0` or negative, Vireo computes a fallback size from the text.

Text has two render paths:

- built-in pixel fallback text when no font path is set;
- SDL_ttf text when `TextStyle::fontPath` is set.

TTF example:

```cpp
vireo::TextStyle title;
title.fontPath = "/System/Library/Fonts/SFNS.ttf";
title.fontSize = 42;
title.color = vireo::colors::white;
title.fallbackScale = 4;

auto text = vireo::Text("CChess", title);
```

If the font fails to load, the component falls back to the built-in pixel text renderer.

## Button

`Button` renders a colored rectangle with hover and pressed states.
It can contain children, which is how button labels are usually added:

```cpp
vireo::Button(vireo::colors::green, {0, 0, 220, 56})(
    vireo::Center({0, 0, 220, 56})(
        vireo::Text("Start")
    )
).onClick([] {
    std::cout << "Start clicked\n";
})
```

Constructors:

```cpp
vireo::Button(vireo::Color color, vireo::Rect rect = {80, 80, 220, 72});
vireo::Button(vireo::Rect rect, vireo::ButtonStyle style);
```

Click handler:

```cpp
vireo::Button(...).onClick([] {
    // callback
});
```

The callback currently runs on left mouse button down when the pointer is inside the button.

## Checkbox

`Checkbox` renders a square box and toggles when clicked:

```cpp
vireo::Checkbox();
vireo::Checkbox(true, {0, 0, 28, 28});
```

Constructor:

```cpp
vireo::Checkbox(bool checked = false, vireo::Rect rect = {0, 0, 28, 28});
```

The runtime component exposes:

```cpp
bool checked() const noexcept;
void setChecked(bool checked);
```

The public builder currently does not expose a typed accessor after build.
When you need to read checkbox state later, keep a runtime pointer and cast carefully, or add a higher-level state API
as the framework evolves.

## Panel

`Panel` renders a filled rectangle with an optional border.
It can also contain children:

```cpp
vireo::Panel({40, 40, 360, 240}, vireo::colors::surface, vireo::colors::border)(
    vireo::Text("Settings")
)
```

Constructors:

```cpp
vireo::Panel(
    vireo::Rect rect = {0, 0, 320, 220},
    vireo::Color color = vireo::rgba(36, 40, 50),
    vireo::Color border = vireo::rgba(82, 90, 108)
);

vireo::Panel(vireo::Rect rect, vireo::PanelStyle style);
```

Use `PanelStyle` when you need border width or padding.

## Image

`Image` loads a texture through SDL_image and renders it inside a rectangle:

```cpp
vireo::Image("assets/logo.png", {0, 0, 128, 128});
vireo::Image("assets/background.png", {0, 0, 960, 540}, vireo::ImageFit::Cover);
```

Constructor:

```cpp
vireo::Image(
    std::string path,
    vireo::Rect rect = {0, 0, 96, 96},
    vireo::ImageFit fit = vireo::ImageFit::Contain
);
```

Fit modes:

| Fit | Behavior |
|-----|----------|
| `Stretch` | Stretch the image to exactly fill the component bounds. |
| `Contain` | Preserve aspect ratio and fit the whole image inside the bounds. |
| `Cover` | Preserve aspect ratio and cover the full bounds, clipping overflow. |

If loading fails, Vireo renders a placeholder rectangle with an X.

## Component Composition

Components that inherit from `ComponentBuilder` can contain children:

```cpp
vireo::Panel({0, 0, 320, 220})(
    vireo::VStack({0, 0, 320, 220}, 12)(
        vireo::Text("Title"),
        vireo::Button(vireo::colors::blue)
    )
)
```

Small leaf builders such as `Text`, `Image`, `Spacer`, and dividers do not expose nested child syntax.
