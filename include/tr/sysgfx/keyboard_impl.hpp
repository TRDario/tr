#pragma once
#include "keyboard.hpp"

namespace tr::sys::impl {
	// Copied from SDL with modifications.
	constexpr std::array<cstring_view, 291> SCANCODE_NAME_TABLE{{
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
	keycode to_keycode(cstring_view str);
} // namespace tr::sys::impl

constexpr tr::sys::scancode tr::sys::to_scancode(std::string_view str)
{
	for (usize i = 0; i < impl::SCANCODE_NAME_TABLE.size(); ++i) {
		if (!impl::SCANCODE_NAME_TABLE[i].empty() && impl::SCANCODE_NAME_TABLE[i] == str) {
			return scancode(i);
		}
	}
	return tr::sys::scancode::UNKNOWN;
}

constexpr tr::cstring_view tr::sys::name(scancode scan)
{
	return impl::SCANCODE_NAME_TABLE[int(scan)];
}

constexpr std::string tr::sys::name(scan_chord scan)
{
	std::string str;
	if (scan.mods & keymod::CTRL) {
		str.append("Ctrl+");
	}
	if (scan.mods & keymod::ALT) {
		str.append("Alt+");
	}
	if (scan.mods & keymod::SHIFT) {
		str.append("Shift+");
	}
	str.append(name(scan.scan));
	return str;
}

consteval tr::sys::scancode tr::sys::literals::keyboard_literals::operator""_s(const char* cstr, usize size)
{
	scancode result{to_scancode({cstr, size})};
	TR_ASSERT(result != scancode::UNKNOWN, "Invalid scancode name.");
	return result;
}

consteval tr::sys::scan_chord tr::sys::literals::keyboard_literals::operator""_sc(const char* cstr, usize size)
{
	tr::sys::scan_chord chord{};
	const std::string_view str{cstr, size};

	if (str.find('+') == str.npos) {
		chord.scan = operator""_s(cstr, size);
		return chord;
	}

	std::string_view substr{str.begin(), str.begin() + str.find('+')};
	while (substr.end() != str.end()) {
		if (substr == "Shift") {
			chord.mods |= keymod::SHIFT;
		}
		else if (substr == "Ctrl") {
			chord.mods |= keymod::CTRL;
		}
		else if (substr == "Alt") {
			chord.mods |= keymod::ALT;
		}
		else {
			TR_ASSERT(false, "Invalid keyboard modifier name.");
		}

		const usize next_start{str.find('+', substr.end() - str.begin() + 1)};
		substr = {substr.end() + 1, next_start == str.npos ? str.end() : str.begin() + next_start};
	}
	chord.scan = to_scancode(substr);
	if (chord.scan == scancode::UNKNOWN) {
		TR_ASSERT(false, "Invalid key name.");
	}
	return chord;
}

//

constexpr tr::sys::keycode tr::sys::to_keycode(cstring_view cstr)
{
	if (cstr.empty()) {
		return keycode::UNKNOWN;
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
	if (int(scan) >= int(scancode::CAPSLOCK)) {
		return keycode{int(scan) | (1 << 30)};
	}

	return impl::to_keycode(cstr);
}

consteval tr::sys::keycode tr::sys::literals::keyboard_literals::operator""_k(const char* cstr, usize)
{
	keycode result{to_keycode(cstr)};
	TR_ASSERT(result != keycode::UNKNOWN, "Invalid keycode name.");
	return result;
}

consteval tr::sys::key_chord tr::sys::literals::keyboard_literals::operator""_kc(const char* cstr, usize size)
{
	tr::sys::key_chord chord{};
	const std::string_view str{cstr, size};

	if (str.find('+') == str.npos) {
		chord.key = operator""_k(cstr, size);
		return chord;
	}

	std::string_view substr{str.begin(), str.begin() + str.find('+')};
	while (substr.end() != str.end()) {
		if (substr == "Shift") {
			chord.mods |= keymod::SHIFT;
		}
		else if (substr == "Ctrl") {
			chord.mods |= keymod::CTRL;
		}
		else if (substr == "Alt") {
			chord.mods |= keymod::ALT;
		}
		else {
			TR_ASSERT(false, "Invalid keyboard modifier name.");
		}

		const usize next_start{str.find('+', substr.end() - str.begin() + 1)};
		substr = {substr.end() + 1, next_start == str.npos ? str.end() : str.begin() + next_start};
	}
	chord.key = to_keycode(substr.data());
	if (chord.key == keycode::UNKNOWN) {
		TR_ASSERT(false, "Invalid key name.");
	}
	return chord;
}