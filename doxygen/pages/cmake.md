# CMake Library

tr provides several definitions and functions you may use in your project:

## Definitions

### `TR_HAS_GCC`, `TR_HAS_CLANG`, `TR_HAS_CLANG_CL`, `TR_HAS_MSVC`

Strict compiler detection definitions (only one of these will ever be defined).

### `TR_DO_NOTHING`

Defines a command that does nothing when invoked. This is useful in generator expressions when running a command isn't always necessary.

## Functions

### `tr_generate_embeddable_string(TARGET, INPUT_FILE, OUTPUT_FILE, VARIABLE_NAME)`

Processes text from `INPUT_FILE` into an embeddable string named `VARIABLE_NAME` and generates a C++ header holding it at `OUTPUT_FILE`.

Parameters:
 - `TARGET` - CMake target that depends on the generated header.
 - `INPUT_FILE` - Input text file path relative to `${CMAKE_CURRENT_SOURCE_DIR}`.
 - `OUTPUT_FILE` - Output header file path relative to `${CMAKE_CURRENT_BINARY_DIR}/include/generated`.
 - `VARIABLE_NAME` - Name of the string constant within the header.

### `tr_generate_embeddable_binary(TARGET, INPUT_FILE, OUTPUT_FILE, VARIABLE_NAME)`

Processes binary data from `INPUT_FILE` into an embeddable data array named `VARIABLE_NAME` and generates a C++ header holding it at `OUTPUT_FILE`.

Parameters:
 - `TARGET` - CMake target that depends on the generated header.
 - `INPUT_FILE` - Input binary file path relative to `${CMAKE_CURRENT_SOURCE_DIR}`.
 - `OUTPUT_FILE` - Output header file path relative to `${CMAKE_CURRENT_BINARY_DIR}/include/generated`.
 - `VARIABLE_NAME` - Name of the data array constant within the header.

### `tr_git_hash_setup()`

Generates a header which defines the `GIT_HASH` macro as a string holding the hash of the current git commit.
This header can be included with `#include <git_hash.hpp>` from within the project that called this function.

### `tr_target_template(TARGET)`

Configures a CMake C++ target with tr-friendly settings. The command takes care of several compile options, compile definitions, and linking to tr.

Parameters:
 - `TARGET` - CMake target to apply the tr template on.

