# system_state — vendored cfbase probes

`vendored-cfbase/` is a static copy of CFDesktop cfbase's CPU/memory probes.
It lets `system_state` run with **zero runtime dependency on any external
`libcfbase.so`** and evolve independently from the main repo's cfbase.

## Provenance

- **Source**: CFDesktop `base/` (cfbase)
- **Commit**: `0a5c8ada6` (2026-07-03)
- **Scope**: `system/cpu/` + `system/memory/` (incl. private `win_impl/`,
  `linux_impl/`) + shared utils (`include/base/{export.h, linux/proc_parser.h,
  windows/co_helper.hpp, windows/common.h}`, `utils/linux/proc_parser.cpp`)
- **Size**: ~48 files / ~3.8k LOC
- **Built as**: `mini_system_probe` STATIC lib — see
  [`vendored-cfbase/CMakeLists.txt`](vendored-cfbase/CMakeLists.txt)

## Sync policy

This copy does **not** auto-track upstream cfbase. To pick up probe improvements:

1. Diff `vendored-cfbase/` against `base/` in the CFDesktop main repo.
2. Manually update the relevant files here.
3. Bump the commit reference above.

The `cf::` namespace and `CF_BASE_EXPORT` macro are preserved verbatim —
isolation is by physical separation, not renaming. The two copies are expected
to drift independently; that is the point.
