# marquee-console

A multithreaded C++ console app that scrolls a text marquee while concurrently accepting user commands. Built for CSOPESY (OS emulator exercise).

## What it does

Continuously animates a scrolling string on the terminal, while the main thread stays responsive to input commands that mutate the animation state (text, speed, start/stop).

## How it works

- **Thread A (Main):** blocks on `std::getline(std::cin, ...)`, parses commands.
- **Thread B (Worker):** render loop — prints marquee frame, sleeps for `speed_ms`.
- **Shared state:**

| Variable       | Type                | Guard                                          |
| -------------- | ------------------- | ---------------------------------------------- |
| `is_running`   | `std::atomic<bool>` | lock-free toggle via `start/stop_marquee`      |
| `is_app_alive` | `std::atomic<bool>` | `false` on `exit` to join threads              |
| `speed_ms`     | `std::atomic<int>`  | set via `set_speed`                            |
| `marquee_text` | `std::string`       | `std::mutex` + `std::lock_guard`, narrow scope |

Screen refresh uses carriage return (`\r`) / clear to animate without flooding history.

## Requirements

- Linux + `g++` with C++17 support
- `make`
- `pthread` (linked via `-pthread`)

## Build & Run

```bash
make          # builds ./marquee_app from src/*.cpp
./marquee_app # run
make run      # build + run in one step
make clean    # removes obj/ and binary
```

## Commands

| Command         | Action                                                                                  |
| --------------- | --------------------------------------------------------------------------------------- |
| `help`          | List all commands with descriptions.                                                    |
| `start_marquee` | Set `is_running=true`. Prints `Marquee is already running.` if already on.              |
| `stop_marquee`  | Set `is_running=false`. Prints `Marquee is already stopped.` if already off.            |
| `set_text`      | Prompt for string, lock mutex, update `marquee_text`.                                   |
| `set_speed`     | Prompt `Enter new speed (in milliseconds): `, validate positive int, update `speed_ms`. |
| `exit`          | Set `is_app_alive=false`, `is_running=false`, join worker, print goodbye, return `0`.   |

## Example session

```text
> help
help - show commands
start_marquee - begin scrolling
...
> set_text
Enter text: Hello CSOPESY!
> set_speed
Enter new speed (in milliseconds): 100
> start_marquee
Hello CSOPESY! scrolling...
> stop_marquee
> exit
Goodbye.
```

## Project layout

```text
Makefile      # all / clean / run, BIN=marquee_app
src/          # *.cpp sources (currently empty — implementation pending)
specs/        # master_specs.md + per-command specs
LICENSE       # MIT
```

## Notes

- `specs/cmd_set_text.md` is currently empty; behavior above follows `master_specs.md §4`.
- `Makefile` passes `-Iinclude` but no `include/` dir exists yet.
- `.gitignore` does not yet cover `obj/` or `marquee_app`.

## License

MIT — see `LICENSE`.
