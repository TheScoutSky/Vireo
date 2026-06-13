//
// Created by Antonio Wil on 09.06.26.
//

#include <vireo/core/component.h>

#include <algorithm>

namespace vireo {

// ----------------------------------- //
// Construction                        //
// ----------------------------------- //

Component::Component(const int x, const int y, const int width, const int height, const int margin, const int marginX,
                     const int marginY) {
    base = SDL_Rect{x, y, width, height};
    this->margin = margin;
    this->marginX = marginX;
    this->marginY = marginY;
    rebuildContentRect();
}

// ----------------------------------- //
// Event Handling                      //
// ----------------------------------- //

void Component::handleEvents(SDL_Event* event) {
    if (event == nullptr) {
        return;
    }

    handleSelf(event);

    for (const auto& [id, child] : children) {
        child->handleEvents(event);
    }
}

// ----------------------------------- //
// Child - Parent                      //
// ----------------------------------- //

void Component::addChild(const int id, Component* child) {
    if (child != nullptr) {
        child->parent = this;
        children[id] = child;
        markDirty();
    }
}

Component* Component::addChild(std::unique_ptr<Component> child) {
    return addChild(nextChildId++, std::move(child));
}

Component* Component::addChild(const int id, std::unique_ptr<Component> child) {
    if (!child) {
        return nullptr;
    }

    removeChild(id);

    Component* rawChild = child.get();
    ownedChildren.push_back(std::move(child));
    addChild(id, rawChild);
    return rawChild;
}

void Component::removeChild(const int id) {
    const auto it = children.find(id);
    if (it != children.end() && it->second != nullptr) {
        it->second->parent = nullptr;
        Component* rawChild = it->second;
        children.erase(it);

        ownedChildren.erase(std::remove_if(ownedChildren.begin(), ownedChildren.end(),
                                           [rawChild](const std::unique_ptr<Component>& ownedChild) {
                                               return ownedChild.get() == rawChild;
                                           }),
                            ownedChildren.end());
        markDirty();
        return;
    }

    children.erase(id);
    markDirty();
}

void Component::removeChild(Component* child) {
    if (child != nullptr) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (it->second == child) {
                child->parent = nullptr;
                children.erase(it);

                ownedChildren.erase(std::remove_if(ownedChildren.begin(), ownedChildren.end(),
                                                   [child](const std::unique_ptr<Component>& ownedChild) {
                                                       return ownedChild.get() == child;
                                                   }),
                                    ownedChildren.end());
                markDirty();
                break;
            }
        }
    }
}

void Component::removeAllChildren() {
    for (auto& [id, child] : children) {
        if (child != nullptr) {
            child->parent = nullptr;
        }
    }
    children.clear();
    ownedChildren.clear();
    markDirty();
}

std::vector<Component*> Component::getChildren() {
    std::vector<Component*> result;
    result.reserve(children.size());
    for (const auto& [id, child] : children) {
        result.push_back(child);
    }
    return result;
}

Component* Component::getChild(const int id) {
    auto it = children.find(id);
    if (it != children.end()) {
        return it->second;
    }
    return nullptr;
}

Component* Component::getParent() const {
    return parent;
}

bool Component::fillsParent() const {
    return fillParent;
}

void Component::applyLayoutConstraints() {
    if (!fillParent || parent == nullptr) {
        return;
    }

    const SDL_Rect parentBase = parent->base;
    const SDL_Rect parentContent = parent->content;
    const SDL_Rect fillBounds{
        parentContent.x - parentBase.x,
        parentContent.y - parentBase.y,
        std::max(0, parentContent.w),
        std::max(0, parentContent.h),
    };

    if (base.x == fillBounds.x && base.y == fillBounds.y && base.w == fillBounds.w && base.h == fillBounds.h) {
        return;
    }

    setBounds(fillBounds);
}

// ----------------------------------- //
// Rendering                           //
// ----------------------------------- //

void Component::render(SDL_Renderer* renderer) {
    applyLayoutConstraints();
    renderSelf(renderer);
    for (const auto& [id, child] : children) {
        child->render(renderer);
    }
}

void Component::update(const float deltaTime) {
    updateSelf(deltaTime);
    for (const auto& [id, child] : children) {
        child->update(deltaTime);
    }
}

void Component::markDirty() {
    dirty = true;
    if (parent) {
        parent->markDirty();
    }
}

void Component::clearDirty() {
    dirty = false;
}

bool Component::isDirty() const {
    return dirty;
}

const SDL_Rect& Component::getBaseRect() const {
    return base;
}

const SDL_Rect& Component::getContentRect() const {
    return content;
}

SDL_Rect Component::getAbsoluteBaseRect() const {
    SDL_Rect result = base;

    for (const Component* current = parent; current != nullptr; current = current->parent) {
        result.x += current->base.x;
        result.y += current->base.y;
    }

    return result;
}

SDL_Rect Component::getAbsoluteContentRect() const {
    SDL_Rect result = content;

    for (const Component* current = parent; current != nullptr; current = current->parent) {
        result.x += current->base.x;
        result.y += current->base.y;
    }

    return result;
}

void Component::setPosition(int x, int y) {
    base.x = x;
    base.y = y;
    rebuildContentRect();
    markDirty();
}

void Component::setSize(int width, int height) {
    base.w = width;
    base.h = height;
    rebuildContentRect();
    markDirty();
}

void Component::setBounds(const SDL_Rect& rect) {
    base = rect;
    rebuildContentRect();
    markDirty();
}

void Component::setPadding(EdgeInsets padding) {
    paddingInsets = padding;
    rebuildContentRect();
    markDirty();
}

void Component::setFillParent(bool fill) {
    if (fillParent == fill) {
        return;
    }

    fillParent = fill;
    markDirty();
}

void Component::rebuildContentRect() {
    const int leftPadding = marginX + margin + paddingX + padding + paddingInsets.left;
    const int topPadding = marginY + margin + paddingY + padding + paddingInsets.top;
    const int rightPadding = marginX + margin + paddingX + padding + paddingInsets.right;
    const int bottomPadding = marginY + margin + paddingY + padding + paddingInsets.bottom;

    content = SDL_Rect{
        base.x + leftPadding,
        base.y + topPadding,
        base.w - leftPadding - rightPadding,
        base.h - topPadding - bottomPadding,
    };
}

} // namespace vireo
