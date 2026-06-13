#include <vireo/core/verticalstack.h>

#include <algorithm>
#include <memory>
#include <vector>

namespace vireo {

VerticalStackComponent::VerticalStackComponent(Rect rect, int gap, int padding, Alignment alignment, Justify justify)
    : Component(rect.x, rect.y, rect.width, rect.height, 0, 0, 0), gap_(gap), padding_(padding), alignment_(alignment),
      justify_(justify) {}

VerticalStackComponent::VerticalStackComponent(FillTag, int gap, int padding, Alignment alignment, Justify justify)
    : VerticalStackComponent(Rect{}, gap, padding, alignment, justify) {
    setFillParent();
}

void VerticalStackComponent::render(SDL_Renderer* renderer) {
    applyLayoutConstraints();
    layoutChildren();
    Component::render(renderer);
}

void VerticalStackComponent::handleSelf(SDL_Event*) {}

void VerticalStackComponent::renderSelf(SDL_Renderer*) {}

void VerticalStackComponent::updateSelf(float) {}

void VerticalStackComponent::layoutChildren() {
    std::vector<Component*> children = getChildren();
    const SDL_Rect bounds = getBaseRect();
    const int availableWidth = std::max(0, bounds.w - 2 * padding_);
    const int availableHeight = std::max(0, bounds.h - 2 * padding_);

    int totalHeight = 0;
    int visibleChildren = 0;
    for (Component* child : children) {
        if (child == nullptr) {
            continue;
        }

        child->applyLayoutConstraints();
        totalHeight += child->getBaseRect().h;
        ++visibleChildren;
    }

    if (visibleChildren > 1 && justify_ != Justify::SpaceBetween) {
        totalHeight += gap_ * (visibleChildren - 1);
    }

    int cursorY = padding_;
    int gap = gap_;

    if (justify_ == Justify::Center) {
        cursorY += std::max(0, (availableHeight - totalHeight) / 2);
    } else if (justify_ == Justify::End) {
        cursorY += std::max(0, availableHeight - totalHeight);
    } else if (justify_ == Justify::SpaceBetween && visibleChildren > 1) {
        gap = std::max(0, (availableHeight - totalHeight) / (visibleChildren - 1));
    }

    for (Component* child : children) {
        if (child == nullptr) {
            continue;
        }

        child->applyLayoutConstraints();
        int childX = padding_;
        int childWidth = child->getBaseRect().w;

        if (alignment_ == Alignment::Center) {
            childX = padding_ + std::max(0, (availableWidth - childWidth) / 2);
        } else if (alignment_ == Alignment::End) {
            childX = padding_ + std::max(0, availableWidth - childWidth);
        } else if (alignment_ == Alignment::Stretch) {
            childWidth = availableWidth;
            child->setSize(childWidth, child->getBaseRect().h);
        }

        child->setPosition(childX, cursorY);
        cursorY += child->getBaseRect().h + gap;
    }
}

VerticalStack::VerticalStack(Rect rect, int gap, int padding, Alignment alignment, Justify justify)
    : ComponentBuilder(std::make_unique<VerticalStackComponent>(rect, gap, padding, alignment, justify)) {}

VerticalStack::VerticalStack(FillTag fill, int gap, int padding, Alignment alignment, Justify justify)
    : ComponentBuilder(std::make_unique<VerticalStackComponent>(fill, gap, padding, alignment, justify)) {}

} // namespace vireo
