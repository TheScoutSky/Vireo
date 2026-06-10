#ifndef VIREO_VERTICALSTACK_H
#define VIREO_VERTICALSTACK_H

#include <vireo/core/component.h>
#include <vireo/core/componentbuilder.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/style.h>

namespace vireo {

class VIREO_API VerticalStackComponent : public Component {
  public:
    explicit VerticalStackComponent(Rect rect = {0, 0, 320, 300}, int gap = 12, int padding = 0,
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

class VIREO_API VerticalStack : public ComponentBuilder<VerticalStack> {
  public:
    explicit VerticalStack(Rect rect = {0, 0, 320, 300}, int gap = 12, int padding = 0,
                           Alignment alignment = Alignment::Start, Justify justify = Justify::Start);
};

using VStack = VerticalStack;

} // namespace vireo

#endif // VIREO_VERTICALSTACK_H
