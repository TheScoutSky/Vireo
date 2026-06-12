//
// Created by Antonio Wil on 12.06.26.
//

#ifndef VIREO_WHEN_H
#define VIREO_WHEN_H

#include <memory>
#include <type_traits>
#include <utility>

#include <vireo/core/component.h>

namespace vireo {

template <typename Then> class WhenBuilder {
  public:
    WhenBuilder(bool condition, Then thenComponent)
        : condition_(condition), thenComponent_(std::move(thenComponent)) {}

    std::unique_ptr<Component> build() && {
        if (condition_) {
            return std::move(thenComponent_).build();
        }
        return nullptr;
    }

  private:
    bool condition_;
    Then thenComponent_;
};

template <typename Then, typename Else> class WhenElseBuilder {
  public:
    WhenElseBuilder(bool condition, Then thenComponent, Else elseComponent)
        : condition_(condition), thenComponent_(std::move(thenComponent)),
          elseComponent_(std::move(elseComponent)) {}

    std::unique_ptr<Component> build() && {
        if (condition_) {
            return std::move(thenComponent_).build();
        }

        return std::move(elseComponent_).build();
    }

  private:
    bool condition_;
    Then thenComponent_;
    Else elseComponent_;
};

template <typename Then>
auto When(bool condition, Then&& thenComponent) {
    return WhenBuilder<std::decay_t<Then>>(condition, std::forward<Then>(thenComponent));
}

template <typename Then, typename Else>
auto When(bool condition, Then&& thenComponent, Else&& elseComponent) {
    return WhenElseBuilder<std::decay_t<Then>, std::decay_t<Else>>(
        condition, std::forward<Then>(thenComponent), std::forward<Else>(elseComponent));
}

} // namespace vireo

#endif // VIREO_WHEN_H
