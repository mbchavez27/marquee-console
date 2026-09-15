# Commands: `start_marquee` and `stop_marquee`

**Trigger:** User types "start_marquee".
**Action:**

1. Check if `is_running` is already `true`. If yes, output "Marquee is already running."
2. If no, set `is_running = true` via `exchange`, snapshot `marquee_text` under lock, convert via `ascii_art::convert_to_ascii` outside the lock, print the 5 rows once.
   **Post-conditions:** One-shot ASCII banner visible. No scrolling, no worker.

**Trigger:** User types "stop_marquee".
**Action:**

1. Check if `is_running` is already `false`. If yes, output "Marquee is already stopped."
2. If no, set `is_running = false` via `exchange` and call `clear_line()`.
   **Post-conditions:** Current line cleared. Console awaits next command.
