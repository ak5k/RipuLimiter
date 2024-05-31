### JUCE CMake Audio Plugin Template ###

cmake_minimum_required(VERSION 3.22)

project(RipuLimiter VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-D_SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)

### Dependency versions ###

set(JUCE_TAG "7.0.12")
set(SIGNALSMITH_DSP_TAG "v1.6.0")

if(DEFINED ENV{GITHUB_ACTIONS})
  set(JUCE_TAG $ENV{JUCE_TAG})
  set(SIGNALSMITH_DSP_TAG $ENV{SIGNALSMITH_DSP_TAG})
  message("Running in GitHub Actions")
else()
  if(WIN32)
    include("c:/vcpkg/scripts/buildsystems/vcpkg.cmake")
  else()
    include("~/vcpkg/scripts/buildsystems/vcpkg.cmake")
  endif()
endif()

find_package(Git REQUIRED)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --is-inside-work-tree
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE IS_GIT_REPO
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  if(IS_GIT_REPO STREQUAL "true")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_LATEST_TAG
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT "${GIT_LATEST_TAG}" STREQUAL "")
      string(REGEX REPLACE "[^0-9.]" "" GIT_LATEST_TAG ${GIT_LATEST_TAG})
    endif()

    if(NOT "${GIT_LATEST_TAG}" STREQUAL "")
      project(${PROJECT_NAME} VERSION ${GIT_LATEST_TAG} LANGUAGES C CXX)
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT
    )

  else()
    execute_process(
      COMMAND ${GIT_EXECUTABLE} init .
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
  endif()
endif()
message("Project version: ${PROJECT_VERSION}")

if(NOT DEFINED BUILD_NUMBER)
    set(BUILD_NUMBER 0)
endif()

if(NOT DEFINED GIT_TAG)
    set(GIT_TAG 0)
    set(BUILD_NUMBER 0)
endif()

string(TIMESTAMP BUILD_DATE "%Y%m%d")

include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${PROJECT_SOURCE_DIR}/Libs" CACHE PATH "External dependencies path." FORCE)

# 32-bit mac builds require macOS 10.14 Mojave, Xcode 9, and libc++ to be included
if (APPLE AND CMAKE_OSX_ARCHITECTURES MATCHES "i386")
  message("Building for 32-bit macOS")
  set(FILE_URL "https://releases.llvm.org/5.0.2/libcxx-5.0.2.src.tar.xz")
  set(FILE_PATH "${FETCHCONTENT_BASE_DIR}/libcxx-5.0.2.src.tar.xz")

  file(DOWNLOAD ${FILE_URL} ${FILE_PATH}
      STATUS status
      LOG log)

  execute_process(COMMAND tar -xf ${FILE_PATH} -C ${FETCHCONTENT_BASE_DIR}
                  RESULT_VARIABLE result)
  include_directories(BEFORE SYSTEM "${FETCHCONTENT_BASE_DIR}/libcxx-5.0.2.src/include")
endif()

FetchContent_Declare(juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        ${JUCE_TAG}
    GIT_SHALLOW    TRUE
    GIT_CONFIG     advice.detachedHead=false    # Disable detached HEAD warning for fetching a specific tag
)
FetchContent_GetProperties(juce)
if(NOT juce_POPULATED)
  FetchContent_Populate(juce)
endif()
add_subdirectory(${juce_SOURCE_DIR})

FetchContent_Declare(
  signalsmith-dsp
  GIT_REPOSITORY https://github.com/Signalsmith-Audio/dsp
  GIT_TAG        ${SIGNALSMITH_DSP_TAG}
  GIT_SHALLOW    TRUE
  GIT_CONFIG     advice.detachedHead=false    # Disable detached HEAD warning for fetching a specific tag
  SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/signalsmith/dsp"
)
FetchContent_Populate(signalsmith-dsp)

juce_add_plugin("${PROJECT_NAME}"
    FORMATS AU VST3                             # The formats to build. Valid formats: Standalone Unity VST3 AU AUv3 AAX VST LV2.
                                                # AU and AUv3 plugins will only be enabled when building on macOS.
    PRODUCT_NAME "${PROJECT_NAME}"              # The name of the final executable, which can differ from the target name.
    PLUGIN_NAME  "${PROJECT_NAME}"              # Name of the plugin that will be displayed in the DAW. Can differ from PRODUCT_NAME.

    # ICON_BIG                                  # ICON_* arguments specify a path to an image file to use as an icon for the Standalone.
    # ICON_SMALL
    COMPANY_NAME ak5k                           # The name of this target's author. The value is inherited from JUCE_COMPANY_NAME.
    # COMPANY_WEBSITE                           # The address of a website related to this target in some way.
                                                # The value is inherited from JUCE_COMPANY_WEBSITE.
    # COMPANY_EMAIL                             # An email address for this target's author. The value is inherited from JUCE_COMPANY_EMAIL.
    PLUGIN_MANUFACTURER_CODE Ak5k               # A four-character manufacturer id with at least one upper-case character.
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case.
    PLUGIN_CODE R1pu                            # A unique four-character plugin id with exactly one upper-case character.
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case.

    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # Does the editor need keyboard focus?

    VST3_CATEGORIES Dynamics                    # Should be one or more, separated by spaces, of the following:
                                                # Fx, Instrument, Analyzer, Delay, Distortion, Drum, Dynamics, EQ, External,
                                                # Filter, Generator, Mastering, Modulation, Mono, Network, NoOfflineProcess,
                                                # OnlyOfflineProcess, OnlyRT, Pitch Shift, Restoration, Reverb, Sampler, Spatial,
                                                # Stereo, Surround, Synth, Tools, Up-Downmix
    # AU_MAIN_TYPE                              # Should be one or more, separated by spaces, of the following:
                                                # kAudioUnitType_Effect, kAudioUnitType_FormatConverter, kAudioUnitType_Generator,
                                                # kAudioUnitType_MIDIProcessor, kAudioUnitType_Mixer, kAudioUnitType_MusicDevice,
                                                # kAudioUnitType_MusicEffect, kAudioUnitType_OfflineEffect, kAudioUnitType_Output,
                                                # kAudioUnitType_Panner
    AU_COPY_DIR "/Library/Audio/Plug-Ins/Components" # The subdirectory to copy the AU plugin into (macOS only)

    COPY_PLUGIN_AFTER_BUILD TRUE)               # Should the plugin be installed to a default location after building?
                                                # NOTE: Writing to the default install locations might require administrator privileges on Windows.

juce_generate_juce_header(${PROJECT_NAME})

target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
        # Assets                # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_opengl
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ./Source/*.c
    ./Source/*.cpp
    ./Source/*.h
    ./Source/*.hpp
   )

target_sources(${PROJECT_NAME}
    PRIVATE
        "${SOURCES}"
)

if(MSVC)
    # set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS "/W4")
else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" LOWERCASE_PROCESSOR)
    if (NOT APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      target_compile_options(${PROJECT_NAME} PUBLIC -march=x86-64-v3)
    endif()
    message("non-msvc compiler interface")
    set(MY_FLAGS 
        -Wall 
        -Wextra 
        -Wpedantic
        #
        -Wno-float-conversion
        -Wno-shadow 
        -Wno-shorten-64-to-32
        -Wno-sign-compare
        -Wno-sign-conversion
        -Wno-unused-parameter
        -Wno-unused-private-field
        -Wno-unused-variable
        )
    string(JOIN " " MY_FLAGS_STR ${MY_FLAGS})
    set_source_files_properties(${SOURCES} 
        PROPERTIES COMPILE_FLAGS 
          "${MY_FLAGS_STR}"
    )
endif()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${FETCHCONTENT_BASE_DIR}/signalsmith/*.h*"
   )

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  message("precompiling headers")
  target_precompile_headers(${PROJECT_NAME} PRIVATE "${HEADERS}")
endif()

target_include_directories(${PROJECT_NAME}
    SYSTEM PRIVATE
        "${FETCHCONTENT_BASE_DIR}"
)

message("")