# Command: `set_speed`

**Trigger:** User types "set_speed".
**Action:**

1. Print a prompt: "Enter new speed (in milliseconds): ".
2. Read the user input.
3. Validate that the input is a positive integer greater than 0. If invalid, print an error and abort the command.
4. Update the atomic variable `speed_ms` with the new integer.
   **Post-conditions:** The worker thread's sleep duration is updated to the new value immediately.
