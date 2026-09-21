# Command: `set_text`

**Trigger:** User types "set_text <text>".
**Action:**

1. Parse the inline text argument. If missing, display usage error.
2. If text is wrapped in matching quotes, strip them.
3. Lock `text_mutex` and update `marquee_text`. Print confirmation message.
   **Post-conditions:** `marquee_text` updated under lock. If marquee is currently running, the live scrolling display reflects the new text immediately on the next frame cycle without requiring a restart. If marquee is stopped, new text will be used on next `start_marquee`.

