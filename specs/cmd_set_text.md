# Command: `set_text`

**Trigger:** User types "set_text".
**Action:**

1. Print a prompt: "Enter text: ".
2. Read the full line with `std::getline` (multi-word supported).
3. Lock `text_mutex` and update `marquee_text`. No success message.
   **Post-conditions:** Next `start_marquee` prints the new text.
