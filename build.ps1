#!/usr/bin/env pwsh

param(
    [Alias("p")]
    [int]$Parallel = 16,
    
    [switch]$Clean,
    [switch]$Rebuild,
    [Alias("q")]
    [switch]$Quiet,
    [switch]$Help,
    [Alias("n")]
    [string]$Name,
    [Alias("s")]
    [ValidateRange(1, 8)]
    [int]$Slot,
    [Alias("u")]
    [switch]$Upload,
    [Alias("U")]
    [string]$NetworkUpload,
    [int]$Run = -1,
    [int]$Stop = -1
)

function Write-ColorOutput {
    param($Message, $Color = "White")
    Write-Host $Message -ForegroundColor $Color
}

function Show-Help {
    Write-ColorOutput "VEX V5 Build Script" "Blue"
    Write-Host ""
    Write-Host "Usage: ./build.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -p, -Parallel <num>           Number of parallel jobs (default: 16)"
    Write-Host "  -c, -Clean                    Clean build artifacts"
    Write-Host "  -r, -Rebuild                  Clean and rebuild"
    Write-Host "  -q, -Quiet                    Suppress compiler warnings"
    Write-Host "  -n, -Name <string>            Set project name in vex_project_settings.json"
    Write-Host "  -s, -Slot <1-8>               Set project slot in vex_project_settings.json"
    Write-Host "  -u, -Upload                   Upload the built binary to VEX brain via USB"
    Write-Host "  -U, -NetworkUpload <addr>     Upload the built binary over network (e.g., localhost:8080)"
    Write-Host "  -Run [slot]                   Run program in specified slot (or use slot from settings)"
    Write-Host "  -Stop [slot]                  Stop program in specified slot (or use slot from settings)"
    Write-Host "  -h, -Help                     Show this help"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  ./build.ps1                   # Default build with 16 parallel jobs"
    Write-Host "  ./build.ps1 -Parallel 4       # Build with 4 parallel jobs"
    Write-Host "  ./build.ps1 -Clean            # Clean build artifacts"
    Write-Host "  ./build.ps1 -Rebuild          # Clean and rebuild"
    Write-Host "  ./build.ps1 -Quiet            # Build quietly (suppress warnings)"
    Write-Host "  ./build.ps1 -Rebuild -Quiet   # Clean, rebuild quietly"
    Write-Host "  ./build.ps1 -Name 'MyBot'     # Set project name and build"
    Write-Host "  ./build.ps1 -Slot 3           # Set project slot and build"
    Write-Host "  ./build.ps1 -n 'MyBot' -s 2   # Set both name and slot, then build"
    Write-Host "  ./build.ps1 -Upload           # Build and upload to VEX brain via USB"
    Write-Host "  ./build.ps1 -s 3 -Upload      # Build and upload to slot 3 via USB"
    Write-Host "  ./build.ps1 -NetworkUpload localhost:8080  # Build and upload over network"
    Write-Host "  ./build.ps1 -s 3 -NetworkUpload 192.168.1.100:8080  # Build and upload to slot 3 over network"
    Write-Host "  ./build.ps1 -Run              # Run program in slot from settings"
    Write-Host "  ./build.ps1 -Run 3            # Run program in slot 3"
    Write-Host "  ./build.ps1 -Stop             # Stop program in slot from settings"
    Write-Host "  ./build.ps1 -Stop 3           # Stop program in slot 3"
    exit 0
}

if ($Help) {
    Show-Help
}

# check for conflicting upload options
if ($Upload -and $NetworkUpload) {
    Write-ColorOutput "Error: Cannot use both -Upload and -NetworkUpload" "Red"
    exit 1
}

# check for conflicting run/stop options
if ($Run -ge 0 -and $Stop -ge 0) {
    Write-ColorOutput "Error: Cannot use both -Run and -Stop" "Red"
    exit 1
}

# check if run/stop without network upload address
if (($Run -ge 0 -or $Stop -ge 0) -and -not $NetworkUpload) {
    # Default to localhost:80 if no address specified
    $NetworkUpload = "localhost:80"
}

$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $ProjectRoot "build"
$ToolchainFile = Join-Path $ProjectRoot "VexV5Toolchain.cmake"
$VexProjectSettingsFile = Join-Path (Join-Path $ProjectRoot ".vscode") "vex_project_settings.json"

