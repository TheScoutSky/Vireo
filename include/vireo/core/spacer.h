#ifndef VIREO_SPACER_H
#define VIREO_SPACER_H

#include <memory>

#include <vireo/core/component.h>
#include <vireo/core/geometry.hpp>

namespace vireo {

class VIREO_API SpacerComponent : public Component {
  public:
    explicit SpacerComponent(int width = 16, int height = 16);

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;
};

class VIREO_API Spacer {
  public:
    explicit Spacer(int width = 16, int height = 16);

    [[nodiscard]] std::unique_ptr<Component> build() &&;

  private:
    std::unique_ptr<Component> component_;
};

} // namespace vireo

#endif // VIREO_SPACER_H
