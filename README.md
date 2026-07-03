# mini-apps

Standalone desktop apps for the CFDesktop ecosystem, built with **Qt6 + QuarkWidgets**.
Each app is an independent executable launched by CFDesktop (or any launcher) via
`QProcess` — never linked into the desktop shell, so an app crash or hang cannot
take down the shell.

## Apps

| App | Description |
|-----|-------------|
| `calculator` | Expression calculator (recursive-descent parser, QString-based) |
| `noter` | Plain-text notepad |
| `alarm_clock` | Alarm clock with editable alarm list |
| `calendar` | Calendar with per-date notes |
| `system_state` | CPU/memory monitor (probes vendored from CFDesktop cfbase — see below) |

## Build

```bash
cmake -B build -S .
cmake --build build -j
```

QuarkWidgets and aex are pulled as git submodules (`v0.1.0`). The build tree
mirrors the install layout: `build/apps/<id>/{exe, app.json}` +
`build/apps/libquarkwidgets.so`.

## Install (deployment)

```bash
cmake --install build --prefix /your/prefix
# Layout:
#   <prefix>/apps/<id>/{exe, app.json}
#   <prefix>/apps/libquarkwidgets.so
```

## ABI safety — deployment isolation + runtime self-check

Each app resolves `libquarkwidgets.so` from its own `apps/` tree via rpath
`$ORIGIN/..`, **fully decoupled from any desktop's `bin/libquarkwidgets.so`**.
App and desktop QuarkWidgets versions can therefore evolve independently — a
version mismatch is structurally harmless (each side loads its own .so).

As a runtime backstop, [`apps/abi_check.hpp`](apps/abi_check.hpp) verifies the
loaded .so's ABI version against the compile-time version and fails fast with
`qFatal` if a deploy ever swaps an incompatible .so — turning silent ABI UB
into an obvious, actionable error.

## system_state — vendored cfbase probes

[`apps/system_state/vendored-cfbase/`](apps/system_state/vendored-cfbase/)
contains a static copy of CFDesktop cfbase's CPU/memory probes, so `system_state`
has **zero runtime dependency on any external `libcfbase.so`** and evolves
independently from the main repo's cfbase. See
[`apps/system_state/README.md`](apps/system_state/README.md) for provenance and
sync policy.
