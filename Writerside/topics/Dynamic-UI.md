# Dynamic UI

Dynamic UI means that the component tree is created or changed from application state.
Vireo currently supports two levels of dynamic behavior:

- build-time helpers that decide what gets built;
- runtime tree APIs that add and remove components after `.build()`.

## Build-Time Vs Runtime

Build-time helpers run while the UI tree is being constructed:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::When(showDebug, vireo::Text("Debug enabled"))
).build();
```

If `showDebug` changes later, this already-built tree does not automatically update.
To reflect new state, either rebuild a subtree or use runtime add/remove APIs.

Runtime mutation happens after `.build()`:

```cpp
ui->addChild(100, vireo::Text("Runtime message").build());
ui->removeChild(100);
```

See [Runtime Tree](Runtime-Tree.md) for details.

## Conditional Components With When

`When(...)` is a build-time conditional helper.
It behaves like a component builder, but it may return no component.

Include:

```cpp
#include <vireo/core/when.h>
```

The umbrella header also includes it:

```cpp
#include <vireo/vireo.hpp>
```

### Without Else

```cpp
bool showDebugInfo = true;

auto ui = vireo::Screen(960, 540)(
    vireo::Text("Main Menu"),
    vireo::When(showDebugInfo, vireo::Text("Debug mode enabled"))
).build();
```

When the condition is `true`, the child is built.
When the condition is `false`, `When(...)` returns `nullptr`, and the parent ignores it through the existing child-add
path.

### With Else

```cpp
bool loggedIn = false;

auto ui = vireo::Screen(960, 540)(
    vireo::When(
        loggedIn,
        vireo::Text("Welcome back"),
        vireo::Text("Please sign in")
    )
).build();
```

The true component and else component may be different builder types:

```cpp
vireo::When(
    isImportant,
    vireo::Text("Important"),
    vireo::Button(vireo::colors::blue)
)
```

Only the selected builder is consumed during `build()`.

## Generated Components With ForEach

`ForEach` is the current experimental direction for generated UI.
The idea is to take a collection and a factory function, then produce a stack containing one generated child per item.

Current local API shape:

```cpp
vireo::ForEach(direction, items, factory);
vireo::VForEach(items, factory);
vireo::HForEach(items, factory);
```

The experimental header is:

```cpp
#include <vireo/core/foreach.h>
```

At the current development stage, this header may not yet be included from `vireo/vireo.hpp`.
Include it directly while working on the feature.

### Why ForEach Uses A Factory

Do not pass one component and try to reuse it.
Components use `std::unique_ptr` ownership, so each generated child must be a fresh builder.

Use a lambda:

```cpp
std::vector<std::string> saves = {"Save 1", "Save 2", "Save 3"};

auto list = vireo::VForEach(saves, [](const std::string& saveName) {
    return vireo::Text(saveName);
});
```

For a horizontal generated row:

```cpp
std::vector<std::string> labels = {"Easy", "Normal", "Hard"};

auto row = vireo::HForEach(labels, [](const std::string& label) {
    return vireo::Button(vireo::colors::blue, {0, 0, 120, 42})(
        vireo::Center({0, 0, 120, 42})(
            vireo::Text(label)
        )
    );
});
```

### Direction

The generic form uses `StackDirection`:

```cpp
enum class StackDirection {
    Horizontal,
    Vertical
};
```

Example:

```cpp
auto generated = vireo::ForEach(
    vireo::StackDirection::Vertical,
    items,
    [](const auto& item) {
        return vireo::Text(item);
    }
);
```

### Intended Behavior

The intended behavior is:

1. Create a `VerticalStack` or `HorizontalStack`.
2. Loop over all items.
3. Call the factory for each item.
4. Build the returned child builder.
5. Add the child to the stack.
6. Return the built stack as one `std::unique_ptr<Component>`.

This keeps `ForEach` compatible with the existing DSL because it still returns one component: the generated stack.

## Runtime Generated UI

If the collection changes after the UI has already been built, the build-time `ForEach` result will not update by itself.
Use a runtime subtree replacement pattern:

```cpp
constexpr int SaveListId = 20;

ui->removeChild(SaveListId);
ui->addChild(SaveListId,
    vireo::VForEach(saves, [](const std::string& saveName) {
        return vireo::Text(saveName);
    }).build()
);
```

This pattern is explicit and easy to debug.

## Popup System Plan

Popups are not implemented yet, but the recommended architecture is an overlay layer.

The first version should probably provide:

1. A root overlay host.
2. A semi-transparent backdrop.
3. A popup panel drawn above normal content.
4. Event blocking so clicks do not pass through the popup.
5. Close behavior.

Background blur should come later.
SDL2 does not provide a high-level blur effect, so a blur implementation would likely need a render-to-texture step and
a custom blur pass.

## Suggested Build Order

For dynamic UI features, build in this order:

1. `When(...)` for single conditional children.
2. `VForEach(...)` and `HForEach(...)` for generated stacks.
3. Runtime subtree replacement examples.
4. Overlay host without blur.
5. Modal popup behavior.
6. Blur as a visual improvement after the architecture works.
