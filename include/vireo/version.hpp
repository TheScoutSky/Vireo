#pragma once

#define VIREO_VERSION_MAJOR 0
#define VIREO_VERSION_MINOR 0
#define VIREO_VERSION_PATCH 5
#define VIREO_VERSION_STRING "0.0.5"

namespace vireo {

inline constexpr int version_major = VIREO_VERSION_MAJOR;
inline constexpr int version_minor = VIREO_VERSION_MINOR;
inline constexpr int version_patch = VIREO_VERSION_PATCH;
inline constexpr const char* version_string = VIREO_VERSION_STRING;

} // namespace vireo
