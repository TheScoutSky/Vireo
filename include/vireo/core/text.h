#ifndef VIREO_TEXT_H
#define VIREO_TEXT_H

#include <memory>
#include <string>

#include <vireo/core/color.hpp>
#include <vireo/core/component.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/style.h>

struct SDL_Renderer;
struct SDL_Texture;
typedef struct TTF_Font TTF_Font;

namespace vireo {

class VIREO_API TextComponent : public Component {
  public:
    explicit TextComponent(std::string value, Rect rect = {24, 22, 0, 0}, Color color = colors::white, int scale = 3);
    explicit TextComponent(std::string value, TextStyle style, Rect rect = {24, 22, 0, 0});
    ~TextComponent() override;

    TextComponent(const TextComponent&) = delete;
    TextComponent& operator=(const TextComponent&) = delete;

    void handleSelf(SDL_Event* event) override;
    void renderSelf(SDL_Renderer* renderer) override;
    void updateSelf(float deltaTime) override;

    [[nodiscard]] const std::string& value() const noexcept;

  private:
    void renderFallback(SDL_Renderer* renderer);
    void renderTtf(SDL_Renderer* renderer);
    void loadFont();
    void rebuildTexture(SDL_Renderer* renderer);
    void releaseTexture() noexcept;
    void releaseFont() noexcept;

    std::string value_;
    TextStyle style_{};
    bool autoWidth_{false};
    bool autoHeight_{false};
    TTF_Font* font_{nullptr};
    SDL_Texture* texture_{nullptr};
    SDL_Renderer* textureRenderer_{nullptr};
    int textureWidth_{0};
    int textureHeight_{0};
    bool triedFontLoad_{false};
};

class VIREO_API Text {
  public:
    explicit Text(std::string value, Rect rect = {24, 22, 0, 0}, Color color = colors::white, int scale = 3);
    explicit Text(std::string value, TextStyle style, Rect rect = {24, 22, 0, 0});

    [[nodiscard]] std::unique_ptr<Component> build() &&;

  private:
    std::unique_ptr<Component> component_;
};

} // namespace vireo

#endif // VIREO_TEXT_H
