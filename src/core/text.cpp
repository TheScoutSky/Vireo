#include <vireo/core/text.h>

#include <SDL_rect.h>
#include <SDL_ttf.h>

#include <array>
#include <cctype>
#include <memory>
#include <utility>

namespace vireo {

namespace {

using Glyph = std::array<std::uint8_t, 7>;

[[nodiscard]] Glyph glyphFor(char value) noexcept {
    switch (static_cast<char>(std::toupper(static_cast<unsigned char>(value)))) {
    case 'A':
        return {0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001};
    case 'B':
        return {0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b11110};
    case 'C':
        return {0b01111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b01111};
    case 'D':
        return {0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11110};
    case 'E':
        return {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111};
    case 'F':
        return {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000};
    case 'G':
        return {0b01111, 0b10000, 0b10000, 0b10011, 0b10001, 0b10001, 0b01111};
    case 'H':
        return {0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001};
    case 'I':
        return {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b11111};
    case 'J':
        return {0b00111, 0b00010, 0b00010, 0b00010, 0b10010, 0b10010, 0b01100};
    case 'K':
        return {0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001};
    case 'L':
        return {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111};
    case 'M':
        return {0b10001, 0b11011, 0b10101, 0b10101, 0b10001, 0b10001, 0b10001};
    case 'N':
        return {0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b10001};
    case 'O':
        return {0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110};
    case 'P':
        return {0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000};
    case 'Q':
        return {0b01110, 0b10001, 0b10001, 0b10001, 0b10101, 0b10010, 0b01101};
    case 'R':
        return {0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001};
    case 'S':
        return {0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110};
    case 'T':
        return {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100};
    case 'U':
        return {0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110};
    case 'V':
        return {0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100};
    case 'W':
        return {0b10001, 0b10001, 0b10001, 0b10101, 0b10101, 0b10101, 0b01010};
    case 'X':
        return {0b10001, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b10001};
    case 'Y':
        return {0b10001, 0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b00100};
    case 'Z':
        return {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111};
    case '0':
        return {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110};
    case '1':
        return {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110};
    case '2':
        return {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111};
    case '3':
        return {0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110};
    case '4':
        return {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010};
    case '5':
        return {0b11111, 0b10000, 0b10000, 0b11110, 0b00001, 0b00001, 0b11110};
    case '6':
        return {0b00111, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110};
    case '7':
        return {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000};
    case '8':
        return {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110};
    case '9':
        return {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00010, 0b11100};
    case ' ':
        return {0, 0, 0, 0, 0, 0, 0};
    default:
        return {0b11111, 0b10001, 0b00010, 0b00100, 0b00100, 0b00000, 0b00100};
    }
}

[[nodiscard]] TextStyle makeFallbackTextStyle(Color color, int scale) {
    TextStyle style;
    style.color = color;
    style.fallbackScale = scale;
    return style;
}

[[nodiscard]] Rect normalizedTextRect(const std::string& value, Rect rect, const TextStyle& style) noexcept {
    if (rect.width <= 0) {
        if (!style.fontPath.empty()) {
            rect.width = static_cast<int>(value.size()) * style.fontSize;
        } else {
            rect.width = static_cast<int>(value.size()) * 6 * style.fallbackScale;
        }
    }

    if (rect.height <= 0) {
        if (!style.fontPath.empty()) {
            rect.height = style.fontSize;
        } else {
            rect.height = 7 * style.fallbackScale;
        }
    }

    return rect;
}

} // namespace

TextComponent::TextComponent(std::string value, Rect rect, Color color, int scale)
    : TextComponent(std::move(value), makeFallbackTextStyle(color, scale), rect) {}

TextComponent::TextComponent(std::string value, TextStyle style, Rect rect)
    : Component(normalizedTextRect(value, rect, style).x, normalizedTextRect(value, rect, style).y,
                normalizedTextRect(value, rect, style).width, normalizedTextRect(value, rect, style).height, 0, 0, 0),
      value_(std::move(value)), style_(std::move(style)), autoWidth_(rect.width <= 0), autoHeight_(rect.height <= 0) {}

TextComponent::~TextComponent() {
    releaseTexture();
    releaseFont();
}

void TextComponent::handleSelf(SDL_Event*) {}

void TextComponent::renderSelf(SDL_Renderer* renderer) {
    if (!style_.fontPath.empty()) {
        renderTtf(renderer);
        return;
    }

    renderFallback(renderer);
}

void TextComponent::renderFallback(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, style_.color.r, style_.color.g, style_.color.b, style_.color.a);

    const SDL_Rect origin = getAbsoluteContentRect();
    int cursorX = origin.x;

    for (const char character : value_) {
        const Glyph glyph = glyphFor(character);

        for (int row = 0; row < static_cast<int>(glyph.size()); ++row) {
            for (int column = 0; column < 5; ++column) {
                const bool filled = (glyph[row] & (1 << (4 - column))) != 0;
                if (!filled) {
                    continue;
                }

                SDL_Rect pixel{
                    cursorX + column * style_.fallbackScale,
                    origin.y + row * style_.fallbackScale,
                    style_.fallbackScale,
                    style_.fallbackScale,
                };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }

        cursorX += 6 * style_.fallbackScale;
    }
}

void TextComponent::renderTtf(SDL_Renderer* renderer) {
    loadFont();
    rebuildTexture(renderer);

    if (texture_ == nullptr) {
        renderFallback(renderer);
        return;
    }

    SDL_Rect destination = getAbsoluteContentRect();
    destination.w = textureWidth_;
    destination.h = textureHeight_;

    SDL_RenderCopy(renderer, texture_, nullptr, &destination);
}

void TextComponent::loadFont() {
    if (triedFontLoad_) {
        return;
    }

    triedFontLoad_ = true;
    font_ = TTF_OpenFont(style_.fontPath.c_str(), style_.fontSize);
}

void TextComponent::rebuildTexture(SDL_Renderer* renderer) {
    if (font_ == nullptr || texture_ != nullptr && textureRenderer_ == renderer) {
        return;
    }

    releaseTexture();

    SDL_Color color{style_.color.r, style_.color.g, style_.color.b, style_.color.a};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, value_.c_str(), color);
    if (surface == nullptr) {
        return;
    }

    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    textureRenderer_ = renderer;

    if (texture_ != nullptr) {
        textureWidth_ = surface->w;
        textureHeight_ = surface->h;

        if (autoWidth_ || autoHeight_) {
            const SDL_Rect current = getBaseRect();
            setSize(autoWidth_ ? textureWidth_ : current.w, autoHeight_ ? textureHeight_ : current.h);
        }
    }

    SDL_FreeSurface(surface);
}

void TextComponent::releaseTexture() noexcept {
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
        textureRenderer_ = nullptr;
        textureWidth_ = 0;
        textureHeight_ = 0;
    }
}

void TextComponent::releaseFont() noexcept {
    if (font_ != nullptr) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
}

void TextComponent::updateSelf(float) {}

const std::string& TextComponent::value() const noexcept {
    return value_;
}

Text::Text(std::string value, Rect rect, Color color, int scale)
    : component_(std::make_unique<TextComponent>(std::move(value), rect, color, scale)) {}

Text::Text(std::string value, TextStyle style, Rect rect)
    : component_(std::make_unique<TextComponent>(std::move(value), std::move(style), rect)) {}

std::unique_ptr<Component> Text::build() && {
    return std::move(component_);
}

} // namespace vireo