# update vex_project_settings.json if name or slot parameters are provided
if ($Name -or $Slot) {
    if (Test-Path $VexProjectSettingsFile) {
        try {
            $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
            $Modified = $false
            
            if ($Name) {
                $OldName = $VexSettings.project.name
                $VexSettings.project.name = $Name
                Write-ColorOutput "Updated project name from '$OldName' to '$Name'" "Green"
                $Modified = $true
            }
            
            if ($Slot) {
                $OldSlot = $VexSettings.project.slot
                $VexSettings.project.slot = $Slot
                Write-ColorOutput "Updated project slot from $OldSlot to $Slot" "Green"
                $Modified = $true
            }
            
            if ($Modified) {
                $VexSettings | ConvertTo-Json -Depth 10 | Set-Content $VexProjectSettingsFile -Encoding UTF8
                Write-ColorOutput "Updated vex_project_settings.json" "Green"
            }
        } catch {
            Write-ColorOutput "Error: Could not update vex_project_settings.json: $_" "Red"
            exit 1
        }
    } else {
        Write-ColorOutput "Error: vex_project_settings.json not found, cannot update name/slot" "Red"
        exit 1
    }
}

# read project name from VEX project settings
$ProjectName = "VexProject" # default fallback
if (Test-Path $VexProjectSettingsFile) {
    try {
        $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
        if ($VexSettings.project -and $VexSettings.project.name) {
            $ProjectName = $VexSettings.project.name
        }
    } catch {
        Write-ColorOutput "Warning: Could not read project name from vex_project_settings.json, using default: $ProjectName" "Yellow"
    }
} else {
    Write-ColorOutput "Warning: vex_project_settings.json not found, using default project name: $ProjectName" "Yellow"
}

Set-Location $ProjectRoot

Write-ColorOutput "VEX V5 Build Script" "Blue"
Write-ColorOutput "Project: $ProjectRoot" "Yellow"
Write-ColorOutput "Project Name: $ProjectName" "Yellow"
Write-ColorOutput "Build Directory: $BuildDir" "Yellow"

if ($Clean -or $Rebuild) {
    Write-ColorOutput "Cleaning build directory..." "Yellow"
    if (Test-Path $BuildDir) {
        Remove-Item $BuildDir -Recurse -Force
        Write-ColorOutput "Build directory cleaned" "Green"
    } else {
        Write-ColorOutput "Build directory doesn't exist, nothing to clean" "Yellow"
    }
    
    if ($Clean -and -not $Rebuild) {
        Write-ColorOutput "Clean complete" "Green"
        exit 0
    }
}

# check for situations where configuration is needed
$NeedsConfigure = $false
if (-not (Test-Path $BuildDir)) {
    $NeedsConfigure = $true
    Write-ColorOutput "Build directory doesn't exist" "Yellow"
} elseif (-not (Test-Path (Join-Path $BuildDir "CMakeCache.txt"))) {
    $NeedsConfigure = $true
    Write-ColorOutput "CMake cache not found" "Yellow"
} elseif ((Get-Item $ToolchainFile).LastWriteTime -gt (Get-Item (Join-Path $BuildDir "CMakeCache.txt")).LastWriteTime) {
    $NeedsConfigure = $true
    Write-ColorOutput "Toolchain file is newer than cache" "Yellow"
} else {
    # check if project name has changed
    $CacheFile = Join-Path $BuildDir "CMakeCache.txt"
    if (Test-Path $CacheFile) {
        $CacheContent = Get-Content $CacheFile -Raw
        $CachedProjectName = $null
        
        # try to get VEX_PROJECT_NAME from cache
        if ($CacheContent -match "VEX_PROJECT_NAME:STRING=([^`r`n]*)") {
            $CachedProjectName = $Matches[1]
        }
        # try to get project name from CMAKE_PROJECT_NAME
        elseif ($CacheContent -match "CMAKE_PROJECT_NAME:STATIC=([^`r`n]*)") {
            $CachedProjectName = $Matches[1]
        }
        
        if ($CachedProjectName -and $CachedProjectName -ne $ProjectName) {
            $NeedsConfigure = $true
            Write-ColorOutput "Project name changed from '$CachedProjectName' to '$ProjectName'" "Yellow"
        } elseif (-not $CachedProjectName) {
            # no cached project name found, need to reconfigure
            $NeedsConfigure = $true
            Write-ColorOutput "No cached project name found, reconfiguring" "Yellow"
        }
    }
}

