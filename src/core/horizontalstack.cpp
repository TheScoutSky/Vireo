#include <vireo/core/horizontalstack.h>

#include <algorithm>
#include <memory>
#include <vector>

namespace vireo {

HorizontalStackComponent::HorizontalStackComponent(Rect rect, int gap, int padding, Alignment alignment,
                                                   Justify justify)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), gap_(gap), padding_(padding), alignment_(alignment),
      justify_(justify) {}

HorizontalStackComponent::HorizontalStackComponent(FillTag, int gap, int padding, Alignment alignment, Justify justify)
    : HorizontalStackComponent(Rect{}, gap, padding, alignment, justify) {
    setFillParent();
}

void HorizontalStackComponent::render(SDL_Renderer* renderer) {
    applyLayoutConstraints();
    layoutChildren();
    Component::render(renderer);
}

void HorizontalStackComponent::handleSelf(SDL_Event*) {}

void HorizontalStackComponent::renderSelf(SDL_Renderer*) {}

void HorizontalStackComponent::updateSelf(float) {}

void HorizontalStackComponent::layoutChildren() {
    std::vector<Component*> children = getChildren();
    const SDL_Rect bounds = getBaseRect();
    const int availableWidth = std::max(0, bounds.w - 2 * padding_);
    const int availableHeight = std::max(0, bounds.h - 2 * padding_);

    int totalWidth = 0;
    int visibleChildren = 0;
    for (Component* child : children) {
        if (child == nullptr) {
            continue;
        }

        child->applyLayoutConstraints();
        totalWidth += child->getBaseRect().w;
        ++visibleChildren;
    }

    if (visibleChildren > 1 && justify_ != Justify::SpaceBetween) {
        totalWidth += gap_ * (visibleChildren - 1);
    }

    int cursorX = padding_;
    int gap = gap_;

    if (justify_ == Justify::Center) {
        cursorX += std::max(0, (availableWidth - totalWidth) / 2);
    } else if (justify_ == Justify::End) {
        cursorX += std::max(0, availableWidth - totalWidth);
    } else if (justify_ == Justify::SpaceBetween && visibleChildren > 1) {
        gap = std::max(0, (availableWidth - totalWidth) / (visibleChildren - 1));
    }

    for (Component* child : children) {
        if (child == nullptr) {
            continue;
        }

        child->applyLayoutConstraints();
        int childY = padding_;
        int childHeight = child->getBaseRect().h;

        if (alignment_ == Alignment::Center) {
            childY = padding_ + std::max(0, (availableHeight - childHeight) / 2);
        } else if (alignment_ == Alignment::End) {
            childY = padding_ + std::max(0, availableHeight - childHeight);
        } else if (alignment_ == Alignment::Stretch) {
            childHeight = availableHeight;
            child->setSize(child->getBaseRect().w, childHeight);
        }

        child->setPosition(cursorX, childY);
        cursorX += child->getBaseRect().w + gap;
    }
}

HorizontalStack::HorizontalStack(Rect rect, int gap, int padding, Alignment alignment, Justify justify)
    : ComponentBuilder(std::make_unique<HorizontalStackComponent>(rect, gap, padding, alignment, justify)) {}

HorizontalStack::HorizontalStack(FillTag fill, int gap, int padding, Alignment alignment, Justify justify)
    : ComponentBuilder(std::make_unique<HorizontalStackComponent>(fill, gap, padding, alignment, justify)) {}

} // namespace vireo
