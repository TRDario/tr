/// @file
/// @brief Provides `tr::internal::convert_sdl_keymods()`.

#pragma once
#include <SDL3/SDL.h>
#include <tr/sysgfx/keyboard.hpp>

namespace tr::internal
{
	/// Converts SDL keymods to tr keymods.
	/// @param mods SDL keymods.
	/// @return Equivalent tr keymods.
	[[nodiscard]] constexpr keymod convert_sdl_keymods(SDL_Keymod mods) noexcept
	{
		if (mods & SDL_KMOD_SHIFT) {
			mods |= SDL_KMOD_SHIFT;
		}
		if (mods & SDL_KMOD_CTRL) {
			mods |= SDL_KMOD_CTRL;
		}
		if (mods & SDL_KMOD_ALT) {
			mods |= SDL_KMOD_ALT;
		}
		mods &= (SDL_KMOD_SHIFT | SDL_KMOD_CTRL | SDL_KMOD_ALT);
		return static_cast<keymod>(mods);
	}
} // namespace tr::internal