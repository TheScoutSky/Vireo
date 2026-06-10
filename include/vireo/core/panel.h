#ifndef VIREO_PANEL_H
#define VIREO_PANEL_H

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/componentbuilder.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/style.h>

namespace vireo {

class VIREO_API PanelComponent : public Component {
  public:
    explicit PanelComponent(Rect rect = {0, 0, 320, 220}, Color color = rgba(36, 40, 50),
                            Color border = rgba(82, 90, 108));
    explicit PanelComponent(Rect rect, PanelStyle style);

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

  private:
    Color color_;
    Color border_;
    int borderWidth_{1};
};

class VIREO_API Panel : public ComponentBuilder<Panel> {
  public:
    explicit Panel(Rect rect = {0, 0, 320, 220}, Color color = rgba(36, 40, 50), Color border = rgba(82, 90, 108));
    explicit Panel(Rect rect, PanelStyle style);
};

} // namespace vireo

#endif // VIREO_PANEL_H
