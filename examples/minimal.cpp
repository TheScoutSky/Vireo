#include <vireo/vireo.hpp>

#include <SDL.h>

#include <exception>
#include <iostream>

int main(int, char**) {
    try {
        vireo::SdlContext sdl;

        vireo::WindowOptions window_options;
        window_options.title = "Vireo minimal example";
        window_options.width = 960;
        window_options.height = 540;

        vireo::Window window(window_options);
        vireo::Renderer renderer(window);

        bool running = true;
        while (running) {
            SDL_Event event{};
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            renderer.clear(vireo::rgba(24, 27, 34));
            renderer.present();
            SDL_Delay(16);
        }
    } catch (const std::exception& error) {
        std::cerr << "Vireo example error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
