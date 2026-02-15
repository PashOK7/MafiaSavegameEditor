param(
    [string]$Version = "v0.1.0"
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
Set-Location $root

$distDir = Join-Path $root "dist"
if (-not (Test-Path $distDir)) {
    New-Item -ItemType Directory -Path $distDir | Out-Null
}

$exePath = Join-Path $root "bin\gui\mafia_editor_gui.exe"
if (-not (Test-Path $exePath)) {
    throw "Missing build output: $exePath. Build GUI first."
}

$staging = Join-Path $distDir "staging-$Version"
if (Test-Path $staging) {
    Remove-Item -Recurse -Force $staging
}
New-Item -ItemType Directory -Path $staging | Out-Null

Copy-Item $exePath (Join-Path $staging "mafia_editor_gui.exe")
Copy-Item (Join-Path $root "README.md") (Join-Path $staging "README.md")
Copy-Item (Join-Path $root "docs\GUI_EDITOR.md") (Join-Path $staging "GUI_EDITOR.md")
Copy-Item (Join-Path $root "docs\RELEASE_CHECKLIST.md") (Join-Path $staging "RELEASE_CHECKLIST.md")

$zipName = "MafiaSavegame-$Version-win64.zip"
$zipPath = Join-Path $distDir $zipName
if (Test-Path $zipPath) {
    Remove-Item -Force $zipPath
}

Compress-Archive -Path (Join-Path $staging "*") -DestinationPath $zipPath -CompressionLevel Optimal
Remove-Item -Recurse -Force $staging

Write-Output "Release package created: $zipPath"

