/// @file
/// @brief Implements the constexpr parts of keyboard.hpp.

#pragma once
#include "../../utility/macro.hpp"
#include "../keyboard.hpp"

//

/// Scancode hasher.
template <>
struct boost::hash<tr::scancode>
{
	/// Hashes a scancode.
	/// @param code Scancode to hash.
	/// @return Scancode hash.
	std::size_t operator()(tr::scancode code) const;
};

/// Keycode hasher.
template <>
struct boost::hash<tr::keycode>
{
	/// Hashes a keycode.
	/// @param code Keycode to hash.
	/// @return Keycode hash.
	std::size_t operator()(tr::keycode code) const;
};

/// Scan chord hasher.
template <>
struct boost::hash<tr::scan_chord>
{
	/// Hashes a scan chord.
	/// @param chord Chord to hash.
	/// @return Scan chord hash.
	std::size_t operator()(tr::scan_chord chord) const;
};

/// Key chord hasher.
template <>
struct boost::hash<tr::key_chord>
{
	/// Hashes a key chord.
	/// @param chord Chord to hash.
	/// @return Key chord hash.
	std::size_t operator()(tr::key_chord chord) const;
};

//

/// Scancode formatter.
template <>
struct std::formatter<tr::scancode> : public std::formatter<const char*>
{
	/// Formats a scancode.
	/// @tparam FormatContext Formatting context type.
	/// @param scan Scancode to format.
	/// @param ctx Formatting context.
	template <typename FormatContext>
	constexpr auto format(tr::scancode scan, FormatContext& ctx) const
	{
		ctx.advance_to(std::formatter<const char*>::format(name(scan), ctx));
		return ctx.out();
	}
};

// Scan chord formatter.
template <>
struct std::formatter<tr::scan_chord> : public std::formatter<std::string>
{
	/// Formats a scan chord.
	/// @tparam FormatContext Formatting context type.
	/// @param chord Scan chord to format.
	/// @param ctx Formatting context.
	template <typename FormatContext>
	constexpr auto format(tr::scan_chord chord, FormatContext& ctx) const
	{
		ctx.advance_to(std::formatter<std::string>::format(chord.name(), ctx));
		return ctx.out();
	}
};

// Keycode formatter.
template <>
struct std::formatter<tr::keycode> : public std::formatter<std::string>
{
	/// Formats a keycode.
	/// @tparam FormatContext Formatting context type.
	/// @param scan Keycode to format.
	/// @param ctx Formatting context.
	template <typename FormatContext>
	constexpr auto format(tr::keycode key, FormatContext& ctx) const
	{
		ctx.advance_to(std::formatter<std::string>::format(name(key), ctx));
		return ctx.out();
	}
};

// Key chord formatter.
template <>
struct std::formatter<tr::key_chord> : public std::formatter<std::string>
{
	/// Formats a key chord.
	/// @tparam FormatContext Formatting context type.
	/// @param scan Key chord to format.
	/// @param ctx Formatting context.
	template <typename FormatContext>
	constexpr auto format(tr::key_chord chord, FormatContext& ctx) const
	{
		ctx.advance_to(std::formatter<std::string>::format(chord.name(), ctx));
		return ctx.out();
	}
};

//

/// Enables default binary IO for scancodes.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::scancode>{true};

/// Enables default binary IO for keycodes.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::keycode>{true};

//

/// Scan chord binary reader.
template <>
struct tr::binary_reader<tr::scan_chord>
{
	/// Reads a scan chord from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::istream& is, tr::scan_chord& out) const;
};

/// Scan chord binary writer.
template <>
struct tr::binary_writer<tr::scan_chord>
{
	/// Reads a scan chord from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::ostream& os, const tr::scan_chord& in) const;
};

/// Key chord binary reader.
template <>
struct tr::binary_reader<tr::key_chord>
{
	/// Reads a key chord from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::istream& is, tr::key_chord& out) const;
};

/// Key chord binary writer.
template <>
struct tr::binary_writer<tr::key_chord>
{
	/// Reads a key chord from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::ostream& os, const tr::key_chord& in) const;
};

//

