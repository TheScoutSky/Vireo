# Application Lifecycle

Vireo does not hide SDL's application loop.
Instead, it wraps the noisy resource management parts and lets the application stay in control of events, updates, and
rendering.

## SDL Context

`SdlContext` initializes SDL subsystems and SDL_ttf:

```cpp
vireo::SdlContext sdl;
```

By default, it initializes:

```cpp
SDL_INIT_VIDEO | SDL_INIT_EVENTS
```

You can pass different SDL flags:

```cpp
vireo::SdlContext sdl(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
```

`SdlContext` is RAII-based:

- construction initializes SDL and SDL_ttf;
- destruction quits the subsystems it owns;
- copying is disabled;
- moving is supported.

Useful methods:

```cpp
bool ok = sdl.valid();
Uint32 flags = sdl.flags();
```

## Window

Create windows through `WindowOptions`:

```cpp
vireo::WindowOptions options;
options.title = "Vireo";
options.width = 1280;
options.height = 720;
options.flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

vireo::Window window(options);
```

Defaults:

| Option | Default |
|--------|---------|
| `title` | `"Vireo"` |
| `width` | `1280` |
| `height` | `720` |
| `x` | `SDL_WINDOWPOS_CENTERED` |
| `y` | `SDL_WINDOWPOS_CENTERED` |
| `flags` | `SDL_WINDOW_SHOWN \| SDL_WINDOW_RESIZABLE` |

Useful methods:

```cpp
SDL_Window* handle = window.native_handle();
vireo::Size size = window.size();
window.set_title("New title");
```

`Window` owns the `SDL_Window*`, destroys it automatically, disables copying, and supports moving.

## Renderer

Create a renderer from a window:

```cpp
vireo::Renderer renderer(window);
```

The default renderer options are:

```cpp
driver_index = -1;
flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
```

You can override them:

```cpp
vireo::RendererOptions options;
options.flags = SDL_RENDERER_SOFTWARE;

vireo::Renderer renderer(window, options);
```

Useful methods:

```cpp
renderer.set_draw_color(vireo::colors::green);
renderer.clear(vireo::colors::slate);
renderer.present();

SDL_Renderer* native = renderer.native_handle();
```

## Frame Loop

A typical Vireo frame loop has this shape:

```cpp
while (running) {
    SDL_Event event{};
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        ui->handleEvents(&event);
    }

    ui->update(deltaTime);

    renderer.clear(vireo::colors::slate);
    ui->render(renderer.native_handle());
    renderer.present();
}
```

Vireo does not currently provide a standalone app runner. That keeps the framework simple and lets your game or app own
the loop timing, fixed updates, audio, and shutdown behavior.

## Error Handling

Platform constructors throw exceptions when SDL calls fail.
The examples use a top-level `try`/`catch`:

```cpp
try {
    vireo::SdlContext sdl;
    vireo::Window window;
    vireo::Renderer renderer(window);
} catch (const std::exception& error) {
    std::cerr << "Vireo error: " << error.what() << '\n';
}
```
