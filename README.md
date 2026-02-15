# MafiaSavegame

Reverse-engineering and save editor project for **Mafia 1 (2002)**.

Main goal: parse/edit `mafiaXXX.YYY` save files safely (with original `G_Stream` encryption flow) and provide a practical GUI editor.

## Current Status

- Core parser/encrypter implemented in `mafia_save.cpp` / `mafia_save.hpp`.
- Windows GUI editor implemented in `mafia_editor_gui.cpp`.
- Garage/Cars/Actors/Mission-script editing is available.
- Research sources and notes are included in `docs/`.

## Repository Structure

- `mafia_editor_gui.cpp` - WinAPI GUI editor source.
- `mafia_save.cpp`, `mafia_save.hpp` - save format, segment parsing, read/write helpers.
- `mafia_stream_tool.cpp` - CLI inspector for save internals.
- `gvas_*`, `payload_study.cpp`, `save_probe.cpp` - research/legacy utilities.
- `docs/GUI_EDITOR.md` - GUI feature documentation.
- `docs/REVERSE_NOTES.md` - reverse-engineering notes and findings.
- `docs/reverse/` - reference reverse files and notes (`IDA_SaveData.txt`, etc.).
- `data/` - sample files and reports.

## Build (Windows, MinGW g++)

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -static -static-libgcc -static-libstdc++ mafia_save.cpp mafia_editor_gui.cpp -o "bin/gui/Mafia Savegame Editor.exe" -mwindows -lcomdlg32 -lcomctl32
```

## Run

```powershell
& "bin\gui\Mafia Savegame Editor.exe"
```

Detailed usage is in `docs/GUI_EDITOR.md`.

## Important Notes

- This repository does **not** include bundled game binaries or large unpacked game folder.
- Local unpacked Mafia folder should be placed as `Mafia/` and is ignored by git.
- Garage car names work in standalone mode via embedded catalog (no external `carindex.def` required).
- Save editing can break scripts/mission state if values are invalid. Keep backups.

## Public Release Workflow

Step-by-step checklist for opening repo + creating GitHub release is in:

- `docs/RELEASE_CHECKLIST.md`

There is also a packaging script:

- `scripts/build_release.ps1`
