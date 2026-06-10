cmake_minimum_required(VERSION 3.20)

project(vireo
    VERSION 0.0.2
    DESCRIPTION "A lightweight SDL2 UI framework foundation"
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(VIREO_BUILD_EXAMPLES "Build Vireo example applications" ${PROJECT_IS_TOP_LEVEL})
option(VIREO_BUILD_TESTS "Build Vireo smoke tests" ${PROJECT_IS_TOP_LEVEL})

set(SDL2_USER_PREFIX "$ENV{HOME}/.local/sdl2")
if(EXISTS "${SDL2_USER_PREFIX}/lib/cmake/SDL2/SDL2Config.cmake")
    list(PREPEND CMAKE_PREFIX_PATH "${SDL2_USER_PREFIX}")
endif()

find_package(SDL2 2.0 CONFIG REQUIRED)
find_package(SDL2_image 2.0 CONFIG REQUIRED)

add_library(vireo
    src/core/button.cpp
    src/core/center.cpp
    src/core/checkbox.cpp
    src/core/component.cpp
    src/core/divider.cpp
    src/core/image.cpp
    src/core/horizontalstack.cpp
    src/core/library.cpp
    src/core/panel.cpp
    src/core/screen.cpp
    src/core/spacer.cpp
    src/core/text.cpp
    src/core/verticalstack.cpp
    src/platform/renderer.cpp
    src/platform/sdl_context.cpp
    src/platform/window.cpp
    src/ui/widget.cpp
)
add_library(vireo::vireo ALIAS vireo)

target_compile_features(vireo PUBLIC cxx_std_17)

target_include_directories(vireo
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(vireo PUBLIC SDL2::SDL2 SDL2_image::SDL2_image)

target_compile_definitions(vireo
    PUBLIC
        $<$<BOOL:${BUILD_SHARED_LIBS}>:VIREO_SHARED>
    PRIVATE
        VIREO_BUILDING_LIBRARY
)

set_target_properties(vireo PROPERTIES
    EXPORT_NAME vireo
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(APPLE AND EXISTS "${SDL2_USER_PREFIX}/lib")
    set_target_properties(vireo PROPERTIES
        BUILD_RPATH "${SDL2_USER_PREFIX}/lib"
    )
endif()

if(VIREO_BUILD_EXAMPLES)
    add_executable(vireo_minimal examples/minimal.cpp)
    target_link_libraries(vireo_minimal PRIVATE vireo::vireo)

    add_executable(vireo_dsl_test examples/dsl_test.cpp)
    target_link_libraries(vireo_dsl_test PRIVATE vireo::vireo)
    target_compile_definitions(vireo_dsl_test PRIVATE VIREO_EXAMPLES_ASSET_DIR="${PROJECT_SOURCE_DIR}/examples/assets")

    if(TARGET SDL2::SDL2main)
        target_link_libraries(vireo_minimal PRIVATE SDL2::SDL2main)
        target_link_libraries(vireo_dsl_test PRIVATE SDL2::SDL2main)
    endif()
endif()

if(VIREO_BUILD_TESTS)
    enable_testing()

    add_executable(vireo_smoke_test tests/smoke.cpp)
    target_link_libraries(vireo_smoke_test PRIVATE vireo::vireo)

    add_test(NAME vireo_smoke_test COMMAND vireo_smoke_test)

    if(TARGET vireo_dsl_test)
        add_test(NAME vireo_dsl_test_once COMMAND vireo_dsl_test --once)
        set_tests_properties(vireo_dsl_test_once PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy")
    endif()
endif()

install(TARGETS vireo
    EXPORT vireoTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/vireoConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    cmake/vireoConfig.cmake.in
    "${PROJECT_BINARY_DIR}/vireoConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vireo
)

configure_file(
    cmake/vireo.pc.in
    "${PROJECT_BINARY_DIR}/vireo.pc"
    @ONLY
)

install(FILES
    "${PROJECT_BINARY_DIR}/vireoConfig.cmake"
    "${PROJECT_BINARY_DIR}/vireoConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vireo
)

install(EXPORT vireoTargets
    FILE vireoTargets.cmake
    NAMESPACE vireo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vireo
)

install(FILES
    "${PROJECT_BINARY_DIR}/vireo.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
