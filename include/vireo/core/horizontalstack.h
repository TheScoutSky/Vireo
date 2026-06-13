#ifndef VIREO_HORIZONTALSTACK_H
#define VIREO_HORIZONTALSTACK_H

#include <vireo/core/component.h>
#include <vireo/core/componentbuilder.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/style.h>

namespace vireo {

class VIREO_API HorizontalStackComponent : public Component {
  public:
    explicit HorizontalStackComponent(Rect rect = {0, 0, 400, 80}, int gap = 12, int padding = 0,
                                      Alignment alignment = Alignment::Start, Justify justify = Justify::Start);
    explicit HorizontalStackComponent(FillTag fill, int gap = 12, int padding = 0,
                                      Alignment alignment = Alignment::Start, Justify justify = Justify::Start);

    void render(SDL_Renderer* renderer) override;
    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

  private:
    void layoutChildren();

    int gap_{12};
    int padding_{0};
    Alignment alignment_{Alignment::Start};
    Justify justify_{Justify::Start};
};

class VIREO_API HorizontalStack : public ComponentBuilder<HorizontalStack> {
  public:
    explicit HorizontalStack(Rect rect = {0, 0, 400, 80}, int gap = 12, int padding = 0,
                             Alignment alignment = Alignment::Start, Justify justify = Justify::Start);
    explicit HorizontalStack(FillTag fill, int gap = 12, int padding = 0, Alignment alignment = Alignment::Start,
                             Justify justify = Justify::Start);
};

using HStack = HorizontalStack;

} // namespace vireo

#endif // VIREO_HORIZONTALSTACK_H
