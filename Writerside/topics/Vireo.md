# Vireo

Vireo is a small SDL2-based UI framework for C++ applications.
It is built around one central idea: describe the UI as a tree of components, then let that tree receive events, update,
and render every frame.

The public API is intentionally close to normal C++:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::Center({0, 0, 960, 540})(
        vireo::Text("Hello Vireo")
    )
).build();
```

The DSL builds regular `Component` objects underneath. After `.build()`, the result is a
`std::unique_ptr<vireo::Component>` that can be rendered, updated, and changed at runtime.

## What Vireo Provides

| Area | Features |
|------|----------|
| Platform wrappers | RAII wrappers for SDL initialization, windows, and renderers. |
| Core tree | Parent/child component ownership, recursive event handling, update, and render flow. |
| Builder DSL | Nested component construction with `Screen(...)(child1, child2).build()`. |
| Layout | `Screen`, `Center`, `VStack`, `HStack`, `Spacer`, and dividers. |
| Components | `Text`, `Button`, `Checkbox`, `Panel`, and `Image`. |
| Styling | RGBA colors, named colors, semantic colors, `TextStyle`, `ButtonStyle`, and `PanelStyle`. |
| Dynamic helpers | `When(...)` for conditional build-time components and `ForEach` helpers for generated stacks. |
| Utilities | Version constants, library metadata helpers, export macros, and the `Widget` interface. |
| Publishing | CMake install/export support and Writerside documentation publishing through GitHub Pages. |

## Recommended Reading Order

1. [Getting Started](Getting-Started.md) for the smallest working app shape.
2. [Project Setup](Project-Setup.md) for CMake, dependencies, examples, and tests.
3. [Application Lifecycle](Application-Lifecycle.md) for SDL setup, event loops, and rendering.
4. [Core Concepts](Core-Concepts.md) for the component tree and builder model.
5. [Layout](Layout.md), [Components](Components.md), and [Styling](Styling.md) for the practical UI API.
6. [Dynamic UI](Dynamic-UI.md) and [Runtime Tree](Runtime-Tree.md) for state-driven UI work.
7. [Utilities](Utilities.md) for version, metadata, and helper interfaces.
8. [Publishing](Publishing.md) for releases, CMake installation, and hosted docs.

## Current Status

Vireo is still early and intentionally small. The strongest parts today are the tree model, simple layout, basic
interactive components, and readable DSL composition.

Some APIs are still early and may change as the framework grows. The current focus is improving dynamic layout,
generated components, and popup support without making the core tree model harder to understand.
