# tr

C++23 game development framework based on SDL3, OpenGL 4.5, and OpenAL.

> [!WARNING]
> This is primarily a personal project, fine-tuned to my needs and preferences.
>
> There may be bugs, API stability is not guaranteed, and any new additions are driven by what I need in the moment.
>
> Use at your own risk.

## Build Instructions

Requires a C++23-compatible compiler and standard library, as well as CMake 3.23 to build.

Tested on and guaranteed to build with Clang/libstdc++ on Linux and clang-cl/MSVC STL on Windows. Other combinations of compilers and standard libraries are untested as of now.

For more details, see [this page from the documentation](https://trdario.github.io/tr/md_pages_2building.html).

## Documentation

Documentation for tr can be accessed [here](https://trdario.github.io/tr/) or built yourself using Doxygen with the doxyfile provided in /doxygen.

Examples are planned to be added at a later date.

## Testing

tr is partially covered with unit tests:
 - utility: Fully covered
 - audio: Almost entirely covered, pending revisions
 - sysgfx: Almost entirely uncovered

Further unit tests are planned to be added at a later date.

## Licence

All assets and code are under the [Apache License 2.0](https://github.com/TRDario/tr/blob/main/LICENSE.md).