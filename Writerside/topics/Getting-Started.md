# Getting Started

This page shows the basic shape of a Vireo application.
The application creates SDL resources, builds a UI tree, forwards SDL events into that tree, updates it once per frame,
and renders it through the SDL renderer.

## Include Vireo

Use the umbrella header for application code:

```cpp
#include <vireo/vireo.hpp>
```

It includes the core components, layout helpers, styling helpers, platform wrappers, and version header.

Experimental headers may still need explicit includes while they are being developed. For example, the current
`ForEach` work-in-progress lives in:

```cpp
#include <vireo/core/foreach.h>
```

## Minimal Window

The smallest useful Vireo program creates a context, a window, and a renderer:

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

        bool running = true;
        while (running) {
            SDL_Event event{};
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            renderer.clear(vireo::colors::slate);
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

## Add A UI Tree

Most Vireo UIs start with a `Screen`, then use layout helpers and components inside it:

```cpp
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
            ).onClick([] {
                std::cout << "Start clicked\n";
            })
        )
    )
).build();
```

`Screen(...)(...)` creates a builder. `.build()` consumes that builder and returns a
`std::unique_ptr<vireo::Component>`.

## Use The Tree Each Frame

The built tree participates in the application loop:

```cpp
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
```

The usual order is:

1. Poll SDL events.
2. Send each event into `ui->handleEvents(&event)`.
3. Compute frame delta time.
4. Call `ui->update(deltaTime)`.
5. Clear the renderer.
6. Call `ui->render(renderer.native_handle())`.
7. Present the renderer.

## Next Steps

Read [Core Concepts](Core-Concepts.md) next if the builder syntax feels unusual.
Read [Layout](Layout.md) and [Components](Components.md) when you want to build real screens.
