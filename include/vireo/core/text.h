#ifndef VIREO_TEXT_H
#define VIREO_TEXT_H

#include <memory>
#include <string>

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/geometry.hpp>

namespace vireo {

class VIREO_API TextComponent : public Component {
  public:
    explicit TextComponent(std::string value, Rect rect = {24, 22, 0, 0}, Color color = colors::white, int scale = 3);

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

    [[nodiscard]] const std::string& value() const noexcept;

  private:
    std::string value_;
    Color color_;
    int scale_{3};
};

class VIREO_API Text {
  public:
    explicit Text(std::string value, Rect rect = {24, 22, 0, 0}, Color color = colors::white, int scale = 3);

    [[nodiscard]] std::unique_ptr<Component> build() &&;

  private:
    std::unique_ptr<Component> component_;
};

} // namespace vireo

#endif // VIREO_TEXT_H
