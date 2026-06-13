//
// Created by Antonio Wil on 10.06.26.
//

#ifndef VIREO_COMPONENTBUILDER_H
#define VIREO_COMPONENTBUILDER_H

#include <memory>
#include <type_traits>
#include <utility>

#include "component.h"

namespace vireo {

template <typename Self> class ComponentBuilder {
  public:
    template <typename... Children> Self&& operator()(Children&&... children) && {
        (this->addChild(std::forward<Children>(children)), ...);
        return static_cast<Self&&>(*this);
    }

    [[nodiscard]] std::unique_ptr<Component> build() && {
        return std::move(component_);
    }

    [[nodiscard]] Component* get() const noexcept {
        return component_.get();
    }

    Self&& fill() && {
        component_->setFillParent(true);
        return static_cast<Self&&>(*this);
    }

  protected:
    ComponentBuilder() = default;

    explicit ComponentBuilder(std::unique_ptr<Component> component) : component_(std::move(component)) {}

    template <typename Child> void addChild(Child&& child) {
        if constexpr (std::is_same_v<std::decay_t<Child>, std::unique_ptr<Component>>) {
            component_->addChild(std::forward<Child>(child));
        } else {
            component_->addChild(std::forward<Child>(child).build());
        }
    }

    std::unique_ptr<Component> component_;
};

} // namespace vireo

#endif // VIREO_COMPONENTBUILDER_H
