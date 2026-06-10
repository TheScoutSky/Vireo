#include <vireo/core/divider.h>

#include <memory>

namespace vireo {

DividerComponent::DividerComponent(Rect rect, Color color)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), color_(color) {}

void DividerComponent::handleSelf(SDL_Event*) {}

void DividerComponent::renderSelf(SDL_Renderer* renderer) {
    SDL_Rect bounds = getAbsoluteBaseRect();

    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &bounds);
}

void DividerComponent::updateSelf(float) {}

HorizontalDivider::HorizontalDivider(int width, Color color)
    : component_(std::make_unique<DividerComponent>(Rect{0, 0, width, 1}, color)) {}

std::unique_ptr<Component> HorizontalDivider::build() && {
    return std::move(component_);
}

VerticalDivider::VerticalDivider(int height, Color color)
    : component_(std::make_unique<DividerComponent>(Rect{0, 0, 1, height}, color)) {}

std::unique_ptr<Component> VerticalDivider::build() && {
    return std::move(component_);
}

} // namespace vireo
