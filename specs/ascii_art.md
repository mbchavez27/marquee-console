# Module: ASCII Art Renderer

Pure text-to-banner renderer. No threading, no animation, no screen control.
No scrolling exists yet; `Marquee::start_marquee()` does a one-shot print.

## 1. API

- Function: `std::vector<std::string> ascii_art::convert_to_ascii(const std::string &text)`
- Declared in `include/AsciiArt.h`, implemented in `src/AsciiArt.cpp`.
- Input: arbitrary `std::string` (may be empty, multi-word, mixed case).
- Output: exactly 5 rows (`vector<string>` of size 5), one string per glyph row.
- Purity: no shared state, no I/O, no locks. Thread-safe by construction.

## 2. Font

- Type: `Glyph = std::array<std::string, 5>`, stored in static `unordered_map<char, Glyph>`.
- Each glyph cell is 5 cols wide, 5 rows tall.
- Supported charset (all keys uppercase):
  - `A-Z`, `0-9`, `space`, `!`, `?`, `.`
- Joined with 1-column spacer: `rows[r] += glyph[r] + " "` per character.
- Result width for `n` chars: `n * 6` columns (including trailing space).

## 3. Normalization

1. Per character, apply `toupper(static_cast<unsigned char>(c))`.
2. Look up uppercased key in font map.
3. Input is therefore case-insensitive: `a` and `A` render identically.

## 4. Fallback

- Unknown character (e.g. `@`, `#`, lowercase-mapped miss): use fallback glyph:
  ```text
   ###
  #   #
    ##
  
    #
  ```
- Rationale: visible placeholder instead of skipping or crashing.

## 5. Edge Cases

- Empty `text`: return 5 empty strings, print nothing (caller still flushes).
- Spaces: render as 5 blank cells (`"     " + " "`), preserving word gaps.
- Multi-word input: no wrapping; caller decides viewport clipping.

## 6. Display Contract (how `Marquee` prints it)

`Marquee::start_marquee()` must:

1. Snapshot `marquee_text` under `std::lock_guard<std::mutex>` with narrow scope.
2. Call `convert_to_ascii(snapshot)` outside the lock.
3. Print row-by-row:
   ```cpp
   for (const auto &row : art) {
       std::cout << row << "\n";
   }
   std::cout << std::flush;
   ```
4. Never `std::cout << vector` directly (no `operator<<` for `vector<string>`).

## 7. Example

`convert_to_ascii("Hi!")` produces 5 rows (conceptual):

```text
#   # #####   #
#   #   #     #
#####   #     #
#   #   #
#   # #####   #
```

Printed via the loop in §6, each row terminated with `\n`.

## 8. Non-Goals

- No frame slicing (`substr(offset)`), no `speed_ms` sleep, no `clear_line` redraw.
- No cursor movement or ANSI codes in this module.
- No animation yet; scrolling worker is future work.
