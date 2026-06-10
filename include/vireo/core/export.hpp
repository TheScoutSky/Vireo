#pragma once

#if defined(_WIN32) && defined(VIREO_SHARED)
#if defined(VIREO_BUILDING_LIBRARY)
#define VIREO_API __declspec(dllexport)
#else
#define VIREO_API __declspec(dllimport)
#endif
#else
#define VIREO_API
#endif
