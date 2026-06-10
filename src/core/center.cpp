#include <vireo/core/center.h>

#include <algorithm>
#include <memory>

namespace vireo {

CenterComponent::CenterComponent(Rect rect) : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0) {}

void CenterComponent::render(SDL_Renderer* renderer) {
    layoutChildren();
    Component::render(renderer);
}

void CenterComponent::handleSelf(SDL_Event*) {}

void CenterComponent::renderSelf(SDL_Renderer*) {}

void CenterComponent::updateSelf(float) {}

void CenterComponent::layoutChildren() {
    const SDL_Rect bounds = getBaseRect();

    for (Component* child : getChildren()) {
        if (child == nullptr) {
            continue;
        }

        const SDL_Rect childBounds = child->getBaseRect();
        child->setPosition(std::max(0, (bounds.w - childBounds.w) / 2), std::max(0, (bounds.h - childBounds.h) / 2));
    }
}

Center::Center(Rect rect) : ComponentBuilder(std::make_unique<CenterComponent>(rect)) {}

} // namespace vireo
