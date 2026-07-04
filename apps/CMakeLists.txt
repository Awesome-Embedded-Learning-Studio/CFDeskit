# CFDeskit: standalone desktop apps. Each entry is an independent executable
# that CFDesktop (or any launcher) launches via QProcess — they are not linked
# into any desktop shell, so a crash or hang cannot take down the shell.

# Shared app headers (e.g. abi_check.hpp) — visible to every app subdirectory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(calculator)
add_subdirectory(noter)
add_subdirectory(alarm_clock)
add_subdirectory(calendar)
add_subdirectory(system_state)

# ============================================================
# ABI deployment isolation
# ============================================================
# Apps resolve libquarkwidgets.so via rpath $ORIGIN/.. (set globally via
# CMAKE_INSTALL_RPATH + CMAKE_BUILD_WITH_INSTALL_RPATH in the top-level
# CMakeLists), so each app loads the .so from its own apps/ tree — fully
# decoupled from any desktop's bin/. See plan §ABI 防护 (部署隔离).
#
# Co-locate the shared libquarkwidgets.so under apps/ so the build tree
# mirrors the install layout and rpath resolves identically in both.
set_target_properties(quarkwidgets PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/apps")

# ============================================================
# Install layout (deployment isolation)
# ============================================================
# <prefix>/apps/<id>/{exe, app.json}  +  <prefix>/apps/libquarkwidgets.so
# Apps set INSTALL_RPATH=$ORIGIN/.. above so each resolves the shared .so
# from apps/, fully decoupled from any desktop's bin/libquarkwidgets.so.
foreach(app calculator noter alarm_clock calendar system_state)
    install(TARGETS ${app} RUNTIME DESTINATION apps/${app})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${app}/app.json DESTINATION apps/${app})
endforeach()
install(FILES $<TARGET_FILE:quarkwidgets> DESTINATION apps)
