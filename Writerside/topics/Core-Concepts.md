# Core Concepts

Vireo has two layers that work together:

- **Builders** create components with a compact DSL.
- **Components** are the runtime tree that receives events, updates, and rendering.

Understanding the difference between those two layers makes the rest of the framework much easier to reason about.

## Component

`Component` is the runtime base class.
Every visible or layout-related object eventually becomes a `Component`.

Runtime responsibilities:

- store a base rectangle and a content rectangle;
- hold child components;
- know its parent;
- handle SDL events recursively;
- update recursively;
- render recursively;
- track dirty state;
- expose bounds and position helpers.

Important methods:

```cpp
void handleEvents(SDL_Event* event);
void update(float deltaTime);
void render(SDL_Renderer* renderer);

Component* addChild(std::unique_ptr<Component> child);
Component* addChild(int id, std::unique_ptr<Component> child);
void removeChild(Component* child);
void removeChild(int id);
void removeAllChildren();

std::vector<Component*> getChildren();
Component* getChild(int id);
Component* getParent() const;

void setPosition(int x, int y);
void setSize(int width, int height);
void setBounds(const SDL_Rect& rect);
void setPadding(vireo::EdgeInsets padding);
void setFillParent(bool fill = true);
```

`Component` is abstract. Concrete components implement:

```cpp
void handleSelf(SDL_Event* event) override;
void updateSelf(float deltaTime) override;
void renderSelf(SDL_Renderer* renderer) override;
```

## Builder

Most public UI classes are builders, not components.
For example, `Text`, `Button`, `VStack`, and `Screen` are builder types.

A builder owns a concrete component internally and exposes:

```cpp
std::unique_ptr<Component> build() &&;
Component* get() const noexcept;
```

`build()` consumes the builder and gives ownership of the built component to the caller.
That is why Vireo examples often use temporaries:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::Text("Hello")
).build();
```

## Nested DSL

`ComponentBuilder` provides the nested call syntax:

```cpp
Parent(...)(child1, child2, child3)
```

The outer `(...)` constructs the parent builder.
The second `(...)` adds children to that builder.

Example:

```cpp
auto ui = vireo::Screen(960, 540)(
    vireo::Panel({100, 80, 320, 220})(
        vireo::Text("Settings")
    )
).build();
```

Internally, each child builder is consumed through `.build()` and added to the parent's runtime `Component`.

## Ownership

Vireo uses `std::unique_ptr<Component>` for ownership.
That means a component belongs to exactly one parent.

This is important:

```cpp
auto label = vireo::Text("Hello");
```

`label` cannot be copied into multiple parents because it owns a component.
If you need many similar components, create a new builder for each one:

```cpp
vireo::Text("One");
vireo::Text("Two");
vireo::Text("Three");
```

Generated UI helpers such as `ForEach` use a factory function for exactly this reason:
each item must create a fresh builder.

## Bounds

Vireo uses simple integer geometry types:

```cpp
vireo::Point point{10, 20};
vireo::Size size{960, 540};
vireo::Rect rect{0, 0, 320, 200};
```

`Rect` maps to:

```cpp
x, y, width, height
```

Components store an SDL rectangle internally. The public `Rect` type is used by constructors to keep application code
small and readable.

## Base Rect And Content Rect

Each component has two rectangles:

| Rect | Meaning |
|------|---------|
| Base rect | The component's outer bounds. |
| Content rect | The inner bounds after margin and padding values are applied. |

Use:

```cpp
const SDL_Rect& base = component->getBaseRect();
const SDL_Rect& content = component->getContentRect();
```

Absolute versions include all parent offsets:

```cpp
SDL_Rect absoluteBase = component->getAbsoluteBaseRect();
SDL_Rect absoluteContent = component->getAbsoluteContentRect();
```

Rendering and hit testing usually use absolute rectangles.

## Fill Layout

Components can be marked as filling their parent content rectangle:

```cpp
component->setFillParent();
```

For builder-based code, use either a `Fill` constructor:

```cpp
vireo::Center(vireo::Fill)(...)
vireo::VStack(vireo::Fill)(...)
```

or the generic builder shortcut:

```cpp
vireo::Panel({0, 0, 0, 0}).fill()(...)
```

During rendering, a fill component resolves its local bounds from its parent's content rectangle. This is what allows
layouts to keep using the full window after a resize or fullscreen change.

## Dirty State

Components can be marked dirty:

```cpp
component->markDirty();
bool dirty = component->isDirty();
component->clearDirty();
```

When a component is marked dirty, the dirty state propagates to its parent.
This is useful for later optimizations or layout invalidation.

Current rendering still renders the tree directly each frame.