namespace tr
{
	/// Copied from SDL with modifications.
	constexpr std::array<zstring_view, 291> scancode_name_table{{
		{},
		{},
		{},
		{},
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"0",
		"Enter",
		"Escape",
		"Backspace",
		"Tab",
		"Space",
		"-",
		"=",
		"[",
		"]",
		"\\",
		"#",
		";",
		"'",
		"`",
		",",
		".",
		"/",
		"CapsLock",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"PrintScreen",
		"ScrollLock",
		"Pause",
		"Insert",
		"Home",
		"PgUp",
		"Delete",
		"End",
		"PgDown",
		"Right",
		"Left",
		"Down",
		"Up",
		"Numlock",
		"Keypad /",
		"Keypad *",
		"Keypad -",
		"Keypad +",
		"Keypad Enter",
		"Keypad 1",
		"Keypad 2",
		"Keypad 3",
		"Keypad 4",
		"Keypad 5",
		"Keypad 6",
		"Keypad 7",
		"Keypad 8",
		"Keypad 9",
		"Keypad 0",
		"Keypad .",
		"NonUSBackslash",
		"Application",
		"Power",
		"Keypad =",
		"F13",
		"F14",
		"F15",
		"F16",
		"F17",
		"F18",
		"F19",
		"F20",
		"F21",
		"F22",
		"F23",
		"F24",
		"Execute",
		"Help",
		"Menu",
		"Select",
		"Stop",
		"Again",
		"Undo",
		"Cut",
		"Copy",
		"Paste",
		"Find",
		"Mute",
		"VolumeUp",
		"VolumeDown",
		{},
		{},
		{},
		"Keypad ,",
		"Keypad = (AS400)",
		"International 1",
		"International 2",
		"International 3",
		"International 4",
		"International 5",
		"International 6",
		"International 7",
		"International 8",
		"International 9",
		"Language 1",
		"Language 2",
		"Language 3",
		"Language 4",
		"Language 5",
		"Language 6",
		"Language 7",
		"Language 8",
		"Language 9",
		"AltErase",
		"SysReq",
		"Cancel",
		"Clear",
		"Prior",
		"Return",
		"Separator",
		"Out",
		"Oper",
		"Clear / Again",
		"CrSel",
		"ExSel",
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		"Keypad 00",
		"Keypad 000",
		"ThousandsSeparator",
		"DecimalSeparator",
		"CurrencyUnit",
		"CurrencySubUnit",
		"Keypad (",
		"Keypad )",
		"Keypad {",
		"Keypad }",
		"Keypad Tab",
		"Keypad Backspace",
		"Keypad A",
		"Keypad B",
		"Keypad C",
		"Keypad D",
		"Keypad E",
		"Keypad F",
		"Keypad XOR",
		"Keypad ^",
		"Keypad %",
		"Keypad <",
		"Keypad >",
		"Keypad &",
		"Keypad &&",
		"Keypad |",
		"Keypad ||",
		"Keypad :",
		"Keypad #",
		"Keypad Space",
		"Keypad @",
		"Keypad !",
		"Keypad MemStore",
		"Keypad MemRecall",
		"Keypad MemClear",
		"Keypad MemAdd",
		"Keypad MemSubtract",
		"Keypad MemMultiply",
		"Keypad MemDivide",
		"Keypad +/-",
		"Keypad Clear",
		"Keypad ClearEntry",
		"Keypad Binary",
		"Keypad Octal",
		"Keypad Decimal",
		"Keypad Hexadecimal",
		{},
		{},
		"LCtrl",
		"LShift",
		"LAlt",
		"LWin",
		"RCtrl",
		"RShift",
		"RAlt",
		"RWin",
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		"ModeSwitch",
		"Sleep",
		"Wake",
		"ChannelUp",
		"ChannelDown",
		"MediaPlay",
		"MediaPause",
		"MediaRecord",
		"MediaFastForward",
		"MediaRewind",
		"MediaTrackNext",
		"MediaTrackPrevious",
		"MediaStop",
		"Eject",
		"MediaPlayPause",
		"MediaSelect",
		"AC New",
		"AC Open",
		"AC Close",
		"AC Exit",
		"AC Save",
		"AC Print",
		"AC Properties",
		"AC Search",
		"AC Home",
		"AC Back",
		"AC Forward",
		"AC Stop",
		"AC Refresh",
		"AC Bookmarks",
		"SoftLeft",
		"SoftRight",
		"Call",
		"EndCall",
	}};

	/// `to_keycode` fallback for Unicode characters.
	/// @param str Keycode string.
	/// @return Keycode associated with the string, or `keycode::unknown`.
	keycode to_keycode_fallback(zstring_view str);
} // namespace tr

//

constexpr tr::scancode tr::to_scancode(std::string_view str)
{
	for (usize i = 0; i < scancode_name_table.size(); ++i) {
		if (!scancode_name_table[i].empty() && scancode_name_table[i] == str) {
			return static_cast<scancode>(i);
		}
	}
	return tr::scancode::unknown;
}

