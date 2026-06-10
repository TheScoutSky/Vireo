//
// Created by Antonio Wil on 09.06.26.
//

#ifndef VIREO_COMPONENT_H
#define VIREO_COMPONENT_H

#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_render.h>

#include <map>
#include <memory>
#include <vector>

#include <vireo/core/export.hpp>
#include <vireo/core/style.h>

namespace vireo {

class VIREO_API Component {

  public:
    // --- Variables --- //
    int margin = 0, marginX = 0, marginY = 0, padding = 0, paddingX = 0, paddingY = 0, borderWidth = 0;

    // --- Construction --- //
    Component(int x, int y, int width, int height, int margin, int marginX, int marginY);
    virtual ~Component() = default;

    // --- Event Handling --- //
    void handleEvents(SDL_Event* event);
    virtual void handleSelf(SDL_Event* event) = 0;

    // --- Child Parent Logic --- //
    void addChild(int id, Component* child);
    Component* addChild(std::unique_ptr<Component> child);
    Component* addChild(int id, std::unique_ptr<Component> child);
    void removeChild(Component* child);
    void removeChild(int id);
    void removeAllChildren();

    // --- Render Logic --- //
    virtual void render(SDL_Renderer* renderer);
    virtual void renderSelf(SDL_Renderer* renderer) = 0;

    void update(float deltaTime);
    virtual void updateSelf(float deltaTime) = 0;

    void markDirty();
    void clearDirty();
    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] const SDL_Rect& getBaseRect() const;
    [[nodiscard]] const SDL_Rect& getContentRect() const;
    [[nodiscard]] SDL_Rect getAbsoluteBaseRect() const;
    [[nodiscard]] SDL_Rect getAbsoluteContentRect() const;

    void setPosition(int x, int y);
    void setSize(int width, int height);
    void setBounds(const SDL_Rect& rect);
    void setPadding(EdgeInsets padding);

    std::vector<Component*> getChildren();
    Component* getChild(int id);
    [[nodiscard]] Component* getParent() const;

  private:
    // --- Variables --- //
    bool dirty = false;

    SDL_Rect base{};
    SDL_Rect content{};
    EdgeInsets paddingInsets{};

    std::map<int, Component*> children;
    std::vector<std::unique_ptr<Component>> ownedChildren;
    Component* parent = nullptr;
    int nextChildId = 0;

    void rebuildContentRect();
};

} // namespace vireo

#endif // VIREO_COMPONENT_H
