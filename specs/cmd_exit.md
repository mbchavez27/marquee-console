# Command: `exit`

**Trigger:** User types "exit".
**Action:**

1. Set `is_app_alive = false`.
2. Set `is_running = false` (to unblock the worker thread if it is currently sleeping/looping).
3. Call `.join()` on the worker thread to ensure it shuts down cleanly.
4. Print a goodbye message and return `0` from `main()`.
   **Post-conditions:** Application terminates without memory leaks or dangling threads.
