#include <vireo/core/screen.h>

#include <SDL_events.h>

#include <algorithm>
#include <memory>

namespace vireo {

ScreenComponent::ScreenComponent(int width, int height) : Component(0, 0, width, height, 0, 0, 0) {}

void ScreenComponent::render(SDL_Renderer* renderer) {
    int width = 0;
    int height = 0;
    if (renderer != nullptr && SDL_GetRendererOutputSize(renderer, &width, &height) == 0) {
        const SDL_Rect current = getBaseRect();
        if (current.w != width || current.h != height) {
            setSize(std::max(0, width), std::max(0, height));
        }
    }

    Component::render(renderer);
}

void ScreenComponent::handleSelf(SDL_Event* event) {
    if (event == nullptr || event->type != SDL_WINDOWEVENT) {
        return;
    }

    if (event->window.event == SDL_WINDOWEVENT_RESIZED || event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        setSize(std::max(0, event->window.data1), std::max(0, event->window.data2));
    }
}

void ScreenComponent::renderSelf(SDL_Renderer*) {}

void ScreenComponent::updateSelf(float) {}

Screen::Screen() : ComponentBuilder(std::make_unique<ScreenComponent>()) {}

Screen::Screen(int width, int height) : ComponentBuilder(std::make_unique<ScreenComponent>(width, height)) {}

} // namespace vireo
