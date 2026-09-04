[CmdletBinding()]
param(
    [ValidateSet('eur')]
    [string]$Version = 'eur',

    [string]$EmulatorPath = '',

    [string]$DataProject = '',

    [switch]$DisableDataMods
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Restore-FixedLayoutHeaderChecksums {
    param(
        [Parameter(Mandatory = $true)]
        [string]$BaseRom,
        [Parameter(Mandatory = $true)]
        [string]$BuiltRom
    )

    $headerSize = 0x160
    $checksumOffsets = @(0x6C, 0x6D, 0x15E, 0x15F)
    $baseHeader = New-Object byte[] $headerSize
    $builtHeader = New-Object byte[] $headerSize
    $baseStream = [System.IO.File]::OpenRead($BaseRom)
    $builtStream = [System.IO.File]::OpenRead($BuiltRom)
    try {
        if ($baseStream.Read($baseHeader, 0, $headerSize) -ne $headerSize -or
            $builtStream.Read($builtHeader, 0, $headerSize) -ne $headerSize) {
            throw 'Could not read a complete Nintendo DS header.'
        }
    } finally {
        $baseStream.Dispose()
        $builtStream.Dispose()
    }

    for ($offset = 0; $offset -lt $headerSize; ++$offset) {
        if ($checksumOffsets -notcontains $offset -and
            $baseHeader[$offset] -ne $builtHeader[$offset]) {
            throw ('Unexpected ROM header difference at 0x{0:X}; refusing ' +
                   'to apply the fixed-layout checksum repair.' -f $offset)
        }
    }

    $outputStream = [System.IO.File]::Open(
        $BuiltRom,
        [System.IO.FileMode]::Open,
        [System.IO.FileAccess]::Write,
        [System.IO.FileShare]::None)
    try {
        foreach ($offset in $checksumOffsets) {
            $outputStream.Position = $offset
            $outputStream.WriteByte($baseHeader[$offset])
        }
    } finally {
        $outputStream.Dispose()
    }
}

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$baseRom = Join-Path $repoRoot "extract\baserom_PiT_$Version.nds"
$outputRom = Join-Path $repoRoot "PiT_$Version.nds"
$linkedSources = Join-Path $repoRoot "config\$Version\arm9\linked_sources.txt"
$filesRoot = Join-Path $repoRoot "extract\$Version\files"
$resolvedDataProject = $null

if (-not $DisableDataMods) {
    if ([string]::IsNullOrWhiteSpace($DataProject)) {
        $candidate = Join-Path $repoRoot "data\$Version"
        if (Test-Path -LiteralPath (Join-Path $candidate 'project.json') -PathType Leaf) {
            $resolvedDataProject = $candidate
        }
    } else {
        if ([System.IO.Path]::IsPathRooted($DataProject)) {
            $candidate = $DataProject
        } else {
            $candidate = Join-Path $repoRoot $DataProject
        }
        $resolvedDataProject = (Resolve-Path -LiteralPath $candidate).Path
        if (-not (Test-Path -LiteralPath (Join-Path $resolvedDataProject 'project.json') -PathType Leaf)) {
            throw "Data project has no project.json: $resolvedDataProject"
        }
    }
}

if (-not (Test-Path -LiteralPath $baseRom -PathType Leaf)) {
    throw "Missing private base ROM: $baseRom`nCopy your matching ROM there before building."
}

$python = Get-Command python.exe -ErrorAction SilentlyContinue
if ($null -eq $python) {
    $python = Get-Command python -ErrorAction SilentlyContinue
}
if ($null -eq $python) {
    throw 'Python was not found on PATH.'
}

$ninjaCandidates = [System.Collections.Generic.List[string]]::new()
$pathNinja = Get-Command ninja.exe -ErrorAction SilentlyContinue
if ($null -ne $pathNinja) {
    $ninjaCandidates.Add($pathNinja.Source)
}

$programFiles = [Environment]::GetFolderPath('ProgramFiles')
$jetBrainsRoot = Join-Path $programFiles 'JetBrains'
if (Test-Path -LiteralPath $jetBrainsRoot -PathType Container) {
    Get-ChildItem -LiteralPath $jetBrainsRoot -Directory -Filter 'CLion*' |
        Sort-Object Name -Descending |
        ForEach-Object {
            $ninjaCandidates.Add(
                (Join-Path $_.FullName 'bin\ninja\win\x64\ninja.exe'))
        }
}

$ninjaCandidates.Add((Join-Path -Path $programFiles -ChildPath 'Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe'))
$ninjaCandidates.Add((Join-Path -Path $programFiles -ChildPath 'Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe'))

$ninja = $ninjaCandidates |
    Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } |
    Select-Object -First 1
