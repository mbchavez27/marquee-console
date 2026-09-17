# Commands: `start_marquee` and `stop_marquee`

## `start_marquee`

**Trigger:** User types "start_marquee".
**Action:**

1. Check if `is_running` is already `true`. If yes, output "Marquee is already running." and return.
2. If no, set `is_running = true` via `exchange`.
3. Start (or resume) the background worker thread responsible for continuously scrolling and rendering the marquee display.
4. The worker thread continuously:
   - Reads the latest text snapshot under `text_mutex` protection and the current `speed_ms`.
   - Converts the text to 5-row ASCII art and computes/updates the scroll frame offset.
   - Redraws the marquee frame on the screen.
   - Sleeps for the duration of `speed_ms` between frame redraws (or waits on a condition variable/interrupt flag).
   - Loops while `is_running` is `true` and `is_app_alive` is `true`.
5. The main thread immediately returns to the command prompt loop so the user can continue entering commands without blocking.

**Post-conditions:**
- `is_running == true`.
- Background worker thread is active and scrolling continuously until `stop_marquee` or `exit`.
- Main thread remains responsive to user commands.

---

## `stop_marquee`

**Trigger:** User types "stop_marquee".
**Action:**

1. Check if `is_running` is already `false`. If yes, output "Marquee is already stopped." and return.
2. If no, set `is_running = false` via `exchange`.
3. Signal the background worker thread to stop/pause scrolling, and clean up or clear the marquee rendering area.
4. Worker thread pauses execution without terminating the main application.

**Post-conditions:**
- `is_running == false`.
- Marquee scrolling is stopped/paused.
- Marquee area is cleared.
- Console awaits the next command.

