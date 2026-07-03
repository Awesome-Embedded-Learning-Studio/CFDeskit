/**
 * @file abi_check.hpp
 * @brief QuarkWidgets ABI runtime self-check for mini-apps.
 *
 * Call @ref mini_apps::check_quarkwidgets_abi at the very start of main(),
 * before any QuarkWidgets symbol is touched. If the loaded libquarkwidgets.so
 * was swapped at deploy time for one built against a different ABI version,
 * this fails fast with a clear qFatal message instead of risking ABI UB.
 *
 * See plan §ABI 防护 (运行时自检).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <cstring>

#include <QtGlobal>  // qFatal

#include "quarkwidgets_version.h"  // QUARKWIDGETS_ABI_VERSION + runtime fn

namespace mini_apps {

/**
 * @brief Verify the loaded libquarkwidgets.so matches the compile-time ABI.
 *
 * Compares @ref QUARKWIDGETS_ABI_VERSION (baked into the app at compile time)
 * against @ref quarkwidgets_runtime_abi_version (resolved from the loaded .so).
 *
 * On mismatch, calls qFatal — never returns. Failing loud here (before any
 * QuarkWidgets symbol is touched) turns a silent ABI UB into an obvious,
 * actionable deploy error.
 */
inline void check_quarkwidgets_abi() {
    const char* runtime = quarkwidgets_runtime_abi_version();
    if (runtime == nullptr || std::strcmp(runtime, QUARKWIDGETS_ABI_VERSION) != 0) {
        qFatal("QuarkWidgets ABI mismatch: app compiled against \"%s\", but the "
               "loaded libquarkwidgets.so reports \"%s\". A deploy swapped an "
               "incompatible .so - aborting to avoid ABI UB.",
               QUARKWIDGETS_ABI_VERSION,
               runtime ? runtime : "(null)");
    }
}

}  // namespace mini_apps
