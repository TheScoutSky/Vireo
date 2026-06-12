#include <vireo/core/button.h>

#include <SDL_mouse.h>

#include <algorithm>
#include <memory>
#include <utility>

namespace vireo {

namespace {

[[nodiscard]] int brighten(std::uint8_t value, int amount) noexcept {
    return std::clamp(static_cast<int>(value) + amount, 0, 255);
}

[[nodiscard]] ButtonStyle makeButtonStyle(Color color) noexcept {
    return ButtonStyle{
        color,
        Color{
            static_cast<std::uint8_t>(brighten(color.r, 20)),
            static_cast<std::uint8_t>(brighten(color.g, 20)),
            static_cast<std::uint8_t>(brighten(color.b, 20)),
            color.a,
        },
        Color{
            static_cast<std::uint8_t>(std::clamp(static_cast<int>(color.r) - 26, 0, 255)),
            static_cast<std::uint8_t>(std::clamp(static_cast<int>(color.g) - 26, 0, 255)),
            static_cast<std::uint8_t>(std::clamp(static_cast<int>(color.b) - 26, 0, 255)),
            color.a,
        },
        rgba(13, 16, 22),
        1,
        EdgeInsets::all(0),
    };
}

} // namespace

ButtonComponent::ButtonComponent(Color color, Rect rect) : ButtonComponent(rect, makeButtonStyle(color)) {}

ButtonComponent::ButtonComponent(Rect rect, ButtonStyle style)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), style_(style) {
    setPadding(style.padding);
}

void ButtonComponent::handleSelf(SDL_Event* event) {
    const SDL_Rect bounds = getAbsoluteBaseRect();

    if (event->type == SDL_MOUSEMOTION) {
        const SDL_Point mouse{event->motion.x, event->motion.y};
        hovered_ = SDL_PointInRect(&mouse, &bounds) == SDL_TRUE;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        const SDL_Point mouse{event->button.x, event->button.y};
        pressed_ = SDL_PointInRect(&mouse, &bounds) == SDL_TRUE;
        if (pressed_ && onClick_) {
            onClick_();
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        pressed_ = false;
    }
}

void ButtonComponent::renderSelf(SDL_Renderer* renderer) {
    const Color drawColor = currentColor();
    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

    SDL_Rect bounds = getAbsoluteBaseRect();
    SDL_RenderFillRect(renderer, &bounds);

    SDL_SetRenderDrawColor(renderer, style_.border.r, style_.border.g, style_.border.b, style_.border.a);
    for (int index = 0; index < style_.borderWidth; ++index) {
        SDL_Rect borderRect{bounds.x + index, bounds.y + index, bounds.w - 2 * index, bounds.h - 2 * index};
        SDL_RenderDrawRect(renderer, &borderRect);
    }
}

void ButtonComponent::onClick(std::function<void()> callback) noexcept {
    onClick_ = std::move(callback);
}

void ButtonComponent::updateSelf(float) {}

Color ButtonComponent::currentColor() const noexcept {
    if (pressed_) {
        return style_.pressed;
    }

    if (hovered_) {
        return style_.hover;
    }

    return style_.background;
}

Button::Button(Color color, Rect rect) : ComponentBuilder(std::make_unique<ButtonComponent>(color, rect)) {}

Button::Button(Rect rect, ButtonStyle style) : ComponentBuilder(std::make_unique<ButtonComponent>(rect, style)) {}

Button&& Button::onClick(std::function<void()> callback) && {
    if (auto* buttonComponent = dynamic_cast<ButtonComponent*>(component_.get())) {
        buttonComponent->onClick(std::move(callback));
    }
    return std::move(*this);
}

} // namespace vireo
