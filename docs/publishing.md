# Publishing Vireo

This is the simplest path to make Vireo installable by other C++ projects.

## 1. Put It On GitHub

Create a repository named `vireo`, then push this folder:

```bash
git init
git add .
git commit -m "Initial Vireo library foundation"
git branch -M main
git remote add origin git@github.com:YOUR_NAME/vireo.git
git push -u origin main
```

Keep the public headers under `include/vireo`. That makes the include path stable:

```cpp
#include <vireo/vireo.hpp>
```

## 2. Create Releases

Use semantic versions:

```bash
git tag v0.0.5
git push origin v0.0.5
```

On GitHub, create a release from that tag and attach a source archive if needed.

## 3. Let Users Install With CMake

Users can clone and install:

```bash
git clone https://github.com/YOUR_NAME/vireo.git
cmake -S vireo -B vireo/build -DCMAKE_BUILD_TYPE=Release
cmake --build vireo/build
cmake --install vireo/build --prefix "$HOME/.local"
```

Then they consume it with:

```cmake
find_package(vireo CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE vireo::vireo)
```

## 4. Let Users Fetch It Directly

Users who do not want a system install can use CMake `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    vireo
    GIT_REPOSITORY https://github.com/YOUR_NAME/vireo.git
    GIT_TAG v0.0.5
)

FetchContent_MakeAvailable(vireo)

target_link_libraries(my_app PRIVATE vireo::vireo)
```

## 5. Package Manager Plan

Start with GitHub releases and CMake config files. When the API stabilizes:

- Add a Conan recipe so users can install with `conan install`.
- Add a vcpkg port so users can install with `vcpkg install vireo`.
- Keep SDL2 and the SDL2 add-ons as explicit dependencies instead of bundling them into the repository.

## Release Checklist

Before every tag:

```bash
cmake --preset release
cmake --build --preset release
ctest --test-dir cmake-build-release --output-on-failure
cmake --install cmake-build-release --prefix ./install-test
```

Then test a tiny external project that calls `find_package(vireo CONFIG REQUIRED)`.
