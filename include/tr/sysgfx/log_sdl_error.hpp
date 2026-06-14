///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides the TR_LOG_SDL_ERROR macro.                                                                                                  //
//                                                                                                                                       //
// TR_LOG_SDL_ERROR(...) logs a formatted error message to tr::log, appending the error from SDL on a newline afterwards:                //
//     - TR_LOG_SDL_ERROR("Example")                                                                                                     //
//       -> [21:14:01] [tr] [E] Example                                                                                                  //
//                          --- SDL error message here                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/logger.hpp"
#include <SDL3/SDL.h>

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

// Logs a formatted SDL error to tr::log.
#define TR_LOG_SDL_ERROR(...)                                                                                                              \
	do {                                                                                                                                   \
		TR_LOG(::tr::log, ::tr::severity::error, __VA_ARGS__);                                                                             \
		TR_LOG_CONTINUE(::tr::log, "{}", SDL_GetError());                                                                                  \
	} while (0)