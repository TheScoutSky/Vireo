# Project Setup

Vireo is a CMake project that builds a `vireo` library target and exposes it as `vireo::vireo`.
Applications link against that target and include headers from `include/vireo`.

## Dependencies

Vireo currently links these SDL packages publicly:

- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer
- SDL2_net

Because these dependencies are public, applications that link `vireo::vireo` receive the same link dependencies through
CMake.

The project also checks `$HOME/.local/sdl2` and prepends it to `CMAKE_PREFIX_PATH` when SDL2 CMake files are found
there. This is useful for local SDL builds on macOS.

## Configure And Build

Use the provided CMake presets:

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

The debug preset enables examples and tests:

```cmake
VIREO_BUILD_EXAMPLES=ON
VIREO_BUILD_TESTS=ON
```

The release preset uses the same example/test defaults when Vireo is the top-level project:

```bash
cmake --preset release
cmake --build --preset release
```

## Link From Another Project

The intended CMake usage is:

```cmake
find_package(vireo CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE vireo::vireo)
```

For direct Git usage, use `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    vireo
    GIT_REPOSITORY https://github.com/TheScoutSky/Vireo.git
    GIT_TAG v0.0.5
)

FetchContent_MakeAvailable(vireo)

target_link_libraries(my_app PRIVATE vireo::vireo)
```

## Examples

The project currently includes two example executables when examples are enabled:

| Target | Source | Purpose |
|--------|--------|---------|
| `vireo_minimal` | `examples/minimal.cpp` | Opens a window and clears the screen. |
| `vireo_dsl_test` | `examples/dsl_test.cpp` | Exercises the builder DSL, layouts, text, buttons, checkboxes, and images. |

The DSL example receives two compile definitions:

- `VIREO_EXAMPLES_ASSET_DIR`
- `VIREO_EXAMPLES_FONT_PATH`

On macOS, the project tries to use a system font path automatically for the example.

## Tests

When tests are enabled, Vireo builds `vireo_smoke_test` and registers it with CTest.
If the DSL example target exists, a `vireo_dsl_test_once` test is also registered and runs with:

```text
SDL_VIDEODRIVER=dummy
```

That allows the example to run in CI or headless environments without opening a normal desktop window.

## Install Layout

The install rules export the CMake package files and headers:

- Library target: `vireo`
- Export namespace: `vireo::`
- Headers: `include/vireo`
- Package config: `vireoConfig.cmake`
- Version file: `vireoConfigVersion.cmake`
- pkg-config file: `vireo.pc`

See [Publishing](Publishing.md) for release and installation notes.
