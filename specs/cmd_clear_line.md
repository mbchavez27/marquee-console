# Command: `clear_line`

**Pre-conditions:** None.
**Trigger:** User types "clear_line".
**Action:**

1. Erase the current console line without mutating shared state (`is_running`, `is_app_alive`, `speed_ms`, `marquee_text` unchanged, no mutex required unless printing).
2. Cross-platform implementation via `Marquee::clear_line()`:
   - Linux/macOS: output `"\033[2K\r"` + `flush`.
   - Windows (`_WIN32`): enable `ENABLE_VIRTUAL_TERMINAL_PROCESSING` via `GetConsoleMode`/`SetConsoleMode`, then same ANSI sequence; fallback to `"\r" + string(80,' ') + "\r"` + `flush` if VT unavailable.
3. Callable while marquee is running or stopped; if running, next worker frame redraws normally.
   **Post-conditions:** Current line is visually blank, cursor at col 0, prompt awaits next command. No state change.

# Integration notes (for implementation step, not part of spec file):

- `include/Marquee.h`: public `void clear_line();`
- `src/CommandHandler.cpp:run()`: `else if (command == "clear") { marquee.clear_line(); }`
- `src/CommandHandler.cpp:print_help()`: add `clear - clear current console line`
- `specs/cmd_help.md:7`: append `clear` to list; `specs/master_specs.md §4`: add `7. clear - erase current line.
