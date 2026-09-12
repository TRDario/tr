/// @file
/// @brief Includes all headers from the audio module.
/// @details
/// The audio module provides an audio API based on OpenAL.
///
/// Using this module requires the option TR_BUILD_AUDIO to be turned on in CMake. TR_BUILD_AUDIO, in turn, depends on TR_BUILD_SYSGFX.

#pragma once
#include <tr/audio/audio_buffer.hpp>           // IWYU pragma: export
#include <tr/audio/audio_class.hpp>            // IWYU pragma: export
#include <tr/audio/audio_context.hpp>          // IWYU pragma: export
#include <tr/audio/audio_device.hpp>           // IWYU pragma: export
#include <tr/audio/audio_device_list_view.hpp> // IWYU pragma: export
#include <tr/audio/audio_source.hpp>           // IWYU pragma: export
#include <tr/audio/audio_stream.hpp>           // IWYU pragma: export
#include <tr/audio/exception.hpp>              // IWYU pragma: export
#include <tr/audio/orientation.hpp>            // IWYU pragma: export