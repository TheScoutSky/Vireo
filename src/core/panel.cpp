#include <vireo/core/panel.h>

#include <memory>

namespace vireo {

PanelComponent::PanelComponent(Rect rect, Color color, Color border)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), color_(color), border_(border) {}

PanelComponent::PanelComponent(Rect rect, PanelStyle style)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), color_(style.background), border_(style.border),
      borderWidth_(style.borderWidth) {
    setPadding(style.padding);
}

void PanelComponent::handleSelf(SDL_Event*) {}

void PanelComponent::renderSelf(SDL_Renderer* renderer) {
    SDL_Rect bounds = getAbsoluteBaseRect();

    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &bounds);

    SDL_SetRenderDrawColor(renderer, border_.r, border_.g, border_.b, border_.a);
    for (int index = 0; index < borderWidth_; ++index) {
        SDL_Rect borderRect{bounds.x + index, bounds.y + index, bounds.w - 2 * index, bounds.h - 2 * index};
        SDL_RenderDrawRect(renderer, &borderRect);
    }
}

void PanelComponent::updateSelf(float) {}

Panel::Panel(Rect rect, Color color, Color border)
    : ComponentBuilder(std::make_unique<PanelComponent>(rect, color, border)) {}

Panel::Panel(Rect rect, PanelStyle style) : ComponentBuilder(std::make_unique<PanelComponent>(rect, style)) {}

} // namespace vireo
