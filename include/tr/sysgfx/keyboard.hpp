#pragma once
#include "../utility/macro.hpp"

namespace tr::sys {
	struct key_down_event;
	struct key_up_event;

	// Physical keycode.
	class scancode {
	  public:
		// The base enumerator.
		enum class enum_t {
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
			SEMICOLON = 51,
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
		using enum enum_t;

		// Default-constructs a scancode.
		constexpr scancode() = default;
		// Casts an integer into a scancode.
		constexpr explicit scancode(int base);
		// Constructs a keycode from the base enumerator.
		constexpr scancode(enum_t base);
		// Constructs a scancode by matching it against a key name string.
		explicit scancode(const char* name);

		constexpr bool operator==(const scancode&) const = default;

		// Casts the scancode into an integer.
		constexpr explicit operator int() const;
		// Casts the name of the key.
		const char* name() const;

	  private:
		// The base enumerator.
		enum_t m_base{};
	};
	// Gets a top row scancode from a numeric value.
	constexpr scancode make_top_row_scancode(int v);

	// Virtual keycode.
	class keycode {
	  public:
		// The base enumerator.
		enum class enum_t {
			UNKNOWN,
			BACKSPACE = '\b',
			TAB,
			ENTER = '\r',
			ESCAPE = '\x1b',
			SPACE = ' ',
			EXCLAMATION_MARK,
			DOUBLE_QUOTE,
			HASH,
			DOLLAR,
			PERCENT,
			AMPERSAND,
			QUOTE,
			LEFT_PARENTHESIS,
			RIGHT_PARENTHESIS,
			ASTERRISK,
			PLUS,
			COMMA,
			MINUS,
			PERIOD,
			SLASH,
			TOP_ROW_0,
			TOP_ROW_1,
			TOP_ROW_2,
			TOP_ROW_3,
			TOP_ROW_4,
			TOP_ROW_5,
			TOP_ROW_6,
			TOP_ROW_7,
			TOP_ROW_8,
			TOP_ROW_9,
			COLON,
			SEMICOLON,
			LESS,
			EQUALS,
			GREATER,
			QUESTION_MARK,
			AT,
			LEFT_BRACKET = '[',
			BACKSLASH,
			RIGHT_BRACKET,
			CARET,
			UNDERSCORE,
			BACKQUOTE,
			A,
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
			CAPSLOCK = 1'073'741'881,
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
			APPLICATION = 1'073'741'925,
			LCTRL = 1'073'742'048,
			LSHIFT,
			LALT,
			LWIN,
			RCTRL,
			RSHIFT,
			RALT,
			RWIN
		};
		using enum enum_t;

		// Default-constructs a keycode.
		constexpr keycode() = default;
		// Casts an integer into a keycode.
		constexpr explicit keycode(int base);
		// Constructs a keycode from the base enumerator.
		constexpr keycode(enum_t base);
		// Constructs a scancode by matching it against a key name string.
		explicit keycode(const char* name);

		constexpr bool operator==(const keycode&) const = default;

		// Casts the keycode into an integer.
		constexpr explicit operator int() const;

		// Casts the name of the key.
		std::string name() const;

	  private:
		enum_t m_base{};
	};
	// Gets a top row keycode from a numeric value.
	constexpr keycode make_top_row_keycode(int v);

	// Key modifier bitmasks.
	enum class keymod : u16 {
		NONE = 0x0000,
		SHIFT = 0x0003,
		CTRL = 0x00C0,
		ALT = 0x0300
	};
	DEFINE_BITMASK_OPERATORS(keymod);
	// Scancode chord.
	struct scan_chord {
		// The chord scancode.
		scancode scan;
		// The chord modifiers.
		keymod mods{keymod::NONE};

		constexpr bool operator==(const scan_chord&) const = default;
	};
	// Key chord.
	struct key_chord {
		// The chord keycode.
		keycode key;
		// The chord modifiers.
		keymod mods{keymod::NONE};

		constexpr bool operator==(const key_chord&) const = default;
	};

	// Keyboard key state.
	class key_state {
	  public:
		// Constructs an empty key state.
		key_state() = default;

		// Gets whether a key is held.
		bool held(scancode key) const;

		// Updates the key state.
		void update(const key_down_event& event);
		// Updates the key state.
		void update(const key_up_event& event);
		// Forces a key to be considered held down.
		void force_down(scancode key);
		// Forces a key to the considered up.
		void force_up(scancode key);

	  private:
		// The state is stored in the form of bitflags.
		std::array<std::byte, 14> buffer{};
	};
	// Keyboard key and modifier state.
	class keyboard_state : public key_state {
	  public:
		// The currently held modifiers.
		keymod mods{keymod::NONE};

		// Constructs an empty keyboard state.
		keyboard_state() = default;

		// Updates the key state.
		void update(const key_down_event& event);
		// Updates the key state.
		void update(const key_up_event& event);

	  private:
		using key_state::update;
	};

	// Gets whether the clipboard is empty.
	bool clipboard_empty();
	// Gets the clipboard text.
	std::string clipboard_text();

	// Sets the clipboard text.
	void set_clipboard_text(const char* text);
	// Sets the clipboard text.
	void set_clipboard_text(const std::string& text);
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

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

constexpr tr::sys::scancode::scancode(int base)
	: m_base{enum_t(base)}
{
}

constexpr tr::sys::scancode::scancode(enum_t base)
	: m_base{base}
{
}

constexpr tr::sys::scancode::operator int() const
{
	return int(m_base);
}

constexpr tr::sys::scancode tr::sys::make_top_row_scancode(int v)
{
	TR_ASSERT(v >= 0 && v < 10, "Tried to create top row number scancode {} outside the range [0-9]", v);

	return scancode(int(scancode::TOP_ROW_0) + v);
}

constexpr tr::sys::keycode::keycode(int base)
	: m_base{enum_t(base)}
{
}

constexpr tr::sys::keycode::keycode(enum_t base)
	: m_base{base}
{
}

constexpr tr::sys::keycode::operator int() const
{
	return int(m_base);
}

constexpr tr::sys::keycode tr::sys::make_top_row_keycode(int v)
{
	TR_ASSERT(v >= 0 && v < 10, "Tried to create top row number keycode {} outside the range [0-9]", v);

	return keycode(int(keycode::TOP_ROW_0) + v);
}