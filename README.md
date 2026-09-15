# marquee-console

A single-threaded C++ console app that prints text as a one-shot 5-row ASCII banner. Built for CSOPESY (OS emulator exercise).

## What it does

Prompts for commands in a `Command > ` loop and prints the current text as ASCII art on `start_marquee`. Mutates display state in place via `set_text`, `set_speed`, `start/stop_marquee`, `clear_screen`.

## How it works

- **Main loop only:** blocks on `std::getline(std::cin, ...)`, parses commands.
- No worker thread, no scrolling animation yet. `start_marquee` snapshots text and prints 5 rows once.
- **Shared state:**

| Variable       | Type                | Guard                                          |
| -------------- | ------------------- | ---------------------------------------------- |
| `is_running`   | `std::atomic<bool>` | one-shot flag via `exchange` in `start/stop_marquee` |
| `is_app_alive` | `std::atomic<bool>` | `false` on `exit` to break loop                |
| `speed_ms`     | `std::atomic<int>`  | stored via `set_speed`, not yet used for timing |
| `marquee_text` | `std::string`       | `std::mutex` + `std::lock_guard`, narrow scope |

Screen helpers use ANSI escapes: `clear_line` sends `\033[2K\r`, `clear_screen` sends `\033[2J\033[H`.

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

| Command         | Action                                                                                  |
| --------------- | --------------------------------------------------------------------------------------- |
| `help`          | Print exact 7 supported commands.                                                       |
| `start_marquee` | Print 5-row ASCII banner. Prints `Marquee is already running.` if already on.           |
| `stop_marquee`  | Set `is_running=false` + `clear_line()`. Prints `Marquee is already stopped.` if off.   |
| `set_text`      | Prompt `Enter text: `, lock mutex, update `marquee_text`.                               |
| `set_speed`     | Print `Current speed is Xms`, prompt `Enter new speed (in milliseconds): `, validate positive int, update `speed_ms`. |
| `clear_screen`  | Call `Marquee::clear_screen()` without mutating shared state.                           |
| `exit`          | Set `is_app_alive=false`, `is_running=false`, print `Goodbye.`, break loop, return `0`. |

Per-prompt chrome: greeting `Welcome to CSOPESY!` + `Don't know what to type?...` once, then every iteration `Group Developers:` roster + `Command > `. Unknown input prints `Unknown command. Type 'help'.`.

## Example session

```text
Welcome to CSOPESY!

Don't know what to type? Type help to know the commands!

Group Developers:
Chavez, Max Benedict B.

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

Command > set_text

Enter text: Hi!
Group Developers:
Chavez, Max Benedict B.

Command > set_speed

Current speed is 200ms

Enter new speed (in milliseconds): 100
Speed set to 100ms

Group Developers:
Chavez, Max Benedict B.

Command > start_marquee

#   # #####   #
#   #   #     #
#####   #     #
#   #   #
#   # #####   #

Group Developers:
Chavez, Max Benedict B.

Command > stop_marquee

Group Developers:
Chavez, Max Benedict B.

Command > exit
Goodbye.
```

## Project layout

```text
CMakeLists.txt  # add_executable(marquee_app), run target
src/          # main.cpp, Marquee.cpp, CommandHandler.cpp, AsciiArt.cpp
include/      # Marquee.h, CommandHandler.h, AsciiArt.h
build/        # CMake out-of-source build (gitignored)
specs/        # master_specs.md + per-command specs
LICENSE       # MIT
```

## Notes

- Single-threaded: no worker thread yet, `speed_ms` is stored only.
- `clear_line()` is only reached via `stop_marquee`; no `clear_line` command routed.
- `.gitignore` covers `build/` and CMake-generated files.

## License

MIT — see `LICENSE`.
