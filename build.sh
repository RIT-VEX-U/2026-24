#!/bin/bash

set -e  # exit on any error

# default values
PARALLEL=16
CLEAN=false
REBUILD=false
HELP=false
QUIET=false
NAME=""
SLOT=""
UPLOAD=false

# colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_color() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

show_help() {
    print_color $BLUE "VEX V5 Build Script"
    echo ""
    echo "Usage: ./build.sh [options]"
    echo ""
    echo "Options:"
    echo "  -p, --parallel <num>          Number of parallel jobs (default: 16)"
    echo "  -c, --clean                   Clean build artifacts"
    echo "  -r, --rebuild                 Clean and rebuild"
    echo "  -q, --quiet                   Suppress compiler warnings"
    echo "  -n, --name <string>           Set project name in vex_project_settings.json"
    echo "  -s, --slot <1-8>              Set project slot in vex_project_settings.json"
    echo "  -u, --upload                  Upload the built binary to VEX brain"
    echo "  -h, --help                    Show this help"
    echo ""
    echo "Examples:"
    echo "  ./build.sh                    # Default build with 16 parallel jobs"
    echo "  ./build.sh --parallel 4       # Build with 4 parallel jobs"
    echo "  ./build.sh --clean            # Clean build artifacts"
    echo "  ./build.sh --rebuild          # Clean and rebuild"
    echo "  ./build.sh --quiet            # Build quietly (suppress warnings)"
    echo "  ./build.sh --rebuild --quiet  # Clean, rebuild quietly"
    echo "  ./build.sh --name 'MyBot'     # Set project name and build"
    echo "  ./build.sh --slot 3           # Set project slot and build"
    echo "  ./build.sh -n 'MyBot' -s 2    # Set both name and slot, then build"
    echo "  ./build.sh --upload           # Build and upload to VEX brain"
    echo "  ./build.sh -s 3 --upload      # Build and upload to slot 3"
    exit 0
}

# parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--parallel)
            PARALLEL="$2"
            if ! [[ "$PARALLEL" =~ ^[0-9]+$ ]] || [ "$PARALLEL" -lt 1 ]; then
                print_color $RED "Error: Parallel jobs must be a positive integer"
                exit 1
            fi
            shift 2
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -r|--rebuild)
            REBUILD=true
            shift
            ;;
        -q|--quiet)
            QUIET=true
            shift
            ;;
        -n|--name)
            NAME="$2"
            if [ -z "$NAME" ]; then
                print_color $RED "Error: --name requires a project name"
                exit 1
            fi
            shift 2
            ;;
        -s|--slot)
            SLOT="$2"
            if ! [[ "$SLOT" =~ ^[1-8]$ ]]; then
                print_color $RED "Error: --slot must be a number between 1 and 8"
                exit 1
            fi
            shift 2
            ;;
        -u|--upload)
            UPLOAD=true
            shift
            ;;
        -h|--help)
            HELP=true
            shift
            ;;
        *)
            print_color $RED "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

if [ "$HELP" = true ]; then
    show_help
fi

# get script directory (project root)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_ROOT/build"
TOOLCHAIN_FILE="$PROJECT_ROOT/cmake/VexV5Toolchain.cmake"
VEX_PROJECT_SETTINGS_FILE="$PROJECT_ROOT/.vscode/vex_project_settings.json"

# update vex_project_settings.json if name or slot parameters are provided
if [ -n "$NAME" ] || [ -n "$SLOT" ]; then
    if [ -f "$VEX_PROJECT_SETTINGS_FILE" ]; then
        if command -v jq >/dev/null 2>&1; then
            # Use jq for JSON manipulation
            TEMP_FILE=$(mktemp)
            MODIFIED=false
            
            if [ -n "$NAME" ]; then
                OLD_NAME=$(jq -r '.project.name' "$VEX_PROJECT_SETTINGS_FILE")
                jq --arg name "$NAME" '.project.name = $name' "$VEX_PROJECT_SETTINGS_FILE" > "$TEMP_FILE"
                cp "$TEMP_FILE" "$VEX_PROJECT_SETTINGS_FILE"
                print_color $GREEN "Updated project name from '$OLD_NAME' to '$NAME'"
                MODIFIED=true
            fi
            
            if [ -n "$SLOT" ]; then
                OLD_SLOT=$(jq -r '.project.slot' "$VEX_PROJECT_SETTINGS_FILE")
                jq --argjson slot "$SLOT" '.project.slot = $slot' "$VEX_PROJECT_SETTINGS_FILE" > "$TEMP_FILE"
                cp "$TEMP_FILE" "$VEX_PROJECT_SETTINGS_FILE"
                print_color $GREEN "Updated project slot from $OLD_SLOT to $SLOT"
                MODIFIED=true
            fi
            
            rm -f "$TEMP_FILE"
            
            if [ "$MODIFIED" = true ]; then
                print_color $GREEN "Updated vex_project_settings.json"
            fi
        else
            print_color $RED "Error: jq is required to update vex_project_settings.json but is not installed"
            print_color $YELLOW "Please install jq or update the JSON file manually"
            exit 1
        fi
    else
        print_color $RED "Error: vex_project_settings.json not found, cannot update name/slot"
        exit 1
    fi
