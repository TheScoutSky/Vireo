#include <vireo/core/library.hpp>
#include <vireo/version.hpp>

#include <string_view>

int main() {
    if (vireo::name() != std::string_view{"vireo"}) {
        return 1;
    }

    if (vireo::version().empty()) {
        return 2;
    }

    if (vireo::version_major != VIREO_VERSION_MAJOR) {
        return 3;
    }

    return 0;
}
