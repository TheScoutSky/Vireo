# Dynamic UI

Dynamic UI means that the component tree can react to application state instead of being completely fixed at startup.
Vireo already has the important foundation: every built UI becomes a normal `Component` tree, and components can own,
add, and remove children at runtime.

This page describes the target design for the next dynamic features.

## 1. Conditional Components

Goal: show a component only when a condition is true.

Vireo includes an initial `When(...)` helper for build-time conditions:

```cpp
bool showDebugInfo = true;

auto ui = vireo::Screen(960, 540)(
    vireo::Text("Main Menu"),
    vireo::When(showDebugInfo, vireo::Text("Debug mode enabled"))
).build();
```

`When(...)` can also receive an else component:

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

When the condition is false and no else component exists, no component is added. This is a build-time helper: changing
the variable later does not automatically rebuild the UI tree.

## 2. Generated Components

Goal: generate children from a list, vector, or other collection.

Today, this can also be done with normal C++:

```cpp
std::vector<std::string> saves = {"Save 1", "Save 2", "Save 3"};

auto list = vireo::VStack({0, 0, 320, 240}, 8);

for (const auto& saveName : saves) {
    list.get()->addChild(vireo::Text(saveName).build());
}

auto ui = vireo::Screen(960, 540)(
    std::move(list)
).build();
```

Planned DSL direction:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::ForEach(saves, [](const std::string& saveName) {
        return vireo::Text(saveName);
    })
).build();
```

The main challenge is ownership. Generated components should still end up as `std::unique_ptr<Component>` objects owned
by their parent, just like manually written children.

## 3. Adding And Removing Components Later

Goal: allow the application to change the UI after it has already been built.

The current runtime tree already supports this:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::Text("Ready")
).build();

ui->addChild(100, vireo::Text("Runtime message").build());
ui->removeChild(100);
```

The planned improvement is not the storage itself, but the developer experience:

- Clear names for important child IDs.
- A safe way to keep references to components that may later be removed.
- Dirty/layout invalidation rules after a child is added or removed.
- Examples that show when to rebuild a subtree and when to mutate the existing tree.

## 4. Popup System

Goal: show modal UI above the normal screen, with optional background dimming or blur.

A good popup system should probably be built as an overlay layer, not as a special case inside every component.

Possible shape:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::OverlayHost()(
        vireo::Text("Game screen")
    )
).build();

// Later:
popupHost->show(
    vireo::Popup({280, 140, 400, 260})(
        vireo::Text("Paused"),
        vireo::Button(vireo::colors::blue, {0, 0, 160, 48})
    )
);
```

Recommended implementation steps:

1. Start with a modal overlay that draws a semi-transparent background.
2. Add click blocking so events do not pass through the popup.
3. Add popup positioning and close behavior.
4. Add background blur after the overlay architecture works.

Blur is the hardest part because SDL2 does not provide a high-level blur effect by itself. The first version can use a
dark translucent backdrop; later versions can render the background into a texture and apply a simple blur pass.

## Suggested Learning Path

Build these features in this order:

1. Use normal C++ `if` statements and loops with `builder.get()->addChild(...)`.
2. Wrap the repeated patterns into small helpers such as `When` and `ForEach`.
3. Improve runtime add/remove ergonomics and document ownership rules.
4. Build `OverlayHost` and `Popup` as regular components.
5. Add dimming first, then blur.

This keeps each step understandable and makes it easier to test one idea at a time.