# configure if needed
if ($NeedsConfigure) {
    Write-ColorOutput "Configuring CMake..." "Yellow"
    
    # clean up old output files before reconfiguring
    if (Test-Path $BuildDir) {
        Write-ColorOutput "Cleaning old output files..." "Yellow"
        Get-ChildItem $BuildDir -Filter "*.elf" | Remove-Item -Force -ErrorAction SilentlyContinue
        Get-ChildItem $BuildDir -Filter "*.bin" | Remove-Item -Force -ErrorAction SilentlyContinue
        Get-ChildItem $BuildDir -Filter "*.map" | Remove-Item -Force -ErrorAction SilentlyContinue
        
        # clean up old CMakeFiles project directories
        $CMakeFilesDir = Join-Path $BuildDir "CMakeFiles"
        if (Test-Path $CMakeFilesDir) {
            Get-ChildItem $CMakeFilesDir -Directory | Where-Object { $_.Name -match "\.dir$" } | Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
        }
    }
    
    $ConfigureArgs = @(
        "-B", $BuildDir,
        "-DVEX_PROJECT_NAME=$ProjectName",
        "-G", "Unix Makefiles"
    )
    
    if ($Quiet) {
        $ConfigureArgs += "-DVEX_QUIET_BUILD=ON"
    }
    
    Write-ColorOutput "Running: cmake $($ConfigureArgs -join ' ')" "Blue"
    
    try {
        & cmake @ConfigureArgs
        if ($LASTEXITCODE -ne 0) {
            Write-ColorOutput "CMake configuration failed!" "Red"
            exit $LASTEXITCODE
        }
        Write-ColorOutput "CMake configuration successful" "Green"
    } catch {
        Write-ColorOutput "CMake configuration failed: $_" "Red"
        exit 1
    }
} else {
    Write-ColorOutput "Configuration is up to date" "Green"
}

Write-ColorOutput "Building with $Parallel parallel jobs..." "Yellow"

$BuildArgs = @(
    "--build", $BuildDir,
    "--parallel", $Parallel
)

Write-ColorOutput "Running: cmake $($BuildArgs -join ' ')" "Blue"

try {
    $BuildStart = Get-Date
    & cmake @BuildArgs
    if ($LASTEXITCODE -ne 0) {
        Write-ColorOutput "Build failed!" "Red"
        exit $LASTEXITCODE
    }
    $BuildEnd = Get-Date
    $BuildTime = ($BuildEnd - $BuildStart).TotalSeconds
    
    $BuildTimeFormatted = "{0:F1}" -f $BuildTime
    Write-ColorOutput "Build successful! ($BuildTimeFormatted s)" "Green"
    
    # show output files
    $ElfFile = Join-Path $BuildDir "$ProjectName.elf"
    $BinFile = Join-Path $BuildDir "$ProjectName.bin"
    $MapFile = Join-Path $BuildDir "$ProjectName.map"
    
    if (Test-Path $ElfFile) {
        $ElfSize = (Get-Item $ElfFile).Length
        $ElfSizeFormatted = "{0:N0}" -f $ElfSize
        Write-ColorOutput "Output: $ProjectName.elf ($ElfSizeFormatted bytes)" "White"
    }
    
    if (Test-Path $BinFile) {
        $BinSize = (Get-Item $BinFile).Length
        $BinSizeFormatted = "{0:N0}" -f $BinSize
        Write-ColorOutput "Output: $ProjectName.bin ($BinSizeFormatted bytes)" "White"
    }
    
    if (Test-Path $MapFile) {
        $MapSize = (Get-Item $MapFile).Length
        $MapSizeFormatted = "{0:N0}" -f $MapSize
        Write-ColorOutput "Output: $ProjectName.map ($MapSizeFormatted bytes)" "White"
    }
    
} catch {
    Write-ColorOutput "Build failed: $_" "Red"
    exit 1
}

$CompletionTime = Get-Date -Format "HH:mm:ss"
Write-ColorOutput "Build completed at $CompletionTime" "White"