fi

# read project name from VEX project settings
PROJECT_NAME="VexProject"  # Default fallback
if [ -f "$VEX_PROJECT_SETTINGS_FILE" ]; then
    if command -v jq >/dev/null 2>&1; then
        # use jq if available
        VEX_PROJECT_NAME=$(jq -r '.project.name // empty' "$VEX_PROJECT_SETTINGS_FILE" 2>/dev/null)
        if [ -n "$VEX_PROJECT_NAME" ] && [ "$VEX_PROJECT_NAME" != "null" ]; then
            PROJECT_NAME="$VEX_PROJECT_NAME"
        fi
    else
        # try to parse name without jq
        VEX_PROJECT_NAME=$(grep -o '"name"[[:space:]]*:[[:space:]]*"[^"]*"' "$VEX_PROJECT_SETTINGS_FILE" | head -1 | sed 's/.*"name"[[:space:]]*:[[:space:]]*"\([^"]*\)".*/\1/')
        if [ -n "$VEX_PROJECT_NAME" ]; then
            PROJECT_NAME="$VEX_PROJECT_NAME"
        else
            print_color $YELLOW "Warning: Could not parse project name from vex_project_settings.json, using default: $PROJECT_NAME"
        fi
    fi
else
    print_color $YELLOW "Warning: vex_project_settings.json not found, using default project name: $PROJECT_NAME"
fi

cd "$PROJECT_ROOT"

print_color $BLUE "VEX V5 Build Script"
print_color $YELLOW "Project: $PROJECT_ROOT"
print_color $YELLOW "Project Name: $PROJECT_NAME"
print_color $YELLOW "Build Directory: $BUILD_DIR"

if [ "$CLEAN" = true ] || [ "$REBUILD" = true ]; then
    print_color $YELLOW "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_color $GREEN "Build directory cleaned"
    else
        print_color $YELLOW "Build directory doesn't exist, nothing to clean"
    fi
    
    if [ "$CLEAN" = true ] && [ "$REBUILD" = false ]; then
        print_color $GREEN "Clean complete"
        exit 0
    fi
fi

# check for situations where configuration is needed
NEEDS_CONFIGURE=false
if [ ! -d "$BUILD_DIR" ]; then
    NEEDS_CONFIGURE=true
    print_color $YELLOW "Build directory doesn't exist"
