# VEX V5 Toolchain Configuration
# The default configuration does the following:
# 1. Check for existing SDK versions in the global directory (~/.vex/vexcode/V5_*) and use the latest one found.
# 2. Set toolchain path based on platform (toolchain_win32/toolchain_osx64/toolchain_linux64) in global directory.
# 3. If SDK not found, automatically download the latest SDK version from VEX servers to global directory.
# 4. If toolchain not found, automatically download the platform-specific toolchain and vexcom to global directory.
#
# Global directory structure:
# ~/.vex/vexcode/
# |── V5_YYYYMMDD_HH_MM_SS/vexv5/           (SDK)
# └── ATfE-20.1.0-platform/                 (Toolchain)
#     ├── bin/                              (Clang compiler)
#     ├── tools/vexcom/                     (Vexcom)
#     └── tools/ninja/                      (Ninja build system)
# Options:
# -DVEX_FORCE_REINSTALL=ON: Force complete reinstallation by removing ~/.vex/vexcode directory
# -DVEX_QUIET_BUILD=ON: Suppress compiler warnings during build


# Set up global directory

if(WIN32)
  set(WINDOWS 1)
elseif(APPLE)
  set(MACOS 1)
elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
  set(AARCH64 1)
else()
  set(LINUX64 1)
endif()

if(WINDOWS)
    file(TO_CMAKE_PATH "$ENV{USERPROFILE}/.vex/vexcode" VEX_GLOBAL_DIR)
else()
    set(VEX_GLOBAL_DIR "$ENV{HOME}/.vex/vexcode")
endif()

# check if the global directory/sdk exists
if(EXISTS "${VEX_GLOBAL_DIR}")
    file(GLOB SDK_VERSIONS RELATIVE "${VEX_GLOBAL_DIR}" "${VEX_GLOBAL_DIR}/V5_*")
    if(SDK_VERSIONS)
        list(SORT SDK_VERSIONS)
        list(REVERSE SDK_VERSIONS)
        list(GET SDK_VERSIONS 0 LATEST_LOCAL_SDK_VERSION)
        set(VEX_SDK_PATH "${VEX_GLOBAL_DIR}/${LATEST_LOCAL_SDK_VERSION}/vexv5")
    endif()
endif()

# set toolchain path based on platform
if(WINDOWS)
  set(VEX_TOOLCHAIN_PATH "${VEX_GLOBAL_DIR}/ATfE-20.1.0-Windows-x86_64")
elseif(MACOS)
  set(VEX_TOOLCHAIN_PATH "${VEX_GLOBAL_DIR}/ATfE-20.1.0-Darwin-universal")
elseif(AARCH64)
  set(VEX_TOOLCHAIN_PATH "${VEX_GLOBAL_DIR}/ATfE-20.1.0-Linux-AArch64")
else()
  set(VEX_TOOLCHAIN_PATH "${VEX_GLOBAL_DIR}/ATfE-20.1.0-Linux-x86_64")
endif()

# gets the latest SDK version from the manifest.json file
function(get_latest_sdk_version OUT_VERSION)
    set(MANIFEST_URL "https://content.vexrobotics.com/vexos/public/V5/vscode/sdk/cpp/manifest.json")

    # download to temporary directory
    if(WINDOWS)
        set(TEMP_DIR "$ENV{TEMP}")
    else()
        set(TEMP_DIR "/tmp")
    endif()
    set(MANIFEST_FILE "${TEMP_DIR}/vex_manifest.json")

    message(STATUS "Fetching latest SDK version...")
    file(DOWNLOAD "${MANIFEST_URL}" "${MANIFEST_FILE}"
         STATUS DOWNLOAD_STATUS)

    list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if(NOT STATUS_CODE EQUAL 0)
        list(GET DOWNLOAD_STATUS 1 ERROR_MSG)
        message(WARNING "Failed to download manifest: ${ERROR_MSG}. Using fallback version.")
        set(${OUT_VERSION} "V5_20240802_15_00_00" PARENT_SCOPE)
        return()
    endif()

    file(READ "${MANIFEST_FILE}" MANIFEST_CONTENT)

    # find any V5_ version string in the manifest
    string(REGEX MATCH "(V5_[0-9]+_[0-9]+_[0-9]+_[0-9]+)" MATCH_RESULT "${MANIFEST_CONTENT}")
    if(CMAKE_MATCH_1)
        set(LATEST_VERSION "${CMAKE_MATCH_1}")
    endif()

    if(LATEST_VERSION)
        set(${OUT_VERSION} "${LATEST_VERSION}" PARENT_SCOPE)
        message(STATUS "Latest SDK version: ${LATEST_VERSION}")
    else()
        message(WARNING "Could not parse manifest.json. Using fallback version.")
        set(${OUT_VERSION} "V5_20240802_15_00_00" PARENT_SCOPE)
    endif()

    # clean up manifest file
    file(REMOVE "${MANIFEST_FILE}")
