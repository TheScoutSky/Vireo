#ifndef VIREO_DIVIDER_H
#define VIREO_DIVIDER_H

#include <memory>

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/geometry.hpp>

namespace vireo {

class VIREO_API DividerComponent : public Component {
  public:
    explicit DividerComponent(Rect rect, Color color = rgba(82, 90, 108));

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

  private:
    Color color_;
};

class VIREO_API HorizontalDivider {
  public:
    explicit HorizontalDivider(int width = 240, Color color = rgba(82, 90, 108));

    [[nodiscard]] std::unique_ptr<Component> build() &&;

  private:
    std::unique_ptr<Component> component_;
};

class VIREO_API VerticalDivider {
  public:
    explicit VerticalDivider(int height = 80, Color color = rgba(82, 90, 108));

    [[nodiscard]] std::unique_ptr<Component> build() &&;

  private:
    std::unique_ptr<Component> component_;
};

} // namespace vireo

#endif // VIREO_DIVIDER_H
