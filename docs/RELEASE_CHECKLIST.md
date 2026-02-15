# Public Release Checklist

This checklist is for publishing `MafiaSavegame` as a public GitHub project and creating a release with the latest GUI build.

## 1) Local Preflight

1. Ensure build passes:
```powershell
g++ -std=c++17 -O2 -Wall -Wextra -static -static-libgcc -static-libstdc++ mafia_save.cpp mafia_editor_gui.cpp -o "bin/gui/Mafia Savegame Editor.exe" -mwindows -lcomdlg32 -lcomctl32
```
2. Ensure `git status` has only intended changes.
3. Ensure private/local folders are ignored (`Mafia/`, `archive/`, `dist/`).

## 2) Build Release Artifact

Use release script:

```powershell
powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1
```

Result:

- `dist/Mafia Savegame Editor.exe`

## 3) Commit + Push

```powershell
git add .
git commit -m "Prepare public release v0.1.0"
git push origin main
```

## 4) Make GitHub Repository Public

On GitHub:

1. Open repository `Settings`.
2. Go to `General`.
3. Scroll to `Danger Zone`.
4. `Change repository visibility` -> `Make public`.

## 5) Create GitHub Release

On GitHub:

1. Open `Releases` -> `Draft a new release`.
2. Tag: `v0.1.0`.
3. Title: `Mafia Save Editor v0.1.0`.
4. Upload asset: `dist/Mafia Savegame Editor.exe`.
5. Use notes from `docs/releases/v0.1.0.md` (or adapt).
6. Publish release.

## 6) Example Release Notes

```text
Mafia Save Editor (Mafia 1 2002) initial public release.

Highlights:
- GUI editor for mafiaXXX.YYY saves.
- Tabs: Main, Actors, Cars, Garage, Mission/Script.
- Safe parse/edit/rebuild via G_Stream-compatible crypto.
- Garage editor with car table decoding from Mafia/tables/carindex.def.

Build:
- Source included, build with MinGW g++ (see README).
```
