# Command: `exit`

**Trigger:** User types "exit".
**Action:**

1. Set `is_app_alive = false`.
2. Set `is_running = false`.
3. Print `Goodbye.`, break the main loop, return `0` from `main()`.
   **Post-conditions:** Application terminates. No worker thread to join.
