//
// Created by Antonio Wil on 10.06.26.
//

#ifndef VIREO_SCREEN_H
#define VIREO_SCREEN_H

#include <memory>

#include "component.h"
#include "componentbuilder.h"

namespace vireo {

class VIREO_API ScreenComponent : public Component {
  public:
    ScreenComponent(int width = 0, int height = 0);

    void render(SDL_Renderer* renderer) override;
    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;
};

class VIREO_API Screen : public ComponentBuilder<Screen> {
  public:
    Screen();
    Screen(int width, int height);
};

} // namespace vireo

#endif // VIREO_SCREEN_H