endfunction()

# if force reinstall, remove the global directory so it auto downloads
if(VEX_FORCE_REINSTALL)
    message(STATUS "VEX_FORCE_REINSTALL specified - removing installation")
    if(EXISTS "${VEX_GLOBAL_DIR}")
        file(REMOVE_RECURSE "${VEX_GLOBAL_DIR}")
    endif()
    set(VEX_REINSTALL_GLOBAL OFF)  # reset the flag so it doesn't happen on next run
endif()

# download SDK if not found
if(NOT VEX_SDK_PATH OR NOT EXISTS "${VEX_SDK_PATH}")
    message(STATUS "VEX SDK not found, will download automatically...")

    get_latest_sdk_version(LATEST_SDK_VERSION)

    set(SDK_URL "https://content.vexrobotics.com/vexos/public/V5/vscode/sdk/cpp/${LATEST_SDK_VERSION}.zip")
    set(SDK_EXTRACT_PATH "${VEX_GLOBAL_DIR}/${LATEST_SDK_VERSION}/vexv5")
    set(SDK_DOWNLOAD_FILE "${VEX_GLOBAL_DIR}/vex-sdk.zip")

    if(NOT EXISTS "${SDK_EXTRACT_PATH}")
        message(STATUS "Downloading SDK to ${SDK_EXTRACT_PATH}...")

        file(MAKE_DIRECTORY "${VEX_GLOBAL_DIR}")

        file(DOWNLOAD "${SDK_URL}" "${SDK_DOWNLOAD_FILE}"
             SHOW_PROGRESS
             STATUS DOWNLOAD_STATUS)

        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            list(GET DOWNLOAD_STATUS 1 ERROR_MSG)
            message(FATAL_ERROR "Failed to download SDK: ${ERROR_MSG}")
        endif()

        message(STATUS "Extracting SDK...")
        file(ARCHIVE_EXTRACT INPUT "${SDK_DOWNLOAD_FILE}" DESTINATION "${VEX_GLOBAL_DIR}")

        # set execute permissions on unix like systems
        if(UNIX)
            execute_process(
                COMMAND chmod -R +x "${SDK_EXTRACT_PATH}"
                RESULT_VARIABLE CHMOD_RESULT
                OUTPUT_QUIET
                ERROR_QUIET
            )
            if(NOT CHMOD_RESULT EQUAL 0)
                message(WARNING "Failed to set execute permissions on ${SDK_EXTRACT_PATH}")
            endif()
        endif()

        # clean up download file
        file(REMOVE "${SDK_DOWNLOAD_FILE}")

        file(DOWNLOAD "https://gist.githubusercontent.com/PascalSkylake/fa58c0fae422cf36b11b5ceedf922db0/raw/aec5cb9c4b1088dc7a6553b001ff6d79d268e692/lscript1.ld" "${SDK_EXTRACT_PATH}/lscript1.ld")

        message(STATUS "SDK installed to ${SDK_EXTRACT_PATH}")
    else()
        message(STATUS "SDK already exists at ${SDK_EXTRACT_PATH}")
    endif()

    if(EXISTS "${SDK_EXTRACT_PATH}")
        set(VEX_SDK_PATH "${SDK_EXTRACT_PATH}")
        message(STATUS "Successfully installed SDK to global directory")
    else()
        message(FATAL_ERROR "SDK installation failed")
    endif()
endif()

