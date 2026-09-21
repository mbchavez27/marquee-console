# Command: `set_speed`

**Trigger:** User types "set_speed <speed_ms>".
**Action:**

1. Parse the inline speed argument.
2. Validate that the input is a positive integer greater than 0. If missing or invalid, print an error and abort the command.
3. Update the atomic variable `speed_ms` with the new integer and print confirmation message.
   **Post-conditions:** `speed_ms` is stored atomically. If marquee is currently running, the live scrolling loop immediately adjusts its frame interval to the new speed without requiring a restart.

