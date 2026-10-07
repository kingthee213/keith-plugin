# Fetches iPlug2 at a pinned revision into third_party/iPlug2/ (gitignored).
$ErrorActionPreference = "Stop"
$Root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Dest = Join-Path $Root "third_party\iPlug2"
$Iplug2Rev = "d54f69050f517e43b941d88c2a170f0a840b9ee4"
$Iplug2Repo = "https://github.com/iPlug2/iPlug2.git"

if (Test-Path (Join-Path $Dest ".git")) {
    Write-Host "Updating existing iPlug2 checkout at $Dest"
    git -C $Dest fetch --depth 1 origin $Iplug2Rev
    git -C $Dest checkout --detach $Iplug2Rev
    git -C $Dest submodule update --init --recursive
} else {
    Write-Host "Cloning iPlug2 $Iplug2Rev into $Dest"
    New-Item -ItemType Directory -Force -Path (Split-Path $Dest) | Out-Null
    git clone --recursive $Iplug2Repo $Dest
    git -C $Dest checkout --detach $Iplug2Rev
    git -C $Dest submodule update --init --recursive
}

Write-Host "Fetching plugin SDKs (VST3, etc.)..."
Push-Location (Join-Path $Dest "Dependencies\IPlug")
bash ./download-iplug-sdks.sh
Pop-Location

Write-Host "iPlug2 ready at $Iplug2Rev"
