///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of keyboard.hpp.                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "keyboard.hpp"

/////////////////////////////////////////////////////////// SCANCODE AND KEYCODE //////////////////////////////////////////////////////////

namespace tr::sys {
	// Copied from SDL with modifications.
	constexpr std::array<cstring_view, 291> scancode_name_table{{
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

	// Fallback for Unicode characters.
	keycode to_keycode_fallback(cstring_view str);
} // namespace tr::sys

constexpr tr::sys::scancode tr::sys::to_scancode(std::string_view str)
{
	for (usize i = 0; i < scancode_name_table.size(); ++i) {
		if (!scancode_name_table[i].empty() && scancode_name_table[i] == str) {
			return scancode(i);
		}
	}
	return tr::sys::scancode::unknown;
}

constexpr tr::sys::keycode tr::sys::to_keycode(cstring_view cstr)
{
	if (cstr.empty()) {
		return keycode::unknown;
	}

	const std::string_view str{cstr};

	if (str.size() == 1 && u8(str[0]) < 0x80) {
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
	if (int(scan) >= 57) {
		return keycode{int(scan) | (1 << 30)};
	}

	return to_keycode_fallback(cstr);
}

constexpr tr::cstring_view tr::sys::name(scancode scan)
{
	return scancode_name_table[int(scan)];
}

////////////////////////////////////////////////////////////////// CHORDS /////////////////////////////////////////////////////////////////

constexpr tr::sys::scan_chord::scan_chord(scancode scan)
	: scan{scan}
{
}

constexpr tr::sys::scan_chord::scan_chord(keymod mods, scancode scan)
	: mods{mods}, scan{scan}
{
}

constexpr tr::sys::scan_chord::scan_chord(std::string_view str)
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

constexpr std::string tr::sys::scan_chord::name() const
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
	str.append(sys::name(scan));
	return str;
}

constexpr tr::sys::key_chord::key_chord(keycode key)
	: key{key}
{
}

constexpr tr::sys::key_chord::key_chord(keymod mods, keycode key)
	: mods{mods}, key{key}
{
}

constexpr tr::sys::key_chord::key_chord(cstring_view cstr)
{
	const std::string_view str{cstr};
	const auto first_delimiter_pos{str.find('+')};
	if (first_delimiter_pos == str.npos) {
		key = to_keycode(cstr);
		return;
	}

	std::string_view substr{str.begin(), str.begin() + str.find('+')};
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
	key = to_keycode(substr.data());
}

///////////////////////////////////////////////////////////////// LITERALS ////////////////////////////////////////////////////////////////

consteval tr::sys::scancode tr::sys::keyboard_literals::operator""_s(const char* cstr, usize size)
{
	scancode result{to_scancode({cstr, size})};
	if (result == scancode::unknown) {
		throw std::invalid_argument{"Invalid scancode name."};
	}
	return result;
}

consteval tr::sys::scan_chord tr::sys::keyboard_literals::operator""_sc(const char* cstr, usize size)
{
	tr::sys::scan_chord chord{{cstr, size}};
	if (chord.scan == scancode::unknown) {
		throw std::invalid_argument{"Invalid scancode name."};
	}
	if (chord.mods == keymod::unknown) {
		throw std::invalid_argument{"Invalid keyboard modifier name."};
	}
	return chord;
}

//

consteval tr::sys::keycode tr::sys::keyboard_literals::operator""_k(const char* cstr, usize)
{
	keycode result{to_keycode(cstr)};
	TR_ASSERT(result != keycode::unknown, "Invalid keycode name.");
	return result;
}

consteval tr::sys::key_chord tr::sys::keyboard_literals::operator""_kc(const char* cstr, usize)
{
	tr::sys::key_chord chord{cstr};
	if (chord.key == keycode::unknown) {
		throw std::invalid_argument{"Invalid keycode name."};
	}
	if (chord.mods == keymod::unknown) {
		throw std::invalid_argument{"Invalid keyboard modifier name."};
	}
	return chord;
}

//////////////////////////////////////////////////////////////// FORMATTERS ///////////////////////////////////////////////////////////////

template <typename FormatContext>
constexpr auto TR_FMT::formatter<tr::sys::scancode>::format(tr::sys::scancode scan, FormatContext& ctx) const
{
	ctx.advance_to(TR_FMT::formatter<const char*>::format(name(scan), ctx));
	return ctx.out();
}

template <typename FormatContext>
constexpr auto TR_FMT::formatter<tr::sys::scan_chord>::format(tr::sys::scan_chord chord, FormatContext& ctx) const
{
	ctx.advance_to(TR_FMT::formatter<std::string>::format(chord.name(), ctx));
	return ctx.out();
}

template <typename FormatContext> constexpr auto TR_FMT::formatter<tr::sys::keycode>::format(tr::sys::keycode key, FormatContext& ctx) const
{
	ctx.advance_to(TR_FMT::formatter<std::string>::format(name(key), ctx));
	return ctx.out();
}

template <typename FormatContext>
constexpr auto TR_FMT::formatter<tr::sys::key_chord>::format(tr::sys::key_chord chord, FormatContext& ctx) const
{
	ctx.advance_to(TR_FMT::formatter<std::string>::format(chord.name(), ctx));
	return ctx.out();
}