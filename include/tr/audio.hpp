///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio API based on OpenAL.                                                                                                //
//                                                                                                                                       //
// Using this module requires the option TR_BUILD_AUDIO to be turned on in CMake. TR_BUILD_AUDIO, in turn, depends on TR_BUILD_SYSGFX.   //
//                                                                                                                                       //
// Nothing from this module is allowed to be called in main::parse_command_line.                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "audio/buffer.hpp"   // IWYU pragma: export
#include "audio/listener.hpp" // IWYU pragma: export
#include "audio/source.hpp"   // IWYU pragma: export
#include "audio/stream.hpp"   // IWYU pragma: export