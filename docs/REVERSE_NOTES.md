# Mafia 1 Save Reverse Notes

## 1) `GvaS` header mission fields

Header size used by tools: `56` bytes (`0x38`).

Exact mission formulas:

- `d0 = 0xD20B0000 | ((((mission >> 8) ^ 0x1C) & 0xFF) << 8) | ((mission ^ 0x30) & 0xFF)`
- `d1 = 0x29889D95 + mission`
- `d2 = 0x81061EFA + mission * 2`
- `d5 = 0x877ECA39 + mission * 8`

`d3`/`d4` are mission-dependent too, but not yet reduced to a clean formula.  
For safe editing, tools load `d3`/`d4` from existing save files (lookup table by mission id).

## 2) Tools

- `gvas_tool.cpp` + `gvas.cpp`/`gvas.hpp`
- `payload_study.cpp` + `gvas.cpp`/`gvas.hpp`

Build examples:

```powershell
# clang
& 'C:\Program Files\LLVM\bin\clang++.exe' -std=c++17 -O2 -Wall -Wextra gvas.cpp gvas_tool.cpp -o gvas_tool_clang.exe
& 'C:\Program Files\LLVM\bin\clang++.exe' -std=c++17 -O2 -Wall -Wextra gvas.cpp payload_study.cpp -o payload_study_clang.exe
```

`gvas_tool` usage:

```powershell
.\gvas_tool_clang.exe inspect savegame/mafia004.230
.\gvas_tool_clang.exe set-mission savegame/mafia004.230 out.230 240 --table-dir savegame
.\gvas_tool_clang.exe set-mission savegame/mafia004.230 out.230 555 --table-dir savegame --preserve-word34
```

`payload_study` usage:

```powershell
.\payload_study_clang.exe cross-profile savegame 000 004 160 > payload_cross_000_004.csv
.\payload_study_clang.exe same-profile savegame 004 30 > payload_same_004_top30.csv
```

## 3) Payload findings (current)

- For matched missions between profiles `000` and `004`, common prefix is always `56` bytes.
- After offset `0x38`, payload similarity is low:
  - average equal-byte ratio: about `0.0144` (1.44%)
  - for large saves (`>=10k` payload): about `0.0040`, close to random `1/256`
- Strong invariant bytes at absolute offsets:
  - `0x3B = 0xDE`
  - `0x3F = 0x31`
  - `0x47 = 0xF5`
- Near these bytes there are 2-value fields (`0x43`, `0x4B`, `0x4F`), suggesting structured words in early payload.

## 4) New findings (profiles / pre-payload block)

- For the same mission (`275`) across profiles `000`, `004`, `0041`:
  - bytes `0x00..0x37` are identical;
  - bytes starting at `0x38` differ strongly by profile.
- `d0..d5` (`0x20..0x37`) are mission-only in observed data (identical across those profiles for mission `275`).
- Across profile pairs (`000` vs `004`) only bytes `0x3B`, `0x3F`, `0x47` are always equal in this early profile-dependent block.
- Simple profile-mask model is not confirmed:
  - no constant XOR/add mask across missions for offsets `0x38+` (except the invariant bytes above).
- Similarity after `0x98` drops near random level (~`1/256`) for large saves, so useful structured region is likely near the start after header.

## 5) IDA notes (`IDA_SaveData.txt`)

From `G_LoadSaveClass::SaveGameSave` and `G_LoadSaveClass::SaveGameLoad`:

- Save file format write order is:
1. `24` bytes (`WriteAdr(..., 24)`)  
2. `32` bytes (`WriteAdr(v26, 0x20)`)  
3. `264` bytes (`WriteAdr(v29, 0x108)`)  
4. main payload (`WantWrite(Size)`, then `C_game::SaveGameSave`)
- In load path, game reads in same order (`ReadAdr 24`, `Read 32`, `ReadAdr 264`, then payload).
- Key code locations:
  - write path: around line `5314+` (`SaveGameSave`)
  - load path: around line `4718+` (`SaveGameLoad`)

Practical implication:
- bytes `0x00..0x37` are part of the first two metadata blocks.
- changing these bytes can alter menu metadata and can desync later load behavior.

## 6) `G_Stream`-accurate parser/encrypter (new)

Added new files:

- `mafia_save.hpp` / `mafia_save.cpp`
- `mafia_stream_tool.cpp`

They implement exact `G_Stream` key schedule and per-block behavior from reverse notes / `g_stream.txt`:

- init keys: `key1=0x23101976`, `key2=0x10072002`
- decrypt per dword: `plain = key1 ^ cipher`, then `key2 += plain`, `key1 += key2`
- encrypt per dword: `key2 += plain`, `cipher = plain ^ key1`, `key1 += key2`
- bytes not covered by full dwords in a block are left unchanged (same as game `size >> 2`)

`mafia_stream_tool inspect` now decodes layout and shows segment map with absolute offsets.

For sample `experiments/ready_drop_in/mafia004.230_13`:

- `0x018..0x02F`: head24
- `0x030..0x04F`: meta32
- `0x050..0x157`: info264
- `0x158..0x1A2`: game payload
- `0x1A3..0x25A`: ai_groups
- `0x25B..0x262`: ai_follow
- `0x263..0x2EE`: actor_header_0
- `0x2EF..0x797`: actor_payload_0
- `0x798..0x823`: actor_header_1
- `0x824..0xAA4`: actor_payload_1

Important observation:

- roundtrip (decrypt->encrypt without edits) gives byte-identical output;
- one-byte plaintext edit changes many following ciphertext bytes (key stream is stateful), but keeps decryption synchronized and avoids random corruption of all subsequent plaintext.

## 7) In-game validation from actor-header edits (series 006)

User test results:

- `_25` (control from `_13`): normal behavior.
- `_26` (actor `g_car_0` name cleared): mission loads, but the parked arrival car disappears.
- `_27` (actor `Tommy` name cleared): load succeeds but mission logic degrades
  (repeated fail states, restart softlock, broken cutscene transitions).

Interpretation:

- actor name in 140-byte actor header is directly used by `SaveGameLoad` matching;
- breaking non-player actor name affects that actor persistence only;
- breaking player actor name keeps load alive but destabilizes post-load script state.

## 8) In-game validation from Tommy field isolation (series 007)

User test results:

- `_28` (Tommy model empty): mission starts, but Tommy is invisible/intangible; shooting and some exits cause crash/softlock.
- `_29` (Tommy type `2 -> 4`): opening cutscene does not complete.
- `_30` (Tommy idx `0xFFFFFFFF -> 0`): no visible gameplay change.

Interpretation:

- model string is used for player representation/animation resources and is critical for controllable state;
- actor `type` value strongly affects script/actor handling path;
- `idx` for Tommy is effectively non-critical in this scenario.
