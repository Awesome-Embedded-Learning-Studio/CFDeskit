cmake_minimum_required(VERSION 3.16)

project(cfdeskit
    VERSION 0.1.0
    DESCRIPTION "CFDeskit: standalone desktop apps built with Qt6 + QuarkWidgets"
    LANGUAGES CXX C
)

# ============================================================
# Qt6
# ============================================================
find_package(Qt6 6.8.3 REQUIRED COMPONENTS Core Gui Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ============================================================
# RPATH: build tree mirrors install layout (apps/libquarkwidgets.so +
# apps/<id>/exe), so apps resolve the .so via $ORIGIN/.. in BOTH trees.
# This structurally isolates app and desktop QuarkWidgets versions.
# ============================================================
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH "$ORIGIN/..")

# ============================================================
# C++23 (match CFDesktop main repo)
# ============================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Third-party helper (same machinery as CFDesktop main repo).
# setup_third_party() prefers a checked-out third_party/<NAME>/ if present,
# otherwise FetchContent-declares it from git.
# ============================================================
include(cmake/third_party_helper.cmake)

# aex — header-only foundation (probe uses aex/scope_guard, aex/expected).
setup_third_party(
    NAME           aex
    GIT_REPOSITORY https://github.com/Awesome-Embedded-Learning-Studio/aex.git
    GIT_TAG        v0.1.0)

# QuarkWidgets — Material Design 3 UI framework.
setup_third_party(
    NAME           QuarkWidgets
    GIT_REPOSITORY https://github.com/Awesome-Embedded-Learning-Studio/QuarkWidgets.git
    GIT_TAG        v0.1.0)

# ============================================================
# Apps (each is an independent executable)
# ============================================================
add_subdirectory(apps)

# ============================================================
# Tests (GoogleTest, fetched via FetchContent when CFDESKIT_BUILD_TESTS=ON)
# ============================================================
option(CFDESKIT_BUILD_TESTS "Build CFDeskit tests" ON)
if(CFDESKIT_BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0)
    set(gtest_force_shared_rtc ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
    add_subdirectory(test)
endif()