# download toolchain if not found
if(NOT VEX_TOOLCHAIN_PATH OR NOT EXISTS "${VEX_TOOLCHAIN_PATH}")
    message(STATUS "VEX Toolchain not found, will download automatically...")

    # use the right URL for the platform
    if(WINDOWS)
        set(TOOLCHAIN_URL "https://github.com/RIT-VEX-U/ForkTemplate/releases/download/1.0/ATfE-20.1.0-Windows-x86_64.zip")
        set(TOOLCHAIN_SUBDIR "ATfE-20.1.0-Windows-x86_64")
      elseif(MACOS)
        set(TOOLCHAIN_URL "https://github.com/RIT-VEX-U/ForkTemplate/releases/download/1.0/ATfE-20.1.0-Darwin-universal.zip")
        set(TOOLCHAIN_SUBDIR "ATfE-20.1.0-Darwin-universal")
      elseif(AARCH64)
        set(TOOLCHAIN_URL "https://github.com/RIT-VEX-U/ForkTemplate/releases/download/1.0/ATfE-20.1.0-Linux-AArch64.tar.xz")
        set(TOOLCHAIN_SUBDIR "ATfE-20.1.0-Linux-AArch64")
      else()
        set(TOOLCHAIN_URL "https://github.com/RIT-VEX-U/ForkTemplate/releases/download/1.0/ATfE-20.1.0-Linux-x86_64.tar.xz")
        set(TOOLCHAIN_SUBDIR "ATfE-20.1.0-Linux-x86_64")
    endif()


    set(TOOLCHAIN_EXTRACT_PATH "${VEX_GLOBAL_DIR}/${TOOLCHAIN_SUBDIR}")
    set(TOOLCHAIN_DOWNLOAD_FILE "${VEX_GLOBAL_DIR}/vex-toolchain.zip")

    if(NOT EXISTS "${TOOLCHAIN_EXTRACT_PATH}")
        message(STATUS "Downloading toolchain to ${TOOLCHAIN_EXTRACT_PATH}...")

        file(MAKE_DIRECTORY "${VEX_GLOBAL_DIR}")

        file(DOWNLOAD "${TOOLCHAIN_URL}" "${TOOLCHAIN_DOWNLOAD_FILE}"
             SHOW_PROGRESS
             STATUS DOWNLOAD_STATUS)

        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            list(GET DOWNLOAD_STATUS 1 ERROR_MSG)
            message(FATAL_ERROR "Failed to download toolchain: ${ERROR_MSG}")
        endif()

        message(STATUS "Extracting toolchain...")
        file(ARCHIVE_EXTRACT INPUT "${TOOLCHAIN_DOWNLOAD_FILE}" DESTINATION "${VEX_GLOBAL_DIR}")

        # set execute permissions on unix like systems
        if(UNIX)
            execute_process(
                COMMAND chmod -R +x "${TOOLCHAIN_EXTRACT_PATH}"
                RESULT_VARIABLE CHMOD_RESULT
                OUTPUT_QUIET
                ERROR_QUIET
            )
            if(NOT CHMOD_RESULT EQUAL 0)
                message(WARNING "Failed to set execute permissions on ${TOOLCHAIN_EXTRACT_PATH}")
            endif()
        endif()

        # clean up downloaded files
        file(REMOVE "${TOOLCHAIN_DOWNLOAD_FILE}")

        set(VEX_VEXCOM_PATH ${TOOLCHAIN_EXTRACT_PATH}/tools/vexcom)


        message(STATUS "Toolchain installed to ${TOOLCHAIN_EXTRACT_PATH}")
    else()
        message(STATUS "Toolchain already exists at ${TOOLCHAIN_EXTRACT_PATH}")
    endif()

    if(EXISTS "${TOOLCHAIN_EXTRACT_PATH}")
        set(VEX_TOOLCHAIN_PATH "${TOOLCHAIN_EXTRACT_PATH}")
        set(VEX_VEXCOM_PATH "${VEX_TOOLCHAIN_PATH}/tools/vexcom")
        message(STATUS "Successfully installed Toolchain to global directory")
    else()
        message(FATAL_ERROR "Toolchain installation failed")
    endif()
endif()

# finally check if everything exists
if(NOT VEX_SDK_PATH OR NOT EXISTS "${VEX_SDK_PATH}")
    message(FATAL_ERROR
        "VEX SDK not found and download failed.\n"
        "Manually set VEX_SDK_PATH or check your internet connection.\n"
        "Current VEX_SDK_PATH: ${VEX_SDK_PATH}")
endif()

if(NOT VEX_TOOLCHAIN_PATH OR NOT EXISTS "${VEX_TOOLCHAIN_PATH}")
    message(FATAL_ERROR
        "VEX Toolchain not found and download failed.\n"
        "Manually set VEX_TOOLCHAIN_PATH or check your internet connection.\n"
        "Current VEX_TOOLCHAIN_PATH: ${VEX_TOOLCHAIN_PATH}")
endif()

# set(CMAKE_MAKE_PROGRAM              ${VEX_TOOLCHAIN_PATH}/tools/ninja/ninja${EXE_SUFFIX})
if(WINDOWS)
    set(EXE_SUFFIX ".exe")
else()
    set(EXE_SUFFIX "")
endif()

set(VEX_COMPILER_PATH "${VEX_TOOLCHAIN_PATH}/bin")

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_MAKE_PROGRAM              ${VEX_TOOLCHAIN_PATH}/tools/ninja/ninja${EXE_SUFFIX} CACHE FILEPATH "${VEX_TOOLCHAIN_PATH}/tools/ninja/ninja${EXE_SUFFIX}" FORCE)
set(CMAKE_AR                        ${VEX_COMPILER_PATH}/llvm-ar${EXE_SUFFIX})
set(CMAKE_C_COMPILER                ${VEX_COMPILER_PATH}/clang${EXE_SUFFIX})
set(CMAKE_CXX_COMPILER              ${VEX_COMPILER_PATH}/clang${EXE_SUFFIX})
set(CMAKE_C_COMPILER_ID             Clang)
set(CMAKE_CXX_COMPILER_ID           Clang)
set(CMAKE_LINKER                    ${VEX_COMPILER_PATH}/ld.lld${EXE_SUFFIX})
set(CMAKE_OBJCOPY                   ${VEX_COMPILER_PATH}/llvm-objcopy${EXE_SUFFIX})
set(CMAKE_OBJDUMP                   ${VEX_COMPILER_PATH}/llvm-objdump${EXE_SUFFIX})
set(CMAKE_SIZE                      ${VEX_COMPILER_PATH}/llvm-size${EXE_SUFFIX})

# use the vex linker script
set(CMAKE_C_LINK_EXECUTABLE  "<CMAKE_LINKER> -z norelro -T \"${VEX_SDK_PATH}/lscript1.ld\"  --gc-sections -L\"${VEX_TOOLCHAIN_PATH}/lib/clang-runtimes/newlib/arm-none-eabi/armv7a_soft_vfpv3_d16_unaligned/lib\" <OBJECTS> -o <TARGET> --start-group -lc++ -lc -lm -lclang_rt.builtins --end-group")
set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

add_compile_options(-DVexV5)

set(CFLAGS_CL "-target thumbv7-none-eabi -fshort-enums -Wno-unknown-attributes --config=OmaxLTO.cfg  -U__ARM_NEON__ -U__ARM_NEON")
set(CFLAGS_V7 "-march=armv7-a -mfpu=neon -mfloat-abi=softfp")

if(VEX_QUIET_BUILD)
    set(WARNING_FLAGS "-w")  # suppress all warnings
else()
    set(WARNING_FLAGS "-Wall -Werror=return-type")  # enable all warnings
endif()

set(CMAKE_C_FLAGS                   "${CFLAGS_CL} ${CFLAGS_V7} -Os ${WARNING_FLAGS} -ansi -std=c23")
set(CMAKE_CXX_FLAGS                 "${CFLAGS_CL} ${CFLAGS_V7} -Os ${WARNING_FLAGS} -fno-rtti -fno-threadsafe-statics -fno-exceptions  -std=c++23 -ffunction-sections -fdata-sections" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

include_directories(SYSTEM "${VEX_TOOLCHAIN_PATH}/lib/clang-runtimes/newlib/arm-none-eabi/armv7a_soft_vfpv3_d16_unaligned/include/c++/v1")
include_directories(SYSTEM "${VEX_TOOLCHAIN_PATH}/lib/clang-runtimes/newlib/arm-none-eabi/armv7a_soft_vfpv3_d16_unaligned/include")
include_directories(SYSTEM "${VEX_TOOLCHAIN_PATH}/lib/clang/20/include")

# include_directories(SYSTEM "${VEX_SDK_PATH}/include")

function(vex_add_executable target_name)
    add_executable(${target_name})

    set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${target_name}.elf")

    target_precompile_headers(${target_name} PUBLIC ${vex_headers})

    # strip elf into binary
    add_custom_command(
      TARGET ${target_name}
      POST_BUILD
      COMMAND "${CMAKE_OBJCOPY}"
      ARGS -O binary $<TARGET_FILE:${target_name}> $<TARGET_FILE_DIR:${target_name}>/${target_name}.bin
      )

    # show size of program
    add_custom_command(
      TARGET ${target_name}
      POST_BUILD
      COMMAND "${CMAKE_SIZE}"
      ARGS $<TARGET_FILE:${target_name}>
      )

endfunction()
