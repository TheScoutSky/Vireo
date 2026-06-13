//
// Created by Antonio Wil on 12.06.26.
//

#ifndef VIREO_FOREACH_H
#define VIREO_FOREACH_H

#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include <vireo/core/component.h>
#include <vireo/core/geometry.hpp>
#include <vireo/core/horizontalstack.h>
#include <vireo/core/verticalstack.h>

namespace vireo {

enum class StackDirection {
    Horizontal,
    Vertical,
};

struct ForEachLayout {
    Rect rect{};
    bool autoSize{true};
    bool fillParent{false};
    int gap{12};
    int padding{0};
    Alignment alignment{Alignment::Start};
    Justify justify{Justify::Start};
};

template <typename Items, typename Factory> class ForEachBuilder {
  public:
    ForEachBuilder(StackDirection direction, Items items, Factory factory, ForEachLayout layout = {})
        : direction_(direction), items_(std::move(items)), factory_(std::move(factory)), layout_(layout) {}

    std::unique_ptr<Component> build() && {
        std::vector<std::unique_ptr<Component>> children;
        for (const auto& item : items_) {
            auto child = std::move(factory_(item)).build();
            if (child) {
                children.push_back(std::move(child));
            }
        }

        if (children.empty()) {
            return nullptr;
        }

        const Rect rect = layout_.autoSize ? measuredRect(children) : layout_.rect;

        if (direction_ == StackDirection::Horizontal) {
            return buildHorizontal(rect, std::move(children));
        }

        return buildVertical(rect, std::move(children));
    }

  private:
    [[nodiscard]] Rect measuredRect(const std::vector<std::unique_ptr<Component>>& children) const {
        if (direction_ == StackDirection::Horizontal) {
            return measuredHorizontalRect(children);
        }

        return measuredVerticalRect(children);
    }

    [[nodiscard]] Rect measuredHorizontalRect(const std::vector<std::unique_ptr<Component>>& children) const {
        int width = layout_.padding * 2;
        int height = 0;
        int visibleChildren = 0;

        for (const auto& child : children) {
            const SDL_Rect bounds = child->getBaseRect();
            width += bounds.w;
            height = std::max(height, bounds.h);
            ++visibleChildren;
        }

        if (visibleChildren > 1) {
            width += layout_.gap * (visibleChildren - 1);
        }

        return Rect{0, 0, width, height + layout_.padding * 2};
    }

    [[nodiscard]] Rect measuredVerticalRect(const std::vector<std::unique_ptr<Component>>& children) const {
        int width = 0;
        int height = layout_.padding * 2;
        int visibleChildren = 0;

        for (const auto& child : children) {
            const SDL_Rect bounds = child->getBaseRect();
            width = std::max(width, bounds.w);
            height += bounds.h;
            ++visibleChildren;
        }

        if (visibleChildren > 1) {
            height += layout_.gap * (visibleChildren - 1);
        }

        return Rect{0, 0, width + layout_.padding * 2, height};
    }

    std::unique_ptr<Component> buildHorizontal(Rect rect, std::vector<std::unique_ptr<Component>> children) {
        HorizontalStack stack = layout_.fillParent
                                    ? HorizontalStack(Fill, layout_.gap, layout_.padding, layout_.alignment,
                                                      layout_.justify)
                                    : HorizontalStack(rect, layout_.gap, layout_.padding, layout_.alignment,
                                                      layout_.justify);

        for (auto& child : children) {
            stack.get()->addChild(std::move(child));
        }

        return std::move(stack).build();
    }

    std::unique_ptr<Component> buildVertical(Rect rect, std::vector<std::unique_ptr<Component>> children) {
        VerticalStack stack = layout_.fillParent
                                  ? VerticalStack(Fill, layout_.gap, layout_.padding, layout_.alignment,
                                                  layout_.justify)
                                  : VerticalStack(rect, layout_.gap, layout_.padding, layout_.alignment,
                                                  layout_.justify);

        for (auto& child : children) {
            stack.get()->addChild(std::move(child));
        }

        return std::move(stack).build();
    }

    StackDirection direction_;
    Items items_;
    Factory factory_;
    ForEachLayout layout_;
};

template <typename Items, typename Factory>
auto ForEach(StackDirection direction, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
             Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEachBuilder<std::decay_t<Items>, std::decay_t<Factory>>(
        direction, std::forward<Items>(items), std::forward<Factory>(factory),
        ForEachLayout{{}, true, false, gap, padding, alignment, justify});
}

template <typename Items, typename Factory>
auto ForEach(StackDirection direction, Rect rect, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
             Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEachBuilder<std::decay_t<Items>, std::decay_t<Factory>>(
        direction, std::forward<Items>(items), std::forward<Factory>(factory),
        ForEachLayout{rect, false, false, gap, padding, alignment, justify});
}

template <typename Items, typename Factory>
auto ForEach(StackDirection direction, FillTag, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
             Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEachBuilder<std::decay_t<Items>, std::decay_t<Factory>>(
        direction, std::forward<Items>(items), std::forward<Factory>(factory),
        ForEachLayout{{}, false, true, gap, padding, alignment, justify});
}

template <typename Items, typename Factory>
auto VForEach(Items&& items, Factory&& factory, int gap = 12, int padding = 0,
              Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEach(StackDirection::Vertical, std::forward<Items>(items), std::forward<Factory>(factory), gap, padding,
                   alignment, justify);
}

template <typename Items, typename Factory>
auto VForEach(Rect rect, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
              Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEach(StackDirection::Vertical, rect, std::forward<Items>(items), std::forward<Factory>(factory), gap,
                   padding, alignment, justify);
}

template <typename Items, typename Factory>
auto VForEach(FillTag fill, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
              Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEach(StackDirection::Vertical, fill, std::forward<Items>(items), std::forward<Factory>(factory), gap,
                   padding, alignment, justify);
}

template <typename Items, typename Factory>
auto HForEach(Items&& items, Factory&& factory, int gap = 12, int padding = 0,
              Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEach(StackDirection::Horizontal, std::forward<Items>(items), std::forward<Factory>(factory), gap,
                   padding, alignment, justify);
}

template <typename Items, typename Factory>
auto HForEach(Rect rect, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
              Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEach(StackDirection::Horizontal, rect, std::forward<Items>(items), std::forward<Factory>(factory), gap,
                   padding, alignment, justify);
}

template <typename Items, typename Factory>
auto HForEach(FillTag fill, Items&& items, Factory&& factory, int gap = 12, int padding = 0,
              Alignment alignment = Alignment::Start, Justify justify = Justify::Start) {
    return ForEach(StackDirection::Horizontal, fill, std::forward<Items>(items), std::forward<Factory>(factory), gap,
                   padding, alignment, justify);
}

} // namespace vireo

#endif // VIREO_FOREACH_H
