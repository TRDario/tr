# Configuration and Building

## Tooling

tr requires a C++23-compatible compiler and standard library, as well as CMake 3.23 to build.

tr has dependencies on a number of third-party libraries. By default, these libraries are downloaded, configured, and built alongside tr
automatically using FetchContent. If you wish to reduce configuration/build times and use system libraries instead,
set `TR_USE_SYSTEM_LIBRARIES` to `ON` during CMake configuration.

It is recommended to use `tr_target_template(YOUR_TARGET)` in your CMakeLists.txt file to automatically configure a target with tr-friendly
settings. The command takes care of several compile options, compile definitions, and linking to tr.
See [the page on CMake](cmake.md) for more information on available definitions and functions.

@note
The library has been tested on and guaranteed to build with Clang(-cl) on Linux and Windows.
It is likely to work with MSVC and GCC too, but these compilers have not been tested extensively and are currently not a priority.

## Modules

tr is divided into several distinct modules; which modules are built may be controlled when configuring CMake.

### Utility module

The utility module (CMake target: `tr::utility`) contains miscellaneous helper utilities used by the rest of the library.
This includes concepts and template metaprogramming utilities, macros, containers and datatypes, math and geometry utilities,
various helper functions augmenting the standard library, and more.

This module is always built and cannot be disabled.

This module introduces dependencies on [glm](https://github.com/g-truc/glm), [lz4](https://github.com/lz4/lz4),
and [boost_unordered](https://github.com/MikePopoloski/boost_unordered).

### Audio module

The audio module (CMake target: `tr::audio`) contains audio subsystem functionality built upon OpenAL. This includes support for audio
buffers and streams, as well as 3D audio sources.

This module is enabled by setting `TR_BUILD_AUDIO` to `ON` during CMake configuration.

This module introduces dependencies on [Ogg](https://github.com/xiph/ogg), [Vorbis](https://github.com/xiph/vorbis),
and [OpenAL-Soft](https://github.com/kcat/openal-soft).

### System and graphics module

The system and graphics module (CMake target: `tr::sysgfx`) contains windowing, input, and other system functionality built upon SDL3,
as well as graphical functionality built upon OpenGL.
This includes keyboard, clipboard, mouse, and other event polling support, bitmap handling, window and native dialog support,
various rendering primitives as well as some higher-level graphical objects and several ready-made renderers.
Applications using this module are defined with special entry points in the `tr_main` namespace.

This module is enabled by setting `TR_BUILD_SYSGFX` to `ON` during CMake configuration.

This module introduces dependencies on [SDL3](https://github.com/libsdl-org/SDL), [SDL_ttf](https://github.com/libsdl-org/SDL_ttf),
and [SDL_image](https://github.com/libsdl-org/SDL_image).

### ImGui integration module

The ImGui integration module (CMake target: `tr::imgui`) contains functions that allow integration of Dear ImGui with tr applications.

This module is enabled by setting `TR_BUILD_IMGUI` to `ON` during CMake configuration.

This module introduces a dependency on [Dear ImGui](https://github.com/ocornut/imgui), as well as the tr system and graphics module,
which it will automatically enable if it was not previously enabled.

## Compile definitions

tr allows defining several compile macro definitions to control the behavior of the library.

Defining `TR_ENABLE_ASSERTS` enables assertions and precondition checks in the library.
When using `tr_target_template`, it is defined by default in builds with debug info (Debug and RelWithDebInfo).

Defining `TR_ENABLE_CHECKED_GRAPHICS` enables extensive validity checking of graphical primitives and operations.
When using `tr_target_template`, it is defined by default in debug builds (but not RelWithDebInfo).