#ifndef VIREO_CENTER_H
#define VIREO_CENTER_H

#include <vireo/core/component.h>
#include <vireo/core/componentbuilder.h>
#include <vireo/core/geometry.hpp>

namespace vireo {

class VIREO_API CenterComponent : public Component {
  public:
    explicit CenterComponent(Rect rect = {0, 0, 320, 220});
    explicit CenterComponent(FillTag fill);

    void render(SDL_Renderer* renderer) override;
    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

  private:
    void layoutChildren();
};

class VIREO_API Center : public ComponentBuilder<Center> {
  public:
    explicit Center(Rect rect = {0, 0, 320, 220});
    explicit Center(FillTag fill);
};

} // namespace vireo

#endif // VIREO_CENTER_H
