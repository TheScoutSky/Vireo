# Utilities

Vireo also exposes a few small utility APIs that are not visual components.

## Library Metadata

`library.hpp` exposes the library name and version as functions:

```cpp
#include <vireo/core/library.hpp>

std::string_view libraryName = vireo::name();
std::string_view libraryVersion = vireo::version();
```

Current behavior:

```cpp
vireo::name();    // "vireo"
vireo::version(); // VIREO_VERSION_STRING
```

## Version Constants

`version.hpp` exposes preprocessor macros and inline C++ constants:

```cpp
#include <vireo/version.hpp>

VIREO_VERSION_MAJOR
VIREO_VERSION_MINOR
VIREO_VERSION_PATCH
VIREO_VERSION_STRING
```

Inline constants:

```cpp
vireo::version_major;
vireo::version_minor;
vireo::version_patch;
vireo::version_string;
```

For the current version:

```text
0.0.5
```

Use these when printing diagnostics, checking compatibility, or showing framework metadata in an about screen.

## Export Macro

`export.hpp` defines `VIREO_API`.

On Windows shared-library builds:

- `VIREO_BUILDING_LIBRARY` exports symbols while building Vireo;
- consumers import symbols;
- non-Windows builds currently leave `VIREO_API` empty.

Most applications do not need to use this macro directly.
It is mainly for public Vireo declarations.

## Widget Interface

`Widget` is a small interface in `vireo/ui/widget.hpp`:

```cpp
class Widget {
  public:
    virtual ~Widget() = default;

    virtual bool handle_event(const SDL_Event& event);
    virtual void update(double delta_seconds);
    virtual void render(Renderer& renderer) = 0;
};
```

Default behavior:

```cpp
bool handle_event(const SDL_Event&) {
    return false;
}

void update(double) {}
```

`render(...)` is pure virtual.

The current component DSL is based on `Component`, not `Widget`.
Treat `Widget` as a lower-level or alternative interface while the framework evolves.

## Umbrella Header

Most application code should include:

```cpp
#include <vireo/vireo.hpp>
```

It currently includes:

- platform wrappers;
- core components;
- layout helpers;
- style and color helpers;
- library metadata;
- version information;
- `Widget`.

Experimental headers may still need explicit includes until they become part of the stable umbrella API.
