//
// Created by Antonio Wil on 12.06.26.
//

#ifndef VIREO_FOREACH_H
#define VIREO_FOREACH_H
#include "horizontalstack.h"
#include "verticalstack.h"

#include <algorithm>

namespace vireo {

enum class StackDirection {
    Horizontal,
    Vertical
};


template <typename Items, typename Factory>
class ForEachBuilder {

public:

    ForEachBuilder(StackDirection dir, Items items, Factory factory)
        : dir_(dir), items_(std::move(items)), factory_(std::move(factory)) {}


        std::unique_ptr<Component> build() && {
        if (items_.empty()) {
            return nullptr;
        }

        if (dir_ == StackDirection::Horizontal) {
            HorizontalStack stack;
            for (const auto& item : items_) {
                stack.get()->addChild(std::move(factory_(item)).build());
            }
            return std::move(stack).build();
        }
        VerticalStack stack;
        for (const auto& item : items_) {
            stack.get()->addChild(std::move(factory_(item)).build());
        }
        return std::move(stack).build();
        }

private:
    Items items_;
    Factory factory_;
    StackDirection dir_;

};

template <typename Items, typename Factory>
auto ForEach(StackDirection dir, Items&& items, Factory&& factory) {
    return ForEachBuilder<std::decay_t<Items>, std::decay_t<Factory>>(dir, std::forward<Items>(items), std::forward<Factory>(factory));
};

template <typename Items, typename Factory>
auto VForEach(Items&& items, Factory&& factory) {
    return ForEachBuilder<std::decay_t<Items>, std::decay_t<Factory>>(StackDirection::Vertical, std::forward<Items>(items), std::forward<Factory>(factory));
};

template <typename Items, typename Factory>
auto HForEach(Items&& items, Factory&& factory) {
    return ForEachBuilder<std::decay_t<Items>, std::decay_t<Factory>>(StackDirection::Horizontal, std::forward<Items>(items), std::forward<Factory>(factory));
};
}

#endif // VIREO_FOREACH_H
