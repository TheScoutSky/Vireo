#ifndef VIREO_CHECKBOX_H
#define VIREO_CHECKBOX_H

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/componentbuilder.h>
#include <vireo/core/geometry.hpp>

namespace vireo {

class VIREO_API CheckboxComponent : public Component {
  public:
    explicit CheckboxComponent(bool checked = false, Rect rect = {0, 0, 28, 28});

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

    [[nodiscard]] bool checked() const noexcept;
    void setChecked(bool checked);

  private:
    bool checked_{false};
    bool hovered_{false};
    bool pressedInside_{false};
};

class VIREO_API Checkbox : public ComponentBuilder<Checkbox> {
  public:
    explicit Checkbox(bool checked = false, Rect rect = {0, 0, 28, 28});
};

} // namespace vireo

#endif // VIREO_CHECKBOX_H
