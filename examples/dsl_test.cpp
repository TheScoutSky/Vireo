#include <vireo/vireo.hpp>

#include <SDL.h>

#include <cstring>
#include <exception>
#include <iostream>
#include <string>

#ifndef VIREO_EXAMPLES_ASSET_DIR
#define VIREO_EXAMPLES_ASSET_DIR "."
#endif

int main(int argc, char** argv) {
    try {
        bool runOnce = false;
        for (int index = 1; index < argc; ++index) {
            if (std::strcmp(argv[index], "--once") == 0) {
                runOnce = true;
            }
        }

        constexpr int width = 960;
        constexpr int height = 540;

        vireo::SdlContext sdl;

        vireo::WindowOptions windowOptions;
        windowOptions.title = "Vireo DSL test";
        windowOptions.width = width;
        windowOptions.height = height;

        vireo::Window window(windowOptions);

        vireo::RendererOptions rendererOptions;
        if (runOnce) {
            rendererOptions.flags = SDL_RENDERER_SOFTWARE;
        }

        vireo::Renderer renderer(window, rendererOptions);

        vireo::PanelStyle cardStyle;
        cardStyle.background = vireo::rgba(34, 39, 49);
        cardStyle.border = vireo::rgba(94, 108, 132);
        cardStyle.borderWidth = 2;

        vireo::ButtonStyle primaryButton =
            vireo::buttonStyle(vireo::colors::green, vireo::rgba(88, 218, 124), vireo::rgba(44, 154, 78));
        primaryButton.borderWidth = 2;

        const std::string logoPath = std::string(VIREO_EXAMPLES_ASSET_DIR) + "/vireo.xpm";

        auto ui = vireo::Screen(width, height)(
            vireo::Center({0, 0, width, height})(
                vireo::VStack({0, 0, width/4, height / 2}, 16, 0, vireo::Alignment::Center, vireo::Justify::Center)(
                    vireo::Image(logoPath, {0, 0, 64, 64}),
                    vireo::Text("CChess"),
                    vireo::HorizontalDivider(),
                    vireo::Spacer(),
                    vireo::Button(vireo::colors::green, {0, 0, 200, 50}) (
                        vireo::Center({0, 0, 200, 50}) (
                            vireo::Text("Start")
                        )
                    ).onClick([]() {
                        std::cout << "Start button clicked!\n";
                    }),
                    vireo::Button(vireo::colors::blue, {0, 0, 200, 50}) (
                        vireo::Center({0, 0, 200, 50}) (
                            vireo::Text("Options")
                        )
                    ).onClick([]() {
                        std::cout << "Options button clicked!\n";
                    }),
                    vireo::Button(vireo::colors::red, {0, 0, 200, 50}) (
                        vireo::Center({0, 0, 200, 50}) (
                            vireo::Text("Exit")
                        )
                    ).onClick([]() {
                        std::cout << "Start button clicked!\n";
                    }),
                    vireo::Spacer(16, 40)

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

            if (runOnce) {
                running = false;
            }

            SDL_Delay(16);
        }
    } catch (const std::exception& error) {
        std::cerr << "Vireo DSL test error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
