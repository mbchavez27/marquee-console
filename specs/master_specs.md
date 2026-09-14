# System Specification: C++ Marquee Console

## 1. Context & Tech Stack

- **Language:** C++ (Standard C++17 or higher).
- **Environment:** Console/Terminal application.
- **Primary Goal:** Build a CLI tool that continuously animates a scrolling text string on screen while concurrently accepting standard input commands from the user to manipulate the animation state.

## 2. Architectural Guidelines

- **Concurrency Model:** The application requires multithreading using `<thread>`.
  - **Thread A (Main Thread):** Dedicated to `std::cin` blocking input, command parsing, and menu display.
  - **Thread B (Worker Thread):** Dedicated to managing the console output and sleeping/refreshing the screen at the defined speed interval.
- **Thread Safety:** Shared state variables (e.g., `current_text`, `refresh_speed_ms`, `is_running`, `should_exit`) must be protected using `std::mutex` and `std::atomic` to prevent race conditions when the user issues commands while the marquee is running.
- **Screen Handling:** The worker thread will likely need to clear the console or utilize carriage returns (`\r`) to create the illusion of animation without flooding the terminal history.

## 3. Global State Variables

- `std::atomic<bool> is_running`: Toggled by `start_marquee` and `stop_marquee`.
- `std::atomic<bool> is_app_alive`: Set to false by `exit` to cleanly join threads.
- `std::atomic<int> speed_ms`: Dictates the sleep duration of the worker thread.
- `std::string marquee_text`: The text payload to display (guarded by a mutex).

## 4. Command Router

The main loop must listen for the following precise string commands. Detailed implementation specs for each command are located in the `specs/` directory:

1.  `help` - Print command list.
2.  `start_marquee` - Set `is_running = true`.
3.  `stop_marquee` - Set `is_running = false`.
4.  `set_text` - Prompt for string, lock mutex, update `marquee_text`.
5.  `set_speed` - Prompt for int, update `speed_ms`.
6.  `exit` - Set `is_app_alive = false`, join threads, terminate.

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
