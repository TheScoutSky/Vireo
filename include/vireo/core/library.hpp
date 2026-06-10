#pragma once

#include <string_view>

#include <vireo/core/export.hpp>

namespace vireo {

[[nodiscard]] VIREO_API std::string_view name() noexcept;
[[nodiscard]] VIREO_API std::string_view version() noexcept;

} // namespace vireo