elif [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
    NEEDS_CONFIGURE=true
    print_color $YELLOW "CMake cache not found"
elif [ "$TOOLCHAIN_FILE" -nt "$BUILD_DIR/CMakeCache.txt" ]; then
    NEEDS_CONFIGURE=true
    print_color $YELLOW "Toolchain file is newer than cache"
else
    # check if project name has changed
    CACHE_FILE="$BUILD_DIR/CMakeCache.txt"
    if [ -f "$CACHE_FILE" ]; then
        CACHED_PROJECT_NAME=""
        
        # try to get VEX_PROJECT_NAME from cache
        CACHED_PROJECT_NAME=$(grep "VEX_PROJECT_NAME:STRING=" "$CACHE_FILE" 2>/dev/null | sed 's/.*VEX_PROJECT_NAME:STRING=\(.*\)/\1/')
        
        # try to get project name from CMAKE_PROJECT_NAME
        if [ -z "$CACHED_PROJECT_NAME" ]; then
            CACHED_PROJECT_NAME=$(grep "CMAKE_PROJECT_NAME:STATIC=" "$CACHE_FILE" 2>/dev/null | sed 's/.*CMAKE_PROJECT_NAME:STATIC=\(.*\)/\1/')
        fi
        
        if [ -n "$CACHED_PROJECT_NAME" ]; then
            if [ "$CACHED_PROJECT_NAME" != "$PROJECT_NAME" ]; then
                NEEDS_CONFIGURE=true
                print_color $YELLOW "Project name changed from '$CACHED_PROJECT_NAME' to '$PROJECT_NAME'"
            fi
        else
            # no cached project name found, need to reconfigure
            NEEDS_CONFIGURE=true
            print_color $YELLOW "No cached project name found, reconfiguring"
        fi
    fi
fi

# configure if needed
if [ "$NEEDS_CONFIGURE" = true ]; then
    print_color $YELLOW "Configuring CMake..."
    
    # clean up old output files before reconfiguring
    if [ -d "$BUILD_DIR" ]; then
        print_color $YELLOW "Cleaning old output files..."
        find "$BUILD_DIR" -maxdepth 1 -name "*.elf" -delete 2>/dev/null || true
        find "$BUILD_DIR" -maxdepth 1 -name "*.bin" -delete 2>/dev/null || true
        find "$BUILD_DIR" -maxdepth 1 -name "*.map" -delete 2>/dev/null || true
        
        # clean up old CMakeFiles project directories
        CMAKE_FILES_DIR="$BUILD_DIR/CMakeFiles"
        if [ -d "$CMAKE_FILES_DIR" ]; then
            find "$CMAKE_FILES_DIR" -maxdepth 1 -type d -name "*.dir" -exec rm -rf {} + 2>/dev/null || true
        fi
    fi
    
    CONFIGURE_CMD="cmake -B $BUILD_DIR -G \"Unix Makefiles\" -DVEX_PROJECT_NAME=$PROJECT_NAME"
    if [ "$QUIET" = true ]; then
        CONFIGURE_CMD="$CONFIGURE_CMD -DVEX_QUIET_BUILD=ON"
    fi
    
    print_color $BLUE "Running: $CONFIGURE_CMD"
    
    if ! eval "$CONFIGURE_CMD"; then
        print_color $RED "CMake configuration failed!"
        exit 1
    fi
    print_color $GREEN "CMake configuration successful"
else
    print_color $GREEN "Configuration is up to date"
fi

print_color $YELLOW "Building with $PARALLEL parallel jobs..."

BUILD_CMD="cmake --build $BUILD_DIR --parallel $PARALLEL"
print_color $BLUE "Running: $BUILD_CMD"

BUILD_START=$(date +%s)
if ! eval "$BUILD_CMD"; then
    print_color $RED "Build failed!"
    exit 1
fi
BUILD_END=$(date +%s)
BUILD_TIME=$((BUILD_END - BUILD_START))

print_color $GREEN "Build successful! (${BUILD_TIME}.0s)"

ELF_FILE="$BUILD_DIR/$PROJECT_NAME.elf"
BIN_FILE="$BUILD_DIR/$PROJECT_NAME.bin"
MAP_FILE="$BUILD_DIR/$PROJECT_NAME.map"

if [ -f "$ELF_FILE" ]; then
    ELF_SIZE=$(stat -f%z "$ELF_FILE" 2>/dev/null || stat -c%s "$ELF_FILE" 2>/dev/null || echo "unknown")
    if [ "$ELF_SIZE" != "unknown" ]; then
        ELF_SIZE_FORMATTED=$(printf "%'d" "$ELF_SIZE" 2>/dev/null || echo "$ELF_SIZE")
        print_color $GREEN "Output: $PROJECT_NAME.elf ($ELF_SIZE_FORMATTED bytes)"
    else
        print_color $GREEN "Output: $PROJECT_NAME.elf (unknown size)"
    fi
fi

if [ -f "$BIN_FILE" ]; then
    BIN_SIZE=$(stat -f%z "$BIN_FILE" 2>/dev/null || stat -c%s "$BIN_FILE" 2>/dev/null || echo "unknown")
    if [ "$BIN_SIZE" != "unknown" ]; then
        BIN_SIZE_FORMATTED=$(printf "%'d" "$BIN_SIZE" 2>/dev/null || echo "$BIN_SIZE")
        print_color $GREEN "Output: $PROJECT_NAME.bin ($BIN_SIZE_FORMATTED bytes)"
    else
        print_color $GREEN "Output: $PROJECT_NAME.bin (unknown size)"
    fi
fi

if [ -f "$MAP_FILE" ]; then
    MAP_SIZE=$(stat -f%z "$MAP_FILE" 2>/dev/null || stat -c%s "$MAP_FILE" 2>/dev/null || echo "unknown")
    if [ "$MAP_SIZE" != "unknown" ]; then
        MAP_SIZE_FORMATTED=$(printf "%'d" "$MAP_SIZE" 2>/dev/null || echo "$MAP_SIZE")
        print_color $GREEN "Output: $PROJECT_NAME.map ($MAP_SIZE_FORMATTED bytes)"
    else
        print_color $GREEN "Output: $PROJECT_NAME.map (unknown size)"
    fi
fi

COMPLETION_TIME=$(date +"%H:%M:%S")
print_color $GREEN "Build completed at $COMPLETION_TIME"

# upload if requested
if [ "$UPLOAD" = true ]; then
    print_color $YELLOW "Uploading to VEX brain..."
    
    BIN_FILE="$BUILD_DIR/$PROJECT_NAME.bin"
    if [ ! -f "$BIN_FILE" ]; then
        print_color $RED "Error: Binary file not found: $BIN_FILE"
        exit 1
    fi
    
    # find vexcom executable
    VEXCOM_PATH=""
    
    # determine the correct VEX toolchain path based on platform
    if [ "$(uname)" = "Darwin" ]; then
        VEX_GLOBAL_DIR="$HOME/.vex/vexcode"
        TOOLCHAIN_SUBDIR="toolchain_osx64"
    else
        VEX_GLOBAL_DIR="$HOME/.vex/vexcode"
        TOOLCHAIN_SUBDIR="toolchain_linux64"
    fi
    
    VEX_TOOLCHAIN_PATH_DETECTED="$VEX_GLOBAL_DIR/$TOOLCHAIN_SUBDIR"
    
    # first try to find it in the detected VEX toolchain path
    if [ -f "$VEX_TOOLCHAIN_PATH_DETECTED/tools/vexcom/vexcom" ]; then
        VEXCOM_PATH="$VEX_TOOLCHAIN_PATH_DETECTED/tools/vexcom/vexcom"
        print_color $GREEN "Found vexcom in toolchain: $VEX_TOOLCHAIN_PATH_DETECTED"
    # try environment variable as fallback
    elif [ -n "$VEX_TOOLCHAIN_PATH" ] && [ -f "$VEX_TOOLCHAIN_PATH/tools/vexcom/vexcom" ]; then
        VEXCOM_PATH="$VEX_TOOLCHAIN_PATH/tools/vexcom/vexcom"
        print_color $GREEN "Found vexcom in environment toolchain: $VEX_TOOLCHAIN_PATH"
    # fallback to PATH
    elif command -v vexcom >/dev/null 2>&1; then
        VEXCOM_PATH="vexcom"
    else
        print_color $RED "Error: vexcom not found in toolchain or PATH"
        print_color $RED "Make sure VEX toolchain is properly installed"
        exit 1
    fi
    
    # get upload slot number
    UPLOAD_SLOT=1  # default to 1
    if [ -n "$SLOT" ]; then
        UPLOAD_SLOT="$SLOT"
    elif [ -f "$VEX_PROJECT_SETTINGS_FILE" ]; then
        if command -v jq >/dev/null 2>&1; then
            VEX_SLOT=$(jq -r '.project.slot // empty' "$VEX_PROJECT_SETTINGS_FILE" 2>/dev/null)
            if [ -n "$VEX_SLOT" ] && [ "$VEX_SLOT" != "null" ]; then
                UPLOAD_SLOT="$VEX_SLOT"
            fi
        else
            # try to parse without jq
            VEX_SLOT=$(grep -o '"slot"[[:space:]]*:[[:space:]]*[0-9]*' "$VEX_PROJECT_SETTINGS_FILE" | head -1 | sed 's/.*"slot"[[:space:]]*:[[:space:]]*\([0-9]*\).*/\1/')
            if [ -n "$VEX_SLOT" ]; then
                UPLOAD_SLOT="$VEX_SLOT"
            else
                print_color $YELLOW "Warning: Could not parse slot from vex_project_settings.json, using slot $UPLOAD_SLOT"
            fi
        fi
    fi
    
    print_color $YELLOW "Uploading $PROJECT_NAME.bin to slot $UPLOAD_SLOT..."
    
    if ! "$VEXCOM_PATH" -w "$BIN_FILE" --progress -s "$UPLOAD_SLOT"; then
        print_color $RED "Upload failed!"
        exit 1
    fi
    
    print_color $GREEN "Upload successful!"
fi
