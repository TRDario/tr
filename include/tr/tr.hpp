/// @file
/// @brief Includes all available tr headers.

#pragma once
#include <tr/utility.hpp> // IWYU pragma: export
#ifdef TR_HAS_SYSGFX
#include <tr/sysgfx.hpp> // IWYU pragma: export
#endif
#ifdef TR_HAS_AUDIO
#include <tr/audio.hpp> // IWYU pragma: export
#endif
#ifdef TR_HAS_IMGUI
#include <tr/imgui.hpp> // IWYU pragma: export
#endif