# upload via USB if requested
if ($Upload) {
    $BinFile = Join-Path $BuildDir "$ProjectName.bin"
    if (-not (Test-Path $BinFile)) {
        Write-ColorOutput "Error: Binary file not found: $BinFile" "Red"
        exit 1
    }
    
    # determine vexcom executable based on platform
    $VexcomExe = if ($IsWindows -or $env:OS -eq "Windows_NT") { "vexcom.exe" } else { "vexcom" }
    
    # find vexcom in toolchain or PATH
    $VexcomPath = $null
    
    # determine the correct VEX toolchain path based on platform
    $VexGlobalDir = if ($IsWindows -or $env:OS -eq "Windows_NT") { 
        Join-Path $env:USERPROFILE ".vex\vexcode"
    } else { 
        Join-Path $env:HOME ".vex/vexcode"
    }
    
    $ToolchainSubdir = if ($IsWindows -or $env:OS -eq "Windows_NT") { 
        "toolchain_win32"
    } elseif ($IsMacOS) { 
        "toolchain_osx64"
    } else { 
        "toolchain_linux64"
    }
    
    $VexToolchainPath = Join-Path $VexGlobalDir $ToolchainSubdir
    
    # first try to find it in the VEX toolchain
    $ToolchainVexcom = Join-Path (Join-Path (Join-Path $VexToolchainPath "tools") "vexcom") $VexcomExe
    if (Test-Path $ToolchainVexcom) {
        $VexcomPath = $ToolchainVexcom
    } else {
        # also try the environment variable as fallback
        if ($env:VEX_TOOLCHAIN_PATH) {
            $EnvToolchainVexcom = Join-Path (Join-Path (Join-Path $env:VEX_TOOLCHAIN_PATH "tools") "vexcom") $VexcomExe
            if (Test-Path $EnvToolchainVexcom) {
                $VexcomPath = $EnvToolchainVexcom
            }
        }
    }
    
    # fallback to PATH
    if (-not $VexcomPath) {
        try {
            $VexcomPath = (Get-Command $VexcomExe -ErrorAction Stop).Source
        } catch {
            Write-ColorOutput "Error: vexcom not found in toolchain or PATH" "Red"
            Write-ColorOutput "Make sure VEX toolchain is properly installed" "Red"
            exit 1
        }
    }
    
    # get upload slot number
    $UploadSlot = 1  # default to 1
    if ($Slot) {
        $UploadSlot = $Slot
    } elseif (Test-Path $VexProjectSettingsFile) {
        try {
            $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
            if ($VexSettings.project -and $VexSettings.project.slot) {
                $UploadSlot = $VexSettings.project.slot
            }
        } catch {
            Write-ColorOutput "Warning: Could not read slot from vex_project_settings.json, using slot $UploadSlot" "Yellow"
        }
    }
    
    $UploadArgs = @(
        "-w", $BinFile,
        "--progress",
        "-s", $UploadSlot
    )
    
    Write-ColorOutput "Uploading $ProjectName.bin to slot $UploadSlot..." "Yellow"
    
    try {
        & $VexcomPath @UploadArgs
        if ($LASTEXITCODE -ne 0) {
            Write-ColorOutput "Upload failed!" "Red"
            exit $LASTEXITCODE
        }
        Write-ColorOutput "Upload successful!" "Green"
    } catch {
        Write-ColorOutput "Upload failed: $_" "Red"
        exit 1
    }
}
# upload over network if requested
if ($NetworkUpload) {
    Write-ColorOutput "Uploading to VEX brain over network..." "Yellow"
    
    $BinFile = Join-Path $BuildDir "$ProjectName.bin"
    if (-not (Test-Path $BinFile)) {
        Write-ColorOutput "Error: Binary file not found: $BinFile" "Red"
        exit 1
    }
    
    # check if curl is available
    $CurlPath = $null
    try {
        $CurlPath = (Get-Command curl -ErrorAction Stop).Source
    } catch {
        Write-ColorOutput "Error: curl is required for network upload but is not installed" "Red"
        exit 1
    }
    
    # get upload slot number
    $UploadSlot = 1  # default to 1
    if ($Slot) {
        $UploadSlot = $Slot
    } elseif (Test-Path $VexProjectSettingsFile) {
        try {
            $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
            if ($VexSettings.project -and $VexSettings.project.slot) {
                $UploadSlot = $VexSettings.project.slot
            }
        } catch {
            Write-ColorOutput "Warning: Could not read slot from vex_project_settings.json, using slot $UploadSlot" "Yellow"
        }
    }
    
    # get project description if available
    $ProjectDescription = ""
    if (Test-Path $VexProjectSettingsFile) {
        try {
            $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
            if ($VexSettings.project -and $VexSettings.project.description) {
                $ProjectDescription = $VexSettings.project.description
            }
        } catch {
            # ignore errors
        }
    }
    
    if (-not $ProjectDescription) {
        $ProjectDescription = "Built with build.ps1"
    }
    
    # create temp files for upload
    $TmpJson = Join-Path $env:TEMP "vex_upload_$(Get-Random).json"
    $TmpB64 = Join-Path $env:TEMP "vex_upload_$(Get-Random).b64"
    $TmpResponse = Join-Path $env:TEMP "vex_upload_$(Get-Random).response"
    
    try {
        # convert binary to base64
        Write-ColorOutput "Encoding binary to base64..." "Yellow"
        $BinBytes = [System.IO.File]::ReadAllBytes($BinFile)
        $Base64String = [System.Convert]::ToBase64String($BinBytes)
        [System.IO.File]::WriteAllText($TmpB64, $Base64String)
        
        # construct the upload URL
        if ($NetworkUpload -match "^https?://") {
            $UploadUrl = $NetworkUpload
        } else {
            $UploadUrl = "http://$NetworkUpload"
        }
        
        # ensure URL ends with /upload
        if ($UploadUrl -notmatch "/upload$") {
            $UploadUrl = $UploadUrl.TrimEnd('/') + "/upload"
        }
        
        Write-ColorOutput "Uploading $ProjectName.bin to slot $UploadSlot at $UploadUrl..." "Yellow"
        
        # escape JSON strings
        $EscName = $ProjectName -replace '"', '\"'
        $EscDesc = $ProjectDescription -replace '"', '\"'
        
        # create JSON payload
        $JsonPayload = @"
{
  "slot": $UploadSlot,
  "name": "$EscName",
  "description": "$EscDesc",
  "compress": true,
  "after_upload": "show_run_screen",
  "monolith_b64": "$(Get-Content $TmpB64 -Raw)"
}
"@
        
        [System.IO.File]::WriteAllText($TmpJson, $JsonPayload)
        
        # upload via HTTP POST using curl
        $CurlArgs = @(
            "-sS",
            "-o", $TmpResponse,
            "-w", "%{http_code}",
            "-X", "POST",
            "-H", "Content-Type: application/json",
            "--data-binary", "@$TmpJson",
            $UploadUrl
        )
        
        $HttpCode = & $CurlPath @CurlArgs
        $ResponseBody = if (Test-Path $TmpResponse) { Get-Content $TmpResponse -Raw } else { "" }
        
        if ($HttpCode -eq "200") {
            Write-ColorOutput "Upload successful!" "Green"
            if ($ResponseBody) {
                Write-ColorOutput "Server response: $ResponseBody" "Green"
            }
        } else {
            Write-ColorOutput "Upload failed with HTTP code: $HttpCode" "Red"
            if ($ResponseBody) {
                Write-ColorOutput "Server response: $ResponseBody" "Red"
            }
            exit 1
        }
    } finally {
        # cleanup temp files
        Remove-Item $TmpJson -Force -ErrorAction SilentlyContinue
        Remove-Item $TmpB64 -Force -ErrorAction SilentlyContinue
        Remove-Item $TmpResponse -Force -ErrorAction SilentlyContinue
    }
}

# run program if requested
if ($Run -ge 0) {
    Write-ColorOutput "Running program on VEX brain..." "Yellow"
    
    # check if curl is available
    $CurlPath = $null
    try {
        $CurlPath = (Get-Command curl -ErrorAction Stop).Source
    } catch {
        Write-ColorOutput "Error: curl is required for running programs but is not installed" "Red"
        exit 1
    }
    
    # get slot number
    $RunSlot = 1  # default to 1
    if ($Run -gt 0) {
        $RunSlot = $Run
    } elseif ($Slot) {
        $RunSlot = $Slot
    } elseif (Test-Path $VexProjectSettingsFile) {
        try {
            $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
            if ($VexSettings.project -and $VexSettings.project.slot) {
                $RunSlot = $VexSettings.project.slot
            }
        } catch {
            # ignore errors
        }
    }
    
    # construct the run URL
    if ($NetworkUpload -match "^https?://") {
        $RunUrl = $NetworkUpload
    } else {
        $RunUrl = "http://$NetworkUpload"
    }
    
    # ensure URL ends with /run
    if ($RunUrl -notmatch "/run$") {
        $RunUrl = $RunUrl.TrimEnd('/') + "/run"
    }
    
    Write-ColorOutput "Running program in slot $RunSlot at $RunUrl..." "Yellow"
    
    # create temp files
    $TmpRunJson = Join-Path $env:TEMP "vex_run_$(Get-Random).json"
    $TmpRunResponse = Join-Path $env:TEMP "vex_run_$(Get-Random).response"
    
    try {
        # create JSON payload
        $RunJsonPayload = @"
{
  "slot": $RunSlot
}
"@
        
        [System.IO.File]::WriteAllText($TmpRunJson, $RunJsonPayload)
        
        # run via HTTP POST
        $CurlArgs = @(
            "-sS",
            "-o", $TmpRunResponse,
            "-w", "%{http_code}",
            "-X", "POST",
            "-H", "Content-Type: application/json",
            "--data-binary", "@$TmpRunJson",
            $RunUrl
        )
        
        $HttpCode = & $CurlPath @CurlArgs
        $ResponseBody = if (Test-Path $TmpRunResponse) { Get-Content $TmpRunResponse -Raw } else { "" }
        
        if ($HttpCode -eq "200") {
            Write-ColorOutput "Program started successfully!" "Green"
            if ($ResponseBody) {
                Write-ColorOutput "Server response: $ResponseBody" "Green"
            }
        } else {
            Write-ColorOutput "Run failed with HTTP code: $HttpCode" "Red"
            if ($ResponseBody) {
                Write-ColorOutput "Server response: $ResponseBody" "Red"
            }
            exit 1
        }
    } finally {
        # cleanup temp files
        Remove-Item $TmpRunJson -Force -ErrorAction SilentlyContinue
        Remove-Item $TmpRunResponse -Force -ErrorAction SilentlyContinue
    }
}

# stop program if requested
if ($Stop -ge 0) {
    Write-ColorOutput "Stopping program on VEX brain..." "Yellow"
    
    # check if curl is available
    $CurlPath = $null
    try {
        $CurlPath = (Get-Command curl -ErrorAction Stop).Source
    } catch {
        Write-ColorOutput "Error: curl is required for stopping programs but is not installed" "Red"
        exit 1
    }
    
    # get slot number
    $StopSlot = 1  # default to 1
    if ($Stop -gt 0) {
        $StopSlot = $Stop
    } elseif ($Slot) {
        $StopSlot = $Slot
    } elseif (Test-Path $VexProjectSettingsFile) {
        try {
            $VexSettings = Get-Content $VexProjectSettingsFile -Raw | ConvertFrom-Json
            if ($VexSettings.project -and $VexSettings.project.slot) {
                $StopSlot = $VexSettings.project.slot
            }
        } catch {
            # ignore errors
        }
    }
    
    # construct the stop URL
    if ($NetworkUpload -match "^https?://") {
        $StopUrl = $NetworkUpload
    } else {
        $StopUrl = "http://$NetworkUpload"
    }
    
    # ensure URL ends with /stop
    if ($StopUrl -notmatch "/stop$") {
        $StopUrl = $StopUrl.TrimEnd('/') + "/stop"
    }
    
    Write-ColorOutput "Stopping program in slot $StopSlot at $StopUrl..." "Yellow"
    
    # create temp files
    $TmpStopJson = Join-Path $env:TEMP "vex_stop_$(Get-Random).json"
    $TmpStopResponse = Join-Path $env:TEMP "vex_stop_$(Get-Random).response"
    
    try {
        # create JSON payload
        $StopJsonPayload = @"
{
  "slot": $StopSlot
}
"@
        
        [System.IO.File]::WriteAllText($TmpStopJson, $StopJsonPayload)
        
        # stop via HTTP POST
        $CurlArgs = @(
            "-sS",
            "-o", $TmpStopResponse,
            "-w", "%{http_code}",
            "-X", "POST",
            "-H", "Content-Type: application/json",
            "--data-binary", "@$TmpStopJson",
            $StopUrl
        )
        
        $HttpCode = & $CurlPath @CurlArgs
        $ResponseBody = if (Test-Path $TmpStopResponse) { Get-Content $TmpStopResponse -Raw } else { "" }
        
        if ($HttpCode -eq "200") {
            Write-ColorOutput "Program stopped successfully!" "Green"
            if ($ResponseBody) {
                Write-ColorOutput "Server response: $ResponseBody" "Green"
            }
        } else {
            Write-ColorOutput "Stop failed with HTTP code: $HttpCode" "Red"
            if ($ResponseBody) {
                Write-ColorOutput "Server response: $ResponseBody" "Red"
            }
            exit 1
        }
    } finally {
        # cleanup temp files
        Remove-Item $TmpStopJson -Force -ErrorAction SilentlyContinue
        Remove-Item $TmpStopResponse -Force -ErrorAction SilentlyContinue
    }
}