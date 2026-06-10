#include <vireo/core/library.hpp>
#include <vireo/version.hpp>

namespace vireo {

std::string_view name() noexcept {
    return "vireo";
}

std::string_view version() noexcept {
    return VIREO_VERSION_STRING;
}

} // namespace vireo
