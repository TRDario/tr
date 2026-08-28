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

For more details, see [this page from the documentation](https://trdario.github.io/tr/md_pages_2configuration__and__building.html).

## Documentation

Documentation for tr can be accessed [here](https://trdario.github.io/tr/) or built yourself using Doxygen with the doxyfile provided in /doxygen.

Examples and more detailed documentation are planned to be added at a later date.

## Testing

tr is currently in the process of being covered with unit tests.

![Unit test coverage progress bar](https://progress-bar.xyz/39?title=Unit+test+coverage:&width=100&scale=100&suffix=%2100+files)

## Licence

All assets and code are under the [Apache License 2.0](https://github.com/TRDario/tr/blob/master/LICENSE.md).
