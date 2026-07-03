# mini_system_probe — vendored copy of CFDesktop cfbase's CPU/memory probes.
#
# Evolves INDEPENDENTLY from the main repo's cfbase. Brought in at a pinned
# commit and kept as a STATIC lib so that `system_state` has zero runtime
# dependency on any external libcfbase.so (fully self-contained executable).
# See apps/system_state/README.md for provenance + sync policy.

# Platform-agnostic dispatch layer (cfcpu.cpp, memory_info.cpp, ...).
file(GLOB MINI_PROBE_COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/system/cpu/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/memory/*.cpp")

# Platform-specific impls. win_impl/ and linux_impl/ contain same-named files
# (cpu_info.cpp, ...), so we MUST select by platform to avoid duplicate symbols.
if(WIN32)
    file(GLOB MINI_PROBE_PLATFORM_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/system/cpu/private/win_impl/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/system/memory/private/win_impl/*.cpp")
else()
    file(GLOB MINI_PROBE_PLATFORM_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/system/cpu/private/linux_impl/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/system/memory/private/linux_impl/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/utils/linux/*.cpp")
endif()

add_library(mini_system_probe STATIC
    ${MINI_PROBE_COMMON_SOURCES}
    ${MINI_PROBE_PLATFORM_SOURCES}
)

target_include_directories(mini_system_probe PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mini_system_probe PUBLIC
    aex::aex
    Qt6::Core
)

if(WIN32)
    # CF_BASE_EXPORT resolves to dllexport only when CFBASE_EXPORTS is defined;
    # required so probe symbols enter the static archive.
    # NOTE: consumers of a STATIC lib would otherwise see dllimport — the
    # vendored export.h needs a CF_BASE_STATIC branch for full Windows support
    # (tracked as follow-up; Linux works fine as visibility("default")).
    target_compile_definitions(mini_system_probe PRIVATE CFBASE_EXPORTS)
    target_link_libraries(mini_system_probe PRIVATE wbemuuid PowrProf pdh psapi)
endif()
