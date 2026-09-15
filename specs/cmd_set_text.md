# Command: `set_text`

**Trigger:** User types "set_text".
**Action:**

1. Print a prompt: "Enter text: ".
2. Read the full line with `std::getline` (multi-word supported).
3. Lock `text_mutex` and update `marquee_text`. No success message (or optional confirmation as in router).
   **Post-conditions:** `marquee_text` updated under lock. If marquee is currently running, the live scrolling display reflects the new text immediately on the next frame cycle without requiring a restart. If marquee is stopped, new text will be used on next `start_marquee`.

