# Vireo

Vireo is a lightweight, open-source UI framework for building SDL2-based GUIs in C++.
It started as the UI foundation for the chess project "CChess", but the core idea is more general:
compose small components into a readable UI tree.

The preferred style is the builder DSL:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::Text("Hello Vireo")
).build();
```

This keeps simple screens compact while still producing a normal runtime component tree underneath.

## Current Features

| Area | Description |
|------|-------------|
| Basic components | Text, buttons, panels, images, dividers, spacers, checkboxes, and layout helpers. |
| Layout helpers | Vertical stacks, horizontal stacks, and centering helpers for common UI structures. |
| Event handling | Components receive SDL events through the root component tree. |
| Runtime tree | Components can own children, add children, remove children, and update/render recursively. |
| Styling | Shared color helpers and style objects for text, buttons, and panels. |

## Design Goals

Vireo should stay small and understandable. New features should make the DSL more expressive without hiding the fact
that the UI is a tree of `Component` objects.

The next major direction is dynamic UI composition:

- Conditional components that are only added when a state value allows it.
- Generated components from loops or collections.
- Runtime add/remove APIs that feel natural from application code.
- A popup or overlay system for modal UI, dialogs, and background dimming or blur.

See [Dynamic UI](Dynamic-UI.md) for the planned design.

## Contributing

Contributions are welcome, whether they add features, fix bugs, improve examples, or expand the documentation.

## License

Vireo is licensed under the GNU General Public License v3.0.

## Authors

- [Antonio Wilczynski](https://github.com/TheScoutSky)
