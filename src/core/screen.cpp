#include <vireo/core/screen.h>

#include <memory>

namespace vireo {

ScreenComponent::ScreenComponent(int width, int height) : Component(0, 0, width, height, 0, 0, 0) {}

void ScreenComponent::handleSelf(SDL_Event*) {}

void ScreenComponent::renderSelf(SDL_Renderer*) {}

void ScreenComponent::updateSelf(float) {}

Screen::Screen(int width, int height) : ComponentBuilder(std::make_unique<ScreenComponent>(width, height)) {}

} // namespace vireo
