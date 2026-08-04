/// @file
/// @brief Includes all available tr headers.

#pragma once
#include "utility.hpp" // IWYU pragma: export
#ifdef TR_HAS_SYSGFX
#include "sysgfx.hpp" // IWYU pragma: export
#endif
#ifdef TR_HAS_AUDIO
#include "audio.hpp" // IWYU pragma: export
#endif
#ifdef TR_HAS_IMGUI
#include "imgui.hpp" // IWYU pragma: export
#endif