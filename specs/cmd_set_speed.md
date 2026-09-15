# Command: `set_speed`

**Trigger:** User types "set_speed".
**Action:**

1. Print `Current speed is <speed_ms>ms`.
2. Print a prompt: "Enter new speed (in milliseconds): ".
3. Read the user input.
4. Validate that the input is a positive integer greater than 0. If invalid, print an error and abort the command.
5. Update the atomic variable `speed_ms` with the new integer.
   **Post-conditions:** `speed_ms` is stored. No worker timing exists yet.
