# Runtime Tree

After the builder DSL has produced a `std::unique_ptr<Component>`, the UI becomes a normal runtime tree.
This tree can be queried and changed after startup.

## Add Children

Add a child with an automatically assigned ID:

```cpp
auto ui = vireo::Screen(960, 540).build();

vireo::Component* label = ui->addChild(
    vireo::Text("Runtime child").build()
);
```

Add a child with an explicit ID:

```cpp
constexpr int StatusTextId = 100;

ui->addChild(StatusTextId, vireo::Text("Ready").build());
```

If a child already exists for the same ID, it is removed before the new one is inserted.

## Remove Children

Remove by ID:

```cpp
ui->removeChild(StatusTextId);
```

Remove by pointer:

```cpp
ui->removeChild(label);
```

Remove every child:

```cpp
ui->removeAllChildren();
```

When an owned child is removed, the owning `std::unique_ptr` is erased and the child is destroyed.

## Query Children

Get all direct children:

```cpp
std::vector<vireo::Component*> children = ui->getChildren();
```

Get one child by ID:

```cpp
vireo::Component* status = ui->getChild(StatusTextId);
```

Get a component's parent:

```cpp
vireo::Component* parent = status->getParent();
```

The returned pointers are non-owning. Do not keep them after the component has been removed.

## Runtime Mutation Pattern

For simple dynamic screens, replacing a subtree is often easier than editing many small children:

```cpp
constexpr int MenuContentId = 10;

ui->removeChild(MenuContentId);
ui->addChild(MenuContentId,
    vireo::VStack({0, 0, 320, 300}, 12)(
        vireo::Text("New menu"),
        vireo::Button(vireo::colors::green)
    ).build()
);
```

This pattern works well for menus, popup content, status panels, and screens that are rebuilt from application state.

## Layout And Mutation

Layout components such as `VStack`, `HStack`, and `Center` lay out their children during `render(...)`.
If children are added or removed before rendering, the next render pass will position them according to the current
layout rules.

`setPosition`, `setSize`, `setBounds`, and `setPadding` call `markDirty()`.
Direct child insertion and removal currently update the tree structure but do not expose a separate layout invalidation
API.

## Event Flow

Calling:

```cpp
ui->handleEvents(&event);
```

does this:

1. The current component handles the event through `handleSelf(...)`.
2. Each child receives the same event recursively.

There is currently no event capture, bubbling cancellation, focus manager, or modal event blocking built into the base
tree. Popup and overlay work should add those concepts deliberately instead of hiding them inside every component.
