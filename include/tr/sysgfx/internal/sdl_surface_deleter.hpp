/// @file
/// @brief Provides `tr::internal::sdl_surface_deleter`.

#pragma once

struct SDL_Surface;

//

namespace tr::internal
{
	/// @brief SDL surface deleter.
	struct sdl_surface_deleter
	{
		/// @brief Destroys an SDL surface.
		/// @param ptr Pointer to the surface.
		static void operator()(SDL_Surface* ptr) noexcept;
	};
} // namespace tr::internal