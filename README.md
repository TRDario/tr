# tr

C++23 game development framework based on SDL3, OpenGL 4.5, and OpenAL.

As of now primarily geared towards 2D games and tools, but is in theory usable for 3D as well.

> [!WARNING]
> This is primarily a personal project, fine-tuned to my needs and preferences.
>
> There may be bugs, API stability is not guaranteed, and any new additions are driven by what I need in the moment.
>
> Use at your own risk.

## Build Instructions

Requires a C++23-compatible compiler and standard library, as well as CMake 3.23 to build.

Tested on and guaranteed to build with Clang(-cl) on Linux and Windows. Probably works with MSVC and GCC too, but I haven't tested them.

For available CMake options, see [CMakeLists.txt](https://github.com/TRDario/tr/blob/master/CMakeLists.txt).

## Documentation

tr is currently in the process of moving to using doxygen to document the code.

![Doxygen coverage progress bar](https://progress-bar.xyz/89?title=Doxygen+coverage:&width=100&scale=98&suffix=%2F96+files)

For all unmigrated files, the source code provides basic documentation for all types, functions, constants and globals in the library,
and an overview of provided functionality and usage examples are provided at the tops of header files.

## Testing

tr is currently in the process of being covered with unit tests.

![Unit test coverage progress bar](https://progress-bar.xyz/39?title=Unit+test+coverage:&width=100&scale=91&suffix=%2F91+files)

## Licence

All assets and code are under the [Apache License 2.0](https://github.com/TRDario/tr/blob/master/LICENSE.md).
