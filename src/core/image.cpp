#include <vireo/core/image.h>

#include <SDL_image.h>

#include <algorithm>
#include <memory>
#include <utility>

namespace vireo {

ImageComponent::ImageComponent(std::string path, Rect rect, ImageFit fit)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), path_(std::move(path)), fit_(fit) {}

ImageComponent::~ImageComponent() {
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void ImageComponent::handleSelf(SDL_Event*) {}

void ImageComponent::renderSelf(SDL_Renderer* renderer) {
    loadTexture(renderer);

    if (texture_ == nullptr) {
        renderPlaceholder(renderer);
        return;
    }

    SDL_Rect bounds = getAbsoluteBaseRect();
    SDL_Rect destination = fittedRect(bounds);

    if (fit_ == ImageFit::Cover) {
        SDL_RenderSetClipRect(renderer, &bounds);
        SDL_RenderCopy(renderer, texture_, nullptr, &destination);
        SDL_RenderSetClipRect(renderer, nullptr);
        return;
    }

    SDL_RenderCopy(renderer, texture_, nullptr, &destination);
}

void ImageComponent::updateSelf(float) {}

void ImageComponent::loadTexture(SDL_Renderer* renderer) {
    if (triedLoad_) {
        return;
    }

    triedLoad_ = true;

    texture_ = IMG_LoadTexture(renderer, path_.c_str());
    if (texture_ == nullptr) {
        return;
    }

    SDL_QueryTexture(texture_, nullptr, nullptr, &textureWidth_, &textureHeight_);
}

void ImageComponent::renderPlaceholder(SDL_Renderer* renderer) {
    SDL_Rect bounds = getAbsoluteBaseRect();

    SDL_SetRenderDrawColor(renderer, 44, 50, 64, 255);
    SDL_RenderFillRect(renderer, &bounds);

    SDL_SetRenderDrawColor(renderer, 120, 132, 156, 255);
    SDL_RenderDrawRect(renderer, &bounds);
    SDL_RenderDrawLine(renderer, bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h);
    SDL_RenderDrawLine(renderer, bounds.x + bounds.w, bounds.y, bounds.x, bounds.y + bounds.h);
}

SDL_Rect ImageComponent::fittedRect(const SDL_Rect& bounds) const {
    if (fit_ == ImageFit::Stretch || textureWidth_ <= 0 || textureHeight_ <= 0) {
        return bounds;
    }

    const double widthRatio = static_cast<double>(bounds.w) / static_cast<double>(textureWidth_);
    const double heightRatio = static_cast<double>(bounds.h) / static_cast<double>(textureHeight_);
    const double scale =
        fit_ == ImageFit::Cover ? std::max(widthRatio, heightRatio) : std::min(widthRatio, heightRatio);

    const int width = std::max(1, static_cast<int>(textureWidth_ * scale));
    const int height = std::max(1, static_cast<int>(textureHeight_ * scale));

    return SDL_Rect{
        bounds.x + (bounds.w - width) / 2,
        bounds.y + (bounds.h - height) / 2,
        width,
        height,
    };
}

Image::Image(std::string path, Rect rect, ImageFit fit)
    : component_(std::make_unique<ImageComponent>(std::move(path), rect, fit)) {}

std::unique_ptr<Component> Image::build() && {
    return std::move(component_);
}

} // namespace vireo
