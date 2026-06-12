# Getting Started

This page shows the basic shape of a Vireo application: initialize SDL, create a window and renderer, build a UI tree,
then forward events, updates, and rendering through that tree.

## Installation

Clone and build Vireo with CMake:

```bash
git clone https://github.com/TheScoutSky/vireo.git
cmake -S vireo -B vireo/build -DCMAKE_BUILD_TYPE=Release
cmake --build vireo/build
cmake --install vireo/build --prefix "$HOME/.local"
```

Or include it with `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    vireo
    GIT_REPOSITORY https://github.com/TheScoutSky/vireo.git
    GIT_TAG v0.0.2
)

FetchContent_MakeAvailable(vireo)

target_link_libraries(my_app PRIVATE vireo::vireo)
```

## Basic UI

Vireo components are usually composed with the builder DSL:

```cpp
#include <vireo/vireo.hpp>

#include <SDL.h>

#include <exception>
#include <iostream>

int main(int, char**) {
    try {
        vireo::SdlContext sdl;

        vireo::WindowOptions windowOptions;
        windowOptions.title = "Vireo app";
        windowOptions.width = 960;
        windowOptions.height = 540;

        vireo::Window window(windowOptions);
        vireo::Renderer renderer(window);

        auto ui = vireo::Screen(960, 540)(
            vireo::Center({0, 0, 960, 540})(
                vireo::VStack({0, 0, 960, 540}, 16, 0,
                              vireo::Alignment::Center,
                              vireo::Justify::Center)(
                    vireo::Text("Hello Vireo"),
                    vireo::Button(vireo::colors::green, {0, 0, 220, 56})(
                        vireo::Center({0, 0, 220, 56})(
                            vireo::Text("Start")
                        )
                    )
                )
            )
        ).build();

        bool running = true;
        Uint64 previousTicks = SDL_GetTicks64();

        while (running) {
            SDL_Event event{};
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }

                ui->handleEvents(&event);
            }

            const Uint64 currentTicks = SDL_GetTicks64();
            const float deltaTime = static_cast<float>(currentTicks - previousTicks) / 1000.0f;
            previousTicks = currentTicks;

            ui->update(deltaTime);

            renderer.clear(vireo::colors::slate);
            ui->render(renderer.native_handle());
            renderer.present();

            SDL_Delay(16);
        }
    } catch (const std::exception& error) {
        std::cerr << "Vireo error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
```

## Mental Model

The DSL builds a tree of `Component` objects. A `Screen` can contain a `Center`, a `Center` can contain a `VStack`,
and the `VStack` can contain text, buttons, spacers, and other components.

After `.build()`, the result is a `std::unique_ptr<Component>`. From that point on, the UI behaves like a runtime tree:
events, updates, and rendering start at the root and flow through the children.

For state-driven screens, generated lists, and popups, see [Dynamic UI](Dynamic-UI.md).
