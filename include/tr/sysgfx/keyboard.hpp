#pragma once
#include "../utility/macro.hpp"

namespace tr::sys {
	struct key_down_event;
	struct key_up_event;

	// Physical keycode.
	enum class scancode {
		UNKNOWN,
		A = 4,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		TOP_ROW_1,
		TOP_ROW_2,
		TOP_ROW_3,
		TOP_ROW_4,
		TOP_ROW_5,
		TOP_ROW_6,
		TOP_ROW_7,
		TOP_ROW_8,
		TOP_ROW_9,
		TOP_ROW_0,
		ENTER,
		ESCAPE,
		BACKSPACE,
		TAB,
		SPACE,
		MINUS,
		EQUALS,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		BACKSLASH,
		HASH,
		SEMICOLON,
		APOSTROPHE,
		GRAVE,
		COMMA,
		PERIOD,
		SLASH,
		CAPSLOCK,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		PRINT_SCREEN,
		SCROLL_LOCK,
		PAUSE,
		INSERT,
		HOME,
		PAGE_UP,
		DELETE,
		END,
		PAGE_DOWN,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		NUM_LOCK,
		KEYPAD_DIVIDE,
		KEYPAD_MULTIPLY,
		KEYPAD_MINUS,
		KEYPAD_PLUS,
		KEYPAD_ENTER,
		KEYPAD_1,
		KEYPAD_2,
		KEYPAD_3,
		KEYPAD_4,
		KEYPAD_5,
		KEYPAD_6,
		KEYPAD_7,
		KEYPAD_8,
		KEYPAD_9,
		KEYPAD_0,
		KEYPAD_PERIOD,
		NONUS_BACKSLASH,
		APPLICATION,
		LCTRL = 224,
		LSHIFT,
		LALT,
		LWIN,
		RCTRL,
		RSHIFT,
		RALT,
		RWIN
	};
	// Converts a string to a scancode.
	constexpr scancode to_scancode(std::string_view str);
	// Gets the name of a scancode.
	constexpr cstring_view name(scancode scan);

	// Key modifier bitmasks
	enum class keymod : u16 {
		NONE = 0x0000,
		SHIFT = 0x0003,
		CTRL = 0x00C0,
		ALT = 0x0300
	};
	DEFINE_BITMASK_OPERATORS(keymod);

	// Scancode chord.
	struct scan_chord {
		// The chord modifiers.
		keymod mods;
		// The chord scancode.
		scancode scan;

		constexpr bool operator==(const scan_chord&) const = default;
	};
	// Gets the name of a scan chord.
	constexpr std::string name(scan_chord scan);

	inline namespace keyboard_literals {
		// Creates a scancode matching a string.
		consteval scancode operator""_scan(const char* str, usize size);
		// Creates a scancode chord matching a string.
		consteval scan_chord operator""_schord(const char* str, usize size);
	} // namespace keyboard_literals

	//

	// Labelled keycode.
	enum class keycode {
		UNKNOWN,
		// All unicode codepoints are valid.
		CAPSLOCK = 0x40000039,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		PRINT_SCREEN,
		SCROLL_LOCK,
		PAUSE,
		INSERT,
		HOME,
		PAGE_UP,
		DELETE,
		END,
		PAGE_DOWN,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		NUM_LOCK,
		KEYPAD_DIVIDE,
		KEYPAD_MULTIPLY,
		KEYPAD_MINUS,
		KEYPAD_PLUS,
		KEYPAD_ENTER,
		KEYPAD_1,
		KEYPAD_2,
		KEYPAD_3,
		KEYPAD_4,
		KEYPAD_5,
		KEYPAD_6,
		KEYPAD_7,
		KEYPAD_8,
		KEYPAD_9,
		KEYPAD_0,
		KEYPAD_PERIOD,
		APPLICATION = 0x40000065,
		LCTRL = 0x400000E0,
		LSHIFT,
		LALT,
		LWIN,
		RCTRL,
		RSHIFT,
		RALT,
		RWIN
	};
	// Converts a string to a keycode.
	constexpr keycode to_keycode(cstring_view str);
	// Gets the name of a keycode.
	std::string name(keycode scan);

	// Keycode chord.
	struct key_chord {
		// The chord modifiers.
		keymod mods;
		// The chord keycode.
		keycode key;

		constexpr bool operator==(const key_chord&) const = default;
	};
	// Gets the name of a key_chord.
	std::string name(key_chord scan);

	inline namespace keyboard_literals {
		// Creates a keycode matching a string.
		// Keycodes corresponding to unicode characters cannot be currently created using this.
		consteval keycode operator""_key(const char* str, usize size);
		// Creates a keycode chord matching a string.
		// Keycodes corresponding to unicode characters cannot be currently created using this.
		consteval key_chord operator""_kchord(const char* str, usize size);
	} // namespace keyboard_literals

	//

	// Gets whether the clipboard is empty.
	bool clipboard_empty();
	// Gets the clipboard text.
	std::string clipboard_text();
	// Sets the clipboard text.
	void set_clipboard_text(cstring_view text);
} // namespace tr::sys

template <> struct std::hash<tr::sys::scancode> {
	tr::u64 operator()(tr::sys::scancode code) const;
};
template <> struct std::hash<tr::sys::keycode> {
	tr::u64 operator()(tr::sys::keycode code) const;
};
template <> struct std::hash<tr::sys::scan_chord> {
	tr::u64 operator()(tr::sys::scan_chord chord) const;
};
template <> struct std::hash<tr::sys::key_chord> {
	tr::u64 operator()(tr::sys::key_chord chord) const;
};

// Scancode formatter.
template <> struct TR_FMT::formatter<tr::sys::scancode> : public TR_FMT::formatter<const char*> {
	template <typename FormatContext> constexpr auto format(tr::sys::scancode scan, FormatContext& ctx) const
	{
		ctx.advance_to(TR_FMT::formatter<const char*>::format(name(scan), ctx));
		return ctx.out();
	}
};
// Scan chord formatter.
template <> struct TR_FMT::formatter<tr::sys::scan_chord> : public TR_FMT::formatter<std::string> {
	template <typename FormatContext> constexpr auto format(tr::sys::scan_chord chord, FormatContext& ctx) const
	{
		ctx.advance_to(TR_FMT::formatter<std::string>::format(name(chord), ctx));
		return ctx.out();
	}
};
// Keycode formatter.
template <> struct TR_FMT::formatter<tr::sys::keycode> : public TR_FMT::formatter<std::string> {
	template <typename FormatContext> constexpr auto format(tr::sys::keycode key, FormatContext& ctx) const
	{
		ctx.advance_to(TR_FMT::formatter<std::string>::format(name(key), ctx));
		return ctx.out();
	}
};
// Key chord formatter.
template <> struct TR_FMT::formatter<tr::sys::key_chord> : public TR_FMT::formatter<std::string> {
	template <typename FormatContext> constexpr auto format(tr::sys::key_chord chord, FormatContext& ctx) const
	{
		ctx.advance_to(TR_FMT::formatter<std::string>::format(name(chord), ctx));
		return ctx.out();
	}
};

#include "keyboard_impl.hpp"