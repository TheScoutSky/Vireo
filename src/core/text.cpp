#include <vireo/core/text.h>

#include <SDL_rect.h>

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

[[nodiscard]] Rect normalizedTextRect(const std::string& value, Rect rect, int scale) noexcept {
    if (rect.width <= 0) {
        rect.width = static_cast<int>(value.size()) * 6 * scale;
    }

    if (rect.height <= 0) {
        rect.height = 7 * scale;
    }

    return rect;
}

} // namespace

TextComponent::TextComponent(std::string value, Rect rect, Color color, int scale)
    : Component(normalizedTextRect(value, rect, scale).x, normalizedTextRect(value, rect, scale).y,
                normalizedTextRect(value, rect, scale).width, normalizedTextRect(value, rect, scale).height, 0, 0, 0),
      value_(std::move(value)), color_(color), scale_(scale) {}

void TextComponent::handleSelf(SDL_Event*) {}

void TextComponent::renderSelf(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);

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
                    cursorX + column * scale_,
                    origin.y + row * scale_,
                    scale_,
                    scale_,
                };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }

        cursorX += 6 * scale_;
    }
}

void TextComponent::updateSelf(float) {}

const std::string& TextComponent::value() const noexcept {
    return value_;
}

Text::Text(std::string value, Rect rect, Color color, int scale)
    : component_(std::make_unique<TextComponent>(std::move(value), rect, color, scale)) {}

std::unique_ptr<Component> Text::build() && {
    return std::move(component_);
}

} // namespace vireo
