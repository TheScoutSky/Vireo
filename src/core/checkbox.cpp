#include <vireo/core/checkbox.h>

#include <SDL_mouse.h>

#include <memory>

namespace vireo {

CheckboxComponent::CheckboxComponent(bool checked, Rect rect)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), checked_(checked) {}

void CheckboxComponent::handleSelf(SDL_Event* event) {
    const SDL_Rect bounds = getAbsoluteBaseRect();

    if (event->type == SDL_MOUSEMOTION) {
        const SDL_Point mouse{event->motion.x, event->motion.y};
        hovered_ = SDL_PointInRect(&mouse, &bounds) == SDL_TRUE;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        const SDL_Point mouse{event->button.x, event->button.y};
        pressedInside_ = SDL_PointInRect(&mouse, &bounds) == SDL_TRUE;
    }

    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        const SDL_Point mouse{event->button.x, event->button.y};
        const bool releasedInside = SDL_PointInRect(&mouse, &bounds) == SDL_TRUE;

        if (pressedInside_ && releasedInside) {
            checked_ = !checked_;
            markDirty();
        }

        pressedInside_ = false;
    }
}

void CheckboxComponent::renderSelf(SDL_Renderer* renderer) {
    SDL_Rect bounds = getAbsoluteBaseRect();

    const Color fill = hovered_ ? rgba(54, 60, 74) : rgba(40, 45, 56);
    SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
    SDL_RenderFillRect(renderer, &bounds);

    SDL_SetRenderDrawColor(renderer, 115, 126, 148, 255);
    SDL_RenderDrawRect(renderer, &bounds);

    if (!checked_) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, colors::green.r, colors::green.g, colors::green.b, colors::green.a);

    const int left = bounds.x + bounds.w / 4;
    const int middleX = bounds.x + bounds.w / 2;
    const int right = bounds.x + bounds.w - bounds.w / 5;
    const int top = bounds.y + bounds.h / 3;
    const int middleY = bounds.y + bounds.h - bounds.h / 4;

    SDL_RenderDrawLine(renderer, left, bounds.y + bounds.h / 2, middleX, middleY);
    SDL_RenderDrawLine(renderer, middleX, middleY, right, top);
    SDL_RenderDrawLine(renderer, left, bounds.y + bounds.h / 2 + 1, middleX, middleY + 1);
    SDL_RenderDrawLine(renderer, middleX, middleY + 1, right, top + 1);
}

void CheckboxComponent::updateSelf(float) {}

bool CheckboxComponent::checked() const noexcept {
    return checked_;
}

void CheckboxComponent::setChecked(bool checked) {
    checked_ = checked;
    markDirty();
}

Checkbox::Checkbox(bool checked, Rect rect) : ComponentBuilder(std::make_unique<CheckboxComponent>(checked, rect)) {}

} // namespace vireo
