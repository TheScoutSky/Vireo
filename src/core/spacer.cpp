#include <vireo/core/spacer.h>

#include <memory>

namespace vireo {

SpacerComponent::SpacerComponent(int width, int height) : Component(0, 0, width, height, 0, 0, 0) {}

void SpacerComponent::handleSelf(SDL_Event*) {}

void SpacerComponent::renderSelf(SDL_Renderer*) {}

void SpacerComponent::updateSelf(float) {}

Spacer::Spacer(int width, int height) : component_(std::make_unique<SpacerComponent>(width, height)) {}

std::unique_ptr<Component> Spacer::build() && {
    return std::move(component_);
}

} // namespace vireo
