# marquee-console

A multi-threaded C++ console app that displays scrolling text as a 5-row ASCII banner. Built for CSOPESY (OS emulator exercise).

## What it does

Prompts for commands in a `Command > ` loop and renders a continuously scrolling ASCII banner at the top of the terminal. Text and speed can be changed live while the marquee is running. Screen clearing pauses the marquee, wipes the viewport, redraws the banner, and resumes scrolling — no manual restart needed.

## How it works

- **Two threads:** main thread blocks on `std::getline` for command input; worker thread runs a render-scroll-delay loop in the background.
- **Synchronization:** `std::atomic<bool>` for `is_running` / `is_app_alive`, `std::atomic<int>` for `speed_ms`, `std::mutex` + `std::lock_guard` for `marquee_text`. A `std::condition_variable` gates the worker sleep and wakes it on state changes.
- **Scrolling:** Each frame, `render_current_frame()` snapshots `marquee_text`, converts it to 5-row ASCII via `ascii_art::convert_to_ascii`, slices a viewport of width `term_width` using modular indexing `(scroll_offset + col) % total_width`, and writes the rows at terminal positions 1-5 via ANSI escape sequences (Win32 API fallback included).
- **Shared state:**

| Variable       | Type                | Guard                                                  |
| -------------- | ------------------- | ------------------------------------------------------ |
| `is_running`   | `std::atomic<bool>` | `exchange` in `start/stop_marquee`, checked by worker  |
| `is_app_alive` | `std::atomic<bool>` | `false` on `exit` to break loop and join worker        |
| `speed_ms`     | `std::atomic<int>`  | `store` via `set_speed`, read by worker for frame delay |
| `marquee_text` | `std::string`       | `std::mutex` + `std::lock_guard`, narrow scope         |
| `scroll_offset`| `std::size_t`       | Private to worker, incremented per frame               |

## Requirements

- Linux + `g++` with C++17 support
- `cmake` >= 3.16
- `pthread` (linked via `Threads::Threads`)

## Build & Run

```bash
cmake -S . -B build          # configure once
cmake --build build          # builds ./build/marquee_app
./build/marquee_app          # run
cmake --build build --target run  # build + run in one step
```

## Commands

| Command         | Action                                                                                     |
| --------------- | ------------------------------------------------------------------------------------------ |
| `help`          | Print the 7 supported commands.                                                           |
| `start_marquee` | Start continuous background scrolling. Prints `Marquee is already running.` if already on. |
| `stop_marquee`  | Pause worker, clear the 5 banner rows. Prints `Marquee is already stopped.` if already off.|
| `set_text`      | Prompt `Enter text: `, lock mutex, update `marquee_text`. Live update while scrolling.     |
| `set_speed`     | Print current speed, prompt for new ms value, validate positive int, update `speed_ms`.    |
| `clear_screen`  | Pause marquee, clear viewport, redraw banner, resume scrolling. Preserves marquee state.   |
| `exit`          | Set `is_app_alive=false`, join worker thread, print `Goodbye.`, break loop.                |

Per-prompt chrome: greeting `Welcome to CSOPESY!` + `Don't know what to type?...` once, then every iteration `Group Developers:` roster + `Command > `. Unknown input prints `Unknown command. Type 'help'.`.

## Example session

```text
Welcome to CSOPESY!

Don't know what to type? Type help to know the commands!

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > help

help - show commands
start_marquee - begin scrolling
stop_marquee - pause scrolling
set_text - change marquee text
set_speed - change speed (ms)
clear_screen - clear the screen
exit - quit

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > start_marquee

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > set_text

Current text is Hello CSOPESY!

Enter text: CSOPESY Marquee!
New text set to CSOPESY Marquee!

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > set_speed

Current speed is 200ms

Enter new speed (in milliseconds): 100
Speed set to 100ms

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > clear_screen

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > stop_marquee

Group Developers:
Chavez, Max Benedict B.
Leano, Jeremy L.

Command > exit
Goodbye.
```

## Project layout

```text
CMakeLists.txt    # add_executable(marquee_app), run target
src/              # main.cpp, Marquee.cpp, CommandHandler.cpp, AsciiArt.cpp
include/          # Marquee.h, CommandHandler.h, AsciiArt.h
build/            # CMake out-of-source build (gitignored)
specs/            # master_specs.md + per-command specs
LICENSE           # MIT
```

## Notes

- Worker thread uses `std::condition_variable` to sleep between frames and wake early on `stop_marquee` or `exit`.
- `clear_line()` is internal — called only by `stop_marquee` to wipe the banner rows.
- `render_current_frame()` uses raw ANSI escape sequences (Win32 `SetConsoleCursorPosition` fallback).
- `move_cursor()` is Windows-only, guarded by `#ifdef _WIN32`.
- `.gitignore` covers `build/`, object files, and CMake-generated files.

## License

MIT — see `LICENSE`.
