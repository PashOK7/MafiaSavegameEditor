# Mafia Save GUI Editor

Minimal Windows GUI editor for Mafia 1 (2002) saves.

## Executable

- `bin/gui/mafia_editor_gui.exe`

## Build

```powershell
g++ -std=c++17 -O2 -Wall -Wextra mafia_save.cpp mafia_editor_gui.cpp -o bin/gui/mafia_editor_gui.exe -mwindows -lcomdlg32 -lcomctl32
```

## Usage

1. Run `bin/gui/mafia_editor_gui.exe`.
2. Click `Open...` and choose a save file.
3. Use tab `Main` to edit:
- `HP %`
- `Date (DD.MM.YYYY)`
- `Time (HH:MM:SS)`
- `Slot`
- `Mission code`
- `Mission name`
4. Use tab `Actors` to edit selected actor:
- `Actor name`
- `Actor model`
- `Actor type`
- `Actor idx`
- `Payload state`
- `Payload id`
- `Is active`
- `Do remove`
- `Frame on`
- `Pos X`, `Pos Y`, `Pos Z` (only when payload format is mapped)
- `Dir X`, `Dir Y`, `Dir Z` and `Anim ID` (for human/player payload mapping)
- `Seat ID (46)`, `Crouching (50)`, `Aiming (51)`, `Shoot X/Y/Z (54/58/62)` (for human/player payload mapping)
- `Inventory`: mode/flag, selected weapon, coat weapon, slots 1-5 (`ID`, `Ammo Loaded`, `Ammo Hidden`) for human/player payload mapping
- `Inventory item Unk`: additional `Unk` DWORD for selected/coat/slots 1-5 (advanced)
- `Rot W`, `Rot X`, `Rot Y`, `Rot Z` (for car payload mapping)
- `Car Fuel* (304)`, `Fuel Flow* (211)`, `EngNorm* (137)`, `EngCalc* (141)` (experimental offsets)
- `SpeedLimit* (215)`, `LastGear* (245)`, `Gear* (249)`, `GearboxFlg* (273)`, `DisableEng* (277)`, `EngineON* (298)`, `IsEngineOn* (303)`, `Odometer* (345)` (experimental car offsets)
5. Optional on tab `Actors`:
- filter actor list by name and/or type
- clone selected actor header+payload (`Clone Actor`)
6. Use tab `Cars` to edit mission car actors (`type=4`) in scene:
- position/quaternion
- fuel/speed limit/odometer/engine flag
7. Use tab `Garage` to edit persistent Salieri garage slots from `info264`:
- `25` slots (`slot_0 .. slot_24`)
- `Primary A (u32)` from offsets `40..139`
- `Secondary B (u32)` from offsets `140..239`
- automatic decode to car name (`idx -> name`) using `Mafia/tables/carindex.def` (fallback `carcyclopedia.def`)
- car pickers for `Primary` / `Secondary` values
- editable per-slot high-byte flags (`Primary flag`, `Secondary flag`)
- helper fields (`hex`, `low16`, `hi8`, decoded text) for both values
- `B = A`, `Apply Slot`, `Clear Slot`
8. Use tab `Mission/Script` to edit mission runtime header and script snapshot:
- `Payload marker` (read-only)
- `Game field A`, `Game field B` (u32 fields from game payload header, advanced)
- `Mission ID` (payload-level mission id)
- `Timer enabled`, `Timer interval`, `Timer value A/B/C`
- `Score enabled`, `Score value`
- `Script entries`, `Script chunks` (read-only counters from payload header)
- Script program detection:
- `Program offset`, `Script vars`, `Script actors`, `Script frames` (read-only)
- `Pause script` (program command-block flag)
- `Script var #`, `Script value` + `Read Script Var` button for float variable preview/edit
- `Script vars table (index | value)` with click-to-select (fills `Script var #` and `Script value`)
9. Click `Apply Actor` / `Apply Car` / `Apply Slot` if you changed fields, then `Save As...`.

## Notes

- Editor uses full `G_Stream` decrypt/edit/re-encrypt flow.
- Invalid actor fields can break scripts in game.
- Coordinates are currently mapped for human/player payload format (`marker=6`), including `Tommy` and most NPC actors.
- Extended human state fields are mapped at payload offsets `46/50/51/54/58/62` when payload size allows it.
- Human inventory block (`196` bytes) is detected dynamically after human block + 2 actor refs.
- Coordinates and quaternion are mapped for car payload format (`marker=9`) used by `type=4` actors in tested saves.
- Fields marked with `*` are experimental reverse-engineered offsets and may differ between missions/builds.
- Main window is resizable.
- `Actors` tab has a right-side vertical scroll for actor detail fields when height is limited.
- Unsupported parameter groups are hidden automatically for the selected actor payload type.
- `Garage` tab edits the same `info264` area that game save/load uses for persistent garage arrays (`G_LoadSaveClass` internals).
- `Mission/Script` tab parses and edits the first `67` bytes of `game_payload` (`C_game::SaveGameSave` header) and detects `C_program` block heuristically in `game_payload` and `actor_payload_*` segments.
- Script variable is an internal float from mission script (`C_program::m_fVariables`); index meaning depends on mission scripts.
- Script variable editing uses `Script var #` and `Script value`; value is written on `Save As...`.
- `Reset Form` restores fields from currently loaded save.
- Warning line turns red when `Tommy` actor is missing or `Tommy` type is not `2`.
