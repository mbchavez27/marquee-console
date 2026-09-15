# System Specification: C++ Marquee Console

## 1. Context & Tech Stack

- **Language:** C++ (Standard C++17 or higher).
- **Environment:** Console/Terminal application.
- **Primary Goal:** Build an interactive CLI tool that displays scrolling text as a 5-row ASCII banner in a multi-threaded console application while concurrently processing user commands via a `Command > ` loop.

## 2. Architectural Guidelines

- **Concurrency Model:** Multi-threaded architecture.
  - **Main Thread:** Dedicated to `std::cin` blocking input via `std::getline`, interactive command parsing, greeting + roster + `Command > ` prompt chrome.
  - **Worker Thread:** Dedicated background loop responsible for scrolling, frame delay (`speed_ms`), and rendering the 5-row ASCII banner to the terminal.
- **Thread Safety:** Shared state variables (`marquee_text`, `speed_ms`, `is_running`, `is_app_alive`) use `std::mutex` and `std::atomic` primitives:
  - `marquee_text` reads/writes are protected by a `std::mutex` (narrow lock scope).
  - Flags and speeds (`is_running`, `is_app_alive`, `speed_ms`) are atomic.
- **Screen Handling:** VT escape sequences (`\033[2J\033[H` for screen clear, `\033[2K\r` for line clear) with Win32 API fallback where needed. Clearing the screen preserves the running marquee if `is_running == true`.

## 3. Global State Variables

- `std::atomic<bool> is_running`: Indicates whether the marquee animation is actively scrolling. Controlled via `start_marquee` and `stop_marquee`.
- `std::atomic<bool> is_app_alive`: Indicates if the console application is running. Set to `false` by `exit` to break the main loop and signal the worker thread to exit.
- `std::atomic<int> speed_ms`: Animation refresh/scroll interval in milliseconds.
- `std::string marquee_text`: The text payload to display (guarded by `text_mutex`).

## 4. Command Router

The main loop listens for the following precise string commands:

1.  `help` - Print the 7 supported commands.
2.  `start_marquee` - Start/resume continuous scrolling worker; prints `Marquee is already running.` if already active.
3.  `stop_marquee` - Pause scrolling worker and clear marquee line/banner; prints `Marquee is already stopped.` if already stopped.
4.  `set_text` - Prompt `Enter text: `, lock mutex, update `marquee_text`; updates live display dynamically without restart.
5.  `set_speed` - Prompt for int `> 0`, update `speed_ms`; updates live scroll rate dynamically without restart.
6.  `clear_screen` - Wipe viewport; if `is_running == true`, re-prints banner so it remains visible; if stopped, stays blank.
7.  `exit` - Set `is_app_alive = false`, `is_running = false`, join worker thread cleanly, print goodbye, break loop, return `0`.

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
