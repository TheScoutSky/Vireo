#ifndef VIREO_BUTTON_H
#define VIREO_BUTTON_H

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/componentbuilder.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/style.h>

namespace vireo {

class VIREO_API ButtonComponent : public Component {
  public:
    explicit ButtonComponent(Color color, Rect rect = {80, 80, 220, 72});
    explicit ButtonComponent(Rect rect, ButtonStyle style);

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

    void onClick(std::function<void()> callback) noexcept;

  private:
    [[nodiscard]] Color currentColor() const noexcept;

    ButtonStyle style_;
    bool hovered_{false};
    bool pressed_{false};

    std::function<void()> onClick_;
};

class VIREO_API Button : public ComponentBuilder<Button> {
  public:
    explicit Button(Color color, Rect rect = {80, 80, 220, 72});
    explicit Button(Rect rect, ButtonStyle style);

    Button&& onClick(std::function<void()> callback) &&;
};

} // namespace vireo

#endif // VIREO_BUTTON_H
