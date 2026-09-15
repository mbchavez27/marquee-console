# System Specification: C++ Marquee Console

## 1. Context & Tech Stack

- **Language:** C++ (Standard C++17 or higher).
- **Environment:** Console/Terminal application.
- **Primary Goal:** Build a CLI tool that prints text as a one-shot 5-row ASCII banner in a single-threaded `Command > ` loop. No worker thread, no scrolling animation yet.

## 2. Architectural Guidelines

- **Concurrency Model:** Single-threaded main loop only. No `<thread>`, no worker.
  - **Main Loop:** Dedicated to `std::cin` blocking input via `std::getline`, command parsing, greeting + roster + `Command > ` prompt, and one-shot ASCII dispatch.
  - **No Worker Thread:** `start_marquee` snapshots text and prints 5 rows once; `speed_ms` is stored only.
- **Thread Safety:** Shared state variables (e.g., `marquee_text`, `speed_ms`, `is_running`, `is_app_alive`) use `std::mutex` and `std::atomic`. Atomics/mutex kept for future worker, narrow lock scope today.
- **Screen Handling:** Helpers only — `clear_line` sends `\033[2K\r`, `clear_screen` sends `\033[2J\033[H` (+ Win32 fallback). No animation loop.

## 3. Global State Variables

- `std::atomic<bool> is_running`: One-shot flag via `exchange` in `start/stop_marquee`.
- `std::atomic<bool> is_app_alive`: Set to false by `exit` to break the main loop.
- `std::atomic<int> speed_ms`: Stored via `set_speed`, not yet used for timing.
- `std::string marquee_text`: The text payload to display (guarded by a mutex).

## 4. Command Router

The main loop must listen for the following precise string commands. Detailed implementation specs for each command are located in the `specs/` directory.
Static ASCII-art rendering (text -> 5-row banner, print loop) is specified in `specs/ascii_art.md`.

1.  `help` - Print exact 7 supported commands.
2.  `start_marquee` - One-shot ASCII print; `Marquee is already running.` if on.
3.  `stop_marquee` - Set `is_running = false` + `clear_line()`; `Marquee is already stopped.` if off.
4.  `set_text` - Prompt `Enter text: `, lock mutex, update `marquee_text`.
5.  `set_speed` - Print current speed, prompt for int, validate `> 0`, update `speed_ms`.
6.  `clear_screen` - Wipe viewport, no state change.
7.  `exit` - Set `is_app_alive = false`, `is_running = false`, print goodbye, break loop, return `0`.

Per-prompt chrome: `Welcome to CSOPESY!` greeting once, then `Group Developers:` roster + `Command > ` every iteration. Unknown input prints `Unknown command. Type 'help'.`.

## 5. Coding Standards & Best Practices

To maintain a clean, conflict-free codebase across the team and ensure the AI generates consistent code, adhere to the following standards:

- **Header Protection:** Use `#pragma once` at the top of all `.h` or `.hpp` files instead of traditional include guards.
- **Naming Conventions:**
  - Classes and Structs: `PascalCase` (e.g., `CommandHandler`, `Marquee`).
  - Variables and Functions: `snake_case` (e.g., `start_marquee()`, `speed_ms`).
  - Constants and Macros: `UPPER_SNAKE_CASE` (e.g., `MAX_BUFFER_SIZE`).
- **Concurrency Rules:**
  - Never read or write to `marquee_text` without first acquiring a `std::lock_guard<std::mutex>`.
  - Avoid blocking the main thread during synchronization; keep locked scopes as narrow as possible.
- **Memory & Safety:** Use standard C++17 features. Prefer RAII and stack allocation. If dynamic memory is required, strictly use smart pointers (`std::unique_ptr`, `std::shared_ptr`).
- **Input Handling:** Always use `std::getline(std::cin, var)` instead of `cin >> var` to gracefully handle multi-word string inputs and avoid leaving trailing newline characters in the buffer.
