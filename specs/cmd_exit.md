# Command: `exit`

**Trigger:** User types "exit".
**Action:**

1. Set `is_app_alive = false`.
2. Set `is_running = false`.
3. Signal background worker thread (if active or paused) to wake up and exit.
4. Join the worker thread (`worker.join()`) to ensure no hanging or orphaned threads.
5. Print `Goodbye.`, break the main loop, return `0` from `main()`.

**Post-conditions:**
- Background worker thread cleanly joined.
- Application terminates cleanly with exit code 0.

