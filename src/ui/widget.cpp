#include <vireo/ui/widget.hpp>

namespace vireo {

bool Widget::handle_event(const SDL_Event&) {
    return false;
}

void Widget::update(double) {}

} // namespace vireo
