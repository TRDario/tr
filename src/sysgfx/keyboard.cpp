/// @file
/// @brief Implements keyboard.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/keyboard.hpp>

//

std::string tr::name(keycode key)
{
	return SDL_GetKeyName(std::to_underlying(key));
}

tr::keycode tr::internal::to_keycode(zstring_view str) noexcept
{
	return static_cast<keycode>(SDL_GetKeyFromName(str.c_str()));
}

//

std::string tr::key_chord::name() const
{
	std::string str;
	if (mods & keymod::ctrl) {
		str.append("Ctrl+");
	}
	if (mods & keymod::alt) {
		str.append("Alt+");
	}
	if (mods & keymod::shift) {
		str.append("Shift+");
	}
	str.append(tr::name(key));
	return str;
}

//

void tr::binary_reader<tr::scan_chord>::operator()(std::istream& is, tr::scan_chord& out) const
{
	read_binary(is, out.mods, out.scan);
}

void tr::binary_writer<tr::scan_chord>::operator()(std::ostream& os, const tr::scan_chord& in) const
{
	write_binary(os, in.mods, in.scan);
}

void tr::binary_reader<tr::key_chord>::operator()(std::istream& is, tr::key_chord& out) const
{
	read_binary(is, out.mods, out.key);
}

void tr::binary_writer<tr::key_chord>::operator()(std::ostream& os, const tr::key_chord& in) const
{
	write_binary(os, in.mods, in.key);
}