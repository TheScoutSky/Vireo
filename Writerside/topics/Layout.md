# Layout

Vireo's layout system is intentionally small.
Layout components are normal components that reposition their direct children during rendering.

The current layout helpers are:

- `Screen`
- `Center`
- `VerticalStack` / `VStack`
- `HorizontalStack` / `HStack`
- `Spacer`
- `HorizontalDivider`
- `VerticalDivider`

## Screen

`Screen` is usually the root of a UI tree:

```cpp
auto ui = vireo::Screen()(
    vireo::Text("Hello")
).build();
```

Constructor:

```cpp
vireo::Screen();
vireo::Screen(int width, int height);
```

`Screen` itself does not draw anything. When rendered, it reads the renderer output size and updates itself to fill the
current window. It also reacts to SDL window resize events. This keeps the root bounds correct when the window is resized
or switched to fullscreen.

Use `Screen(width, height)` when you want an initial size before the first render pass. Use `Screen()` when the renderer
size should fully drive the root size.

## Fill Sizing

Some layout components can be constructed with `vireo::Fill`:

```cpp
vireo::Center(vireo::Fill)(...)
vireo::VStack(vireo::Fill, 16, 0, vireo::Alignment::Center, vireo::Justify::Center)(...)
vireo::HStack(vireo::Fill)(...)
```

`Fill` means: fill the parent component's content rectangle.

Every `ComponentBuilder` also has a `.fill()` shortcut:

```cpp
vireo::Panel({0, 0, 0, 0}).fill()(
    vireo::Text("Fills parent content")
)
```

For fullscreen-safe screens, the usual pattern is:

```cpp
auto ui = vireo::Screen()(
    vireo::Center(vireo::Fill)(
        vireo::VStack(vireo::Fill, 16, 0,
                      vireo::Alignment::Center,
                      vireo::Justify::Center)(
            vireo::Text("Resizable"),
            vireo::Button(vireo::colors::green)
        )
    )
).build();
```

Fixed rectangles still work, but they stay fixed. If a centered menu should remain centered after resizing, the
containers around it should usually use `Fill`.

## Center

`Center` positions each direct child in the center of its bounds:

```cpp
vireo::Center({0, 0, 960, 540})(
    vireo::Text("Centered")
)
```

Constructor:

```cpp
vireo::Center(vireo::Rect rect = {0, 0, 320, 220});
vireo::Center(vireo::Fill);
```

During render, `Center` reads each child size and sets the child's position to:

```text
((parentWidth - childWidth) / 2, (parentHeight - childHeight) / 2)
```

If multiple children are added to a `Center`, each one is centered individually.

## Vertical Stack

`VerticalStack` lays out children from top to bottom.
`VStack` is an alias:

```cpp
vireo::VStack({0, 0, 320, 300}, 12, 16,
              vireo::Alignment::Center,
              vireo::Justify::Center)(
    vireo::Text("Title"),
    vireo::Button(vireo::colors::green),
    vireo::Button(vireo::colors::blue)
)
```

Constructor:

```cpp
vireo::VerticalStack(
    vireo::Rect rect = {0, 0, 320, 300},
    int gap = 12,
    int padding = 0,
    vireo::Alignment alignment = vireo::Alignment::Start,
    vireo::Justify justify = vireo::Justify::Start
);

vireo::VerticalStack(
    vireo::Fill,
    int gap = 12,
    int padding = 0,
    vireo::Alignment alignment = vireo::Alignment::Start,
    vireo::Justify justify = vireo::Justify::Start
);
```

Vertical meaning:

| Parameter | Meaning |
|-----------|---------|
| `rect` | Outer bounds of the stack. |
| `gap` | Space between children unless `Justify::SpaceBetween` computes the gap. |
| `padding` | Equal padding on all sides. |
| `alignment` | Cross-axis placement, so horizontal placement in a vertical stack. |
| `justify` | Main-axis placement, so vertical placement in a vertical stack. |

`Alignment::Stretch` changes each child's width to the available stack width.

## Horizontal Stack

`HorizontalStack` lays out children from left to right.
`HStack` is an alias:

```cpp
vireo::HStack({0, 0, 500, 80}, 16, 12,
              vireo::Alignment::Center,
              vireo::Justify::SpaceBetween)(
    vireo::Text("Hardcore"),
    vireo::Checkbox(),
    vireo::Text("Test Mode"),
    vireo::Checkbox()
)
```

Constructor:

```cpp
vireo::HorizontalStack(
    vireo::Rect rect = {0, 0, 400, 80},
    int gap = 12,
    int padding = 0,
    vireo::Alignment alignment = vireo::Alignment::Start,
    vireo::Justify justify = vireo::Justify::Start
);

vireo::HorizontalStack(
    vireo::Fill,
    int gap = 12,
    int padding = 0,
    vireo::Alignment alignment = vireo::Alignment::Start,
    vireo::Justify justify = vireo::Justify::Start
);
```

Horizontal meaning:

| Parameter | Meaning |
|-----------|---------|
| `rect` | Outer bounds of the stack. |
| `gap` | Space between children unless `Justify::SpaceBetween` computes the gap. |
| `padding` | Equal padding on all sides. |
| `alignment` | Cross-axis placement, so vertical placement in a horizontal stack. |
| `justify` | Main-axis placement, so horizontal placement in a horizontal stack. |

`Alignment::Stretch` changes each child's height to the available stack height.

## Alignment

`Alignment` controls the cross axis:

```cpp
enum class Alignment {
    Start,
    Center,
    End,
    Stretch,
};
```

In a `VStack`, this means horizontal alignment.
In an `HStack`, this means vertical alignment.

## Justify

`Justify` controls the main axis:

```cpp
enum class Justify {
    Start,
    Center,
    End,
    SpaceBetween,
};
```

`SpaceBetween` computes the gap so the first child is at the start, the last child is at the end, and remaining children
are distributed between them.

## Spacer

`Spacer` occupies space without rendering anything:

```cpp
vireo::Spacer();
vireo::Spacer(24, 16);
```

Constructor:

```cpp
vireo::Spacer(int width = 16, int height = 16);
```

Use it inside stacks when you need a fixed blank area.

## Dividers

Dividers render filled rectangles:

```cpp
vireo::HorizontalDivider();
vireo::HorizontalDivider(300, vireo::colors::border);

vireo::VerticalDivider();
vireo::VerticalDivider(80, vireo::colors::border);
```

Constructors:

```cpp
vireo::HorizontalDivider(int width = 240, vireo::Color color = vireo::rgba(82, 90, 108));
vireo::VerticalDivider(int height = 80, vireo::Color color = vireo::rgba(82, 90, 108));
```

`HorizontalDivider` has a height of `1`.
`VerticalDivider` has a width of `1`.

## Practical Layout Pattern

A common pattern is:

```cpp
vireo::Screen()(
    vireo::Center(vireo::Fill)(
        vireo::VStack(vireo::Fill, 16, 0,
                      vireo::Alignment::Center,
                      vireo::Justify::Center)(
            vireo::Text("CChess"),
            vireo::HorizontalDivider(),
            vireo::Button(vireo::colors::green)
        )
    )
)
```

That gives you a root screen, a centered layout area, and a vertical menu that stays centered after window resizing or
fullscreen changes.
