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

`ForEach` generates UI from a collection.
It takes a collection and a factory function, then produces a stack containing one generated child per item.

API shape:

```cpp
vireo::ForEach(direction, items, factory);
vireo::VForEach(items, factory);
vireo::HForEach(items, factory);
```

The umbrella header includes it:

```cpp
#include <vireo/vireo.hpp>
```

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

When no rectangle is passed, `ForEach` measures the generated children and gives the generated stack a content-sized
rectangle. This prevents generated rows from sitting inside an arbitrary default stack size.

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

### Size Options

Default: auto-size to generated content.

```cpp
auto row = vireo::HForEach(labels, [](const std::string& label) {
    return vireo::Text(label);
});
```

Fixed rectangle:

```cpp
auto row = vireo::HForEach({0, 0, 500, 80}, labels, [](const std::string& label) {
    return vireo::Text(label);
});
```

Fill the parent content rectangle:

```cpp
auto row = vireo::HForEach(vireo::Fill, labels, [](const std::string& label) {
    return vireo::Text(label);
});
```

All variants also accept stack layout options after the factory:

```cpp
auto row = vireo::HForEach(
    labels,
    [](const std::string& label) {
        return vireo::Text(label);
    },
    16,
    8,
    vireo::Alignment::Center,
    vireo::Justify::Center
);
```

Those trailing values are:

```text
gap, padding, alignment, justify
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

1. Build one child for every item.
2. Measure the generated children when no explicit size is provided.
3. Create a `VerticalStack` or `HorizontalStack`.
4. Add the generated children to the stack.
5. Return the built stack as one `std::unique_ptr<Component>`.

This keeps `ForEach` compatible with the existing DSL because it still returns one component: the generated stack.

## Resize-Safe Dynamic Layout

Use `Screen()` and `Fill` containers when generated or conditional content should remain centered after resizing:

```cpp
auto ui = vireo::Screen()(
    vireo::Center(vireo::Fill)(
        vireo::VStack(vireo::Fill, 16, 0,
                      vireo::Alignment::Center,
                      vireo::Justify::Center)(
            vireo::Text("Menu"),
            vireo::HForEach(items, [](const std::string& item) {
                return vireo::Text(item);
            })
        )
    )
).build();
```

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
