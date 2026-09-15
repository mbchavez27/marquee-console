# Command: `clear_screen`

**Pre-conditions:** None.
**Trigger:** User types "clear_screen".
**Action:**

1. Wipe viewport and home cursor via `Marquee::clear_screen()` without mutating shared state.
2. Linux/macOS: `"\033[2J\033[H"` + `flush`. Windows: enable VT then same; fallback `FillConsoleOutputCharacterA` + `SetConsoleCursorPosition(0,0)`.
3. Usable while running or stopped; does not pause marquee.
   **Post-conditions:** Screen blank, cursor home. Next prompt/frame draws on clean screen.