if ($null -eq $ninja) {
    throw 'Ninja was not found on PATH or in the known CLion/Visual Studio locations.'
}

Push-Location $repoRoot
try {
    & $python.Source tools\configure.py $Version
    if ($LASTEXITCODE -ne 0) {
        throw "Project configuration failed with exit code $LASTEXITCODE."
    }

    & $ninja rom
    if ($LASTEXITCODE -ne 0) {
        throw "ROM build failed with exit code $LASTEXITCODE."
    }

    if ($null -ne $resolvedDataProject) {
        $baseRomConfig = Join-Path $repoRoot "build\$Version\build\rom_config.yaml"
        $dataRomConfig = Join-Path $repoRoot "build\$Version\build\rom_config_data_mod.yaml"
        $stagedFiles = Join-Path $repoRoot "build\$Version\data_mod_files"
        $stagedCode = Join-Path $repoRoot "build\$Version\data_mod_code"
        $overlay9 = Join-Path $repoRoot "build\$Version\build\arm9_ov009.bin"
        $dataReport = Join-Path $repoRoot "build\$Version\data_mod_report.json"
        & $python.Source tools\data_mod.py build `
            --files-root $filesRoot `
            --project-root $resolvedDataProject `
            --output-files $stagedFiles `
            --overlay-9-bin $overlay9 `
            --output-code $stagedCode `
            --rom-config-input $baseRomConfig `
            --rom-config-output $dataRomConfig `
            --report $dataReport
        if ($LASTEXITCODE -ne 0) {
            throw "Data rebuild failed with exit code $LASTEXITCODE."
        }

        $dsd = Join-Path $repoRoot 'dsd.exe'
        & $dsd rom build --config $dataRomConfig --rom $outputRom
        if ($LASTEXITCODE -ne 0) {
            throw "Modded ROM packaging failed with exit code $LASTEXITCODE."
        }
    }
} finally {
    Pop-Location
}

if (-not (Test-Path -LiteralPath $outputRom -PathType Leaf)) {
    throw "The build completed without producing $outputRom."
}

Restore-FixedLayoutHeaderChecksums -BaseRom $baseRom -BuiltRom $outputRom

$sha1 = (Get-FileHash -LiteralPath $outputRom -Algorithm SHA1).Hash.ToLowerInvariant()
Write-Host ''
Write-Host "Built ROM: $outputRom"
Write-Host "SHA-1:    $sha1"
if ($null -ne $resolvedDataProject) {
    Write-Host "Data:     $resolvedDataProject"
}
if (Test-Path -LiteralPath $linkedSources -PathType Leaf) {
    Write-Host 'Linked C sources:'
    Get-Content -LiteralPath $linkedSources |
        ForEach-Object { $_.Split('#', 2)[0].Trim() } |
        Where-Object { -not [string]::IsNullOrWhiteSpace($_) } |
        ForEach-Object { Write-Host "  $_" }
}

if (-not [string]::IsNullOrWhiteSpace($EmulatorPath)) {
    if (-not (Test-Path -LiteralPath $EmulatorPath -PathType Leaf)) {
        throw "Emulator executable not found: $EmulatorPath"
    }

    $emulatorDirectory = Split-Path -Parent $EmulatorPath
    Write-Host "Starting:  $EmulatorPath"
    Start-Process -FilePath $EmulatorPath `
        -ArgumentList @("`"$outputRom`"") `
        -WorkingDirectory $emulatorDirectory
}
