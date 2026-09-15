# Command: `clear_screen`
 
**Pre-conditions:** None.
**Trigger:** User types "clear_screen".
**Action:**

1. Wipe viewport and home cursor via `Marquee::clear_screen()`.
2. Linux/macOS: `"\033[2J\033[H"` + `flush`. Windows: enable VT then same; fallback `FillConsoleOutputCharacterA` + `SetConsoleCursorPosition(0,0)`.
3. Check the state of `is_running`:
   - If `is_running == true`: The marquee is active, so re-print / redraw the current marquee display right after clearing, ensuring the banner remains visible without the user having to retype `start_marquee`.
   - If `is_running == false`: Marquee is stopped; the screen remains blank.
4. Does NOT mutate `is_running`, `speed_ms`, or `marquee_text`.

**Post-conditions:**
- Viewport is cleared.
- If `is_running == true`: Banner is visible and continues scrolling.
- If `is_running == false`: Screen remains blank until a new command is run.
- Console prints greeting / roster / `Command > ` prompt on the refreshed screen.

