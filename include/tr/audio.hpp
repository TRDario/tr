///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio API based on OpenAL.                                                                                                //
//                                                                                                                                       //
// Using this module requires the option TR_BUILD_AUDIO to be turned on in CMake. TR_BUILD_AUDIO, in turn, depends on TR_BUILD_SYSGFX.   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "audio/audio_buffer.hpp"           // IWYU pragma: export
#include "audio/audio_class.hpp"            // IWYU pragma: export
#include "audio/audio_context.hpp"          // IWYU pragma: export
#include "audio/audio_device.hpp"           // IWYU pragma: export
#include "audio/audio_device_list_view.hpp" // IWYU pragma: export
#include "audio/audio_source.hpp"           // IWYU pragma: export
#include "audio/audio_stream.hpp"           // IWYU pragma: export