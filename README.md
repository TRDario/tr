# tr

C++20 game development framework based on SDL3, OpenGL 4.5, and OpenAL.

As of now primarily geared towards 2D games and tools, but is in theory usable for 3D as well.

> [!WARNING]
> This is primarily a personal project, fine-tuned to my needs and preferences.
>
> There may be bugs, API stability is not guaranteed, and any new additions are driven by what I need in the moment.
>
> Use at your own risk.

## Build Instructions

Requires a C++20-compatible compiler and standard library (though a fallback for std::format exists), as well as CMake 3.23 to build.

Tested on and guaranteed to build with Clang(-cl) on Linux and Windows. Probably works with MSVC and GCC too, but I haven't tested them.

For available CMake options, see [CMakeLists.txt](https://github.com/TRDario/tr/blob/master/CMakeLists.txt).

## Documentation

The source code provides basic documentation for all types, functions, constants and globals in the library.

An overview of provided functionality and usage examples are provided at the tops of header files.

## Licence

All assets and code are under the [Apache License 2.0](https://github.com/TRDario/tr/blob/master/LICENSE.md).
