# Commands: `start_marquee` and `stop_marquee`

**Trigger:** User types "start_marquee".
**Action:**

1. Check if `is_running` is already `true`. If yes, output "Marquee is already running."
2. If no, set `is_running = true`. Ensure the worker thread wakes up or begins the printing loop.
   **Post-conditions:** Marquee text begins scrolling on screen.

**Trigger:** User types "stop_marquee".
**Action:**

1. Check if `is_running` is already `false`. If yes, output "Marquee is already stopped."
2. If no, set `is_running = false`.
   **Post-conditions:** The worker thread pauses its output loop. Console awaits next command.