constexpr tr::keycode tr::to_keycode(zstring_view str)
{
	if (str.empty()) {
		return keycode::unknown;
	}

	if (static_cast<u8>(str[0]) < 0x80) {
		if (str[0] >= 'A' && str[0] <= 'Z') {
			return keycode{str[0] - 'A' + 'a'};
		}
		return keycode{str[0]};
	}
	else if (str == "Backspace") {
		return keycode{'\b'};
	}
	else if (str == "Tab") {
		return keycode{'\t'};
	}
	else if (str == "Enter") {
		return keycode{'\r'};
	}
	else if (str == "Escape") {
		return keycode{'\x1B'};
	}
	else if (str == "Delete") {
		return keycode{'\x7F'};
	}

	const scancode scan{to_scancode(str)};
	if (std::to_underlying(scan) >= 57) {
		return keycode{std::to_underlying(scan) | (1 << 30)};
	}

	return to_keycode_fallback(str);
}

constexpr tr::zstring_view tr::name(scancode scan)
{
	return scancode_name_table[std::to_underlying(scan)];
}

//

constexpr tr::scan_chord::scan_chord(scancode scan)
	: scan{scan}
{
}

constexpr tr::scan_chord::scan_chord(keymod mods, scancode scan)
	: mods{mods}
	, scan{scan}
{
}

constexpr tr::scan_chord::scan_chord(std::string_view str)
{
	const auto first_delimiter_pos{str.find('+')};
	if (first_delimiter_pos == str.npos) {
		scan = to_scancode(str);
		return;
	}

	std::string_view substr{str.begin(), str.begin() + first_delimiter_pos};
	while (substr.end() != str.end()) {
		if (substr == "Shift") {
			mods |= keymod::shift;
		}
		else if (substr == "Ctrl") {
			mods |= keymod::ctrl;
		}
		else if (substr == "Alt") {
			mods |= keymod::alt;
		}
		else {
			mods = keymod::unknown;
			return;
		}

		const usize next_start{str.find('+', substr.end() - str.begin() + 1)};
		substr = {substr.end() + 1, next_start == str.npos ? str.end() : str.begin() + next_start};
	}
	scan = to_scancode(substr);
}

constexpr std::string tr::scan_chord::name() const
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
	str.append(tr::name(scan));
	return str;
}

constexpr tr::key_chord::key_chord(keycode key)
	: key{key}
{
}

constexpr tr::key_chord::key_chord(keymod mods, keycode key)
	: mods{mods}
	, key{key}
{
}

constexpr tr::key_chord::key_chord(zstring_view str)
{
	const zstring_view::iterator first_delimiter_pos{std::ranges::find(str, '+')};
	if (first_delimiter_pos == str.end()) {
		key = to_keycode(str);
		return;
	}

	std::string_view substr{str.begin(), first_delimiter_pos};
	while (*substr.end() != '\0') {
		if (substr == "Shift") {
			mods |= keymod::shift;
		}
		else if (substr == "Ctrl") {
			mods |= keymod::ctrl;
		}
		else if (substr == "Alt") {
			mods |= keymod::alt;
		}
		else {
			mods = keymod::unknown;
			return;
		}
		substr = {first_delimiter_pos + 1, std::ranges::find(first_delimiter_pos + 1, str.end(), '+')};
	}
	key = to_keycode(substr.data());
}

//

consteval tr::scancode tr::keyboard_literals::operator""_s(const char* cstr, usize size)
{
	scancode result{to_scancode({cstr, size})};
	if (result == scancode::unknown) {
		throw std::invalid_argument{"Invalid scancode name."};
	}
	return result;
}

consteval tr::scan_chord tr::keyboard_literals::operator""_sc(const char* cstr, usize size)
{
	tr::scan_chord chord{{cstr, size}};
	if (chord.scan == scancode::unknown) {
		throw std::invalid_argument{"Invalid scancode name."};
	}
	if (chord.mods == keymod::unknown) {
		throw std::invalid_argument{"Invalid keyboard modifier name."};
	}
	return chord;
}

//

consteval tr::keycode tr::keyboard_literals::operator""_k(const char* cstr, usize)
{
	keycode result{to_keycode(cstr)};
	TR_ASSERT(result != keycode::unknown, "Invalid keycode name.");
	return result;
}

consteval tr::key_chord tr::keyboard_literals::operator""_kc(const char* cstr, usize)
{
	tr::key_chord chord{cstr};
	if (chord.key == keycode::unknown) {
		throw std::invalid_argument{"Invalid keycode name."};
	}
	if (chord.mods == keymod::unknown) {
		throw std::invalid_argument{"Invalid keyboard modifier name."};
	}
	return chord;
}