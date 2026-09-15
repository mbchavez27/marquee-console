**Pre-conditions:** None.
**Trigger:** User types "clear_line".
**Action:**

1. Erase current console line via `Marquee::clear_line()` without mutating `is_running`, `is_app_alive`, `speed_ms`, `marquee_text`.
2. Linux/macOS: `"\033[2K\r"` + `flush`. Windows: enable VT then same; fallback `"\r" + string(80,' ') + "\r"`.
   **Post-conditions:** Line blank, cursor col 0. Next worker frame redraws normally if running.
