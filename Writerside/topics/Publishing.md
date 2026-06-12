# Publishing

This page covers two publishing paths:

- publishing Vireo as a C++ library;
- publishing the Writerside documentation with GitHub Pages.

## Library Releases

Use semantic version tags:

```bash
git tag v0.0.5
git push origin v0.0.5
```

On GitHub, create a release from the tag.
Attach source archives or binaries only if needed.

## CMake Installation

Install locally:

```bash
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
cmake --install cmake-build-release --prefix "$HOME/.local"
```

Consumers can then use:

```cmake
find_package(vireo CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE vireo::vireo)
```

The install rules provide:

- headers under the install include directory;
- the compiled library;
- `vireoTargets.cmake`;
- `vireoConfig.cmake`;
- `vireoConfigVersion.cmake`;
- `vireo.pc`.

## FetchContent Usage

For projects that do not want a local install:

```cmake
include(FetchContent)

FetchContent_Declare(
    vireo
    GIT_REPOSITORY https://github.com/TheScoutSky/Vireo.git
    GIT_TAG v0.0.5
)

FetchContent_MakeAvailable(vireo)

target_link_libraries(my_app PRIVATE vireo::vireo)
```

## Release Checklist

Before tagging:

```bash
cmake --preset release
cmake --build --preset release
ctest --test-dir cmake-build-release --output-on-failure
cmake --install cmake-build-release --prefix ./install-test
```

Then test a tiny external project that calls:

```cmake
find_package(vireo CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE vireo::vireo)
```

## Writerside Docs

The Writerside project lives in:

```text
Writerside/
```

Important files:

| File | Purpose |
|------|---------|
| `writerside.cfg` | Writerside project config. |
| `v.tree` | Documentation instance and table of contents. |
| `topics/*.md` | Documentation pages. |
| `cfg/buildprofiles.xml` | Writerside build profile. |

The active instance is:

```text
Writerside/v
```

## GitHub Pages Deployment

The repo contains a GitHub Actions workflow for Writerside:

```text
.github/workflows/docs.yml
```

The workflow:

1. checks out the repository;
2. builds the Writerside instance;
3. uploads the generated Writerside artifact;
4. unzips the website;
5. uploads the result as a GitHub Pages artifact;
6. deploys it with `actions/deploy-pages`.

In GitHub repository settings, Pages must use:

```text
Source: GitHub Actions
```

Do not use `Deploy from branch` with `/ root` or `/docs` for Writerside output.
That mode publishes files directly from the branch and can overwrite the generated Writerside site with README/Jekyll
output.

## Custom Domain

For a `www` subdomain:

```text
Type: CNAME
Name: www
Value: thescoutsky.github.io
```

For an apex domain, add GitHub Pages A records:

```text
185.199.108.153
185.199.109.153
185.199.110.153
185.199.111.153
```

Then set the custom domain in:

```text
Repository Settings -> Pages -> Custom domain
```

After DNS verification is green, enable:

```text
Enforce HTTPS
```

With GitHub Actions based Pages publishing, a repository-level `CNAME` file is not required for the current workflow.
