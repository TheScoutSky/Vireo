#ifndef VIREO_IMAGE_H
#define VIREO_IMAGE_H

#include <SDL_render.h>

#include <memory>
#include <string>

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/style.h>

namespace vireo {

class VIREO_API ImageComponent : public Component {
  public:
    explicit ImageComponent(std::string path, Rect rect = {0, 0, 96, 96}, ImageFit fit = ImageFit::Contain);
    ~ImageComponent() override;

    ImageComponent(const ImageComponent&) = delete;
    ImageComponent& operator=(const ImageComponent&) = delete;

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

  private:
    void loadTexture(SDL_Renderer* renderer);
    void renderPlaceholder(SDL_Renderer* renderer);
    [[nodiscard]] SDL_Rect fittedRect(const SDL_Rect& bounds) const;

    std::string path_;
    ImageFit fit_{ImageFit::Contain};
    SDL_Texture* texture_{nullptr};
    int textureWidth_{0};
    int textureHeight_{0};
    bool triedLoad_{false};
};

class VIREO_API Image {
  public:
    explicit Image(std::string path, Rect rect = {0, 0, 96, 96}, ImageFit fit = ImageFit::Contain);

    [[nodiscard]] std::unique_ptr<Component> build() &&;

  private:
    std::unique_ptr<Component> component_;
};

} // namespace vireo

#endif // VIREO_IMAGE_H
