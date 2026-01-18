///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides keyboard datatypes and functionality.                                                                                        //
//                                                                                                                                       //
// Physical keys (scancodes) and virtual keys (keycodes) are separated, as are their chords, which also consist of modifiers             //
// (represented as a bitmask). All of the aforementioned are primarily constructed through literals provided in                          //
// tr::sys::keyboard_literals, as well as tr::literals, but keycodes can also be constructed with any Unicode codepoint (Unicode         //
// characters currently aren't supported in the literals), while chords can be constructed from their components. All of the above       //
// datatypes are hashable, binary readable/writable, and formattable:                                                                    //
//     - "A"_s -> code belonging to the physical key associated with 'A' on the English keyboard                                         //
//     - "A"_k -> code of the virtual 'A' key                                                                                            //
//     - tr::sys::keycode{0x010D} -> code of the virtual 'Ć' key                                                                         //
//     - "Ctrl+K"_sc -> chord where Ctrl is held and the physical key associated with 'K' on the english keyboard is pressed             //
//     - tr::sys::scan_chord{tr::sys::keymod::ctrl | tr::sys::keymod::shift, "K"_s} -> equivalent to "Ctrl+Shift+K"_sc                   //
//     - "Ctrl+K"_kc -> chord where Ctrl is held and the virtual 'K' key is pressed                                                      //
//     - tr::sys::key_chord{tr::sys::keymod::ctrl | tr::sys::keymod::shift, "K"_k} -> equivalent to "Ctrl+Shift+K"_kc                    //
//                                                                                                                                       //
// All of the above can be converted to and from strings:                                                                                //
//     - tr::sys::scancode{"A"} -> equivalent to "A"_s                                                                                   //
//     - tr::sys::scancode{"Invalid"} -> scancode::unknown                                                                               //
//     - tr::sys::name("Shift"_s) -> "Shift"                                                                                             //
//     - tr::sys::keycode{"Ć"} -> equivalent to tr::sys::keycode{0x010D}                                                                 //
//     - tr::sys::keycode{"Invalid"} -> keycode::unknown                                                                                 //
//     - tr::sys::name("Ctrl"_k) -> "Ctrl"                                                                                               //
//     - tr::sys::scan_chord{"Shift+A"} -> equivalent to "Shift+A"_sc                                                                    //
//     - tr::sys::scan_chord{"Invalid"} -> .scan = scancode::unknown                                                                     //
//     - tr::sys::scan_chord{tr::sys::keymod::ctrl, "S"_s}.name() -> "Ctrl+S"                                                            //
//     - tr::sys::key_chord{"Ctrl+Ć"} -> equivalent to {tr::sys::keymod::ctrl, tr::sys::keycode{0x010D}}                                 //
//     - tr::sys::key_chord{"Invalid"} -> .key = keycode::unknown                                                                        //
//     - tr::sys::key_chord{tr::sys::keymod::ctrl, "N"_k}.name() -> "Ctrl+N"                                                             //
//                                                                                                                                       //
// Keyboard state can be stored in tr::sys::scan_state (just keys) or tr::sys::keyboard_state (including modifiers). Their state can be  //
// queried, updated with events, or manually manipulated:                                                                                //
//     - keyboard.held("A"_s) -> returns whether the physical 'A' key is held                                                            //
//     - keyboard.held(tr::sys::keymod::ctrl | tr::sys::keymod::shift) -> returns whether Ctrl + Shift is held                           //
//     - keyboard.mods == tr::sys::keymod::ctrl | tr::sys::keymod::shift -> returns whether exactly Ctrl + Shift is held                 //
//     - keyboard.held("Ctrl+A"_sc) -> returns whether Ctrl + A is held                                                                  //
//     - keyboard.handle_event(evt) -> updates the keyboard state based on the received event                                            //
//     - keyboard.force_down("B"_s) -> forces B to be considered held                                                                    //
//     - keyboard.force_up("B"_s) -> forces B to be considered unheld                                                                    //
//                                                                                                                                       //
// The clipboard can be queried for text and set:                                                                                        //
//     - tr::sys::clipboard_empty() -> false if the clipboard has text, true otherwise                                                   //
//     - tr::sys::clipboard_text() -> string containing the text that was in the clipboard                                               //
//     - tr::sys::set_clipboard_text("example") -> clipboard now contains "example"                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::sys {
	struct key_down_event;
	struct key_up_event;
	class event;

	// Physical keycode.
	enum class scancode {
		unknown // Unknown scancode.
	};

	// Labelled keycode.
	enum class keycode {
		unknown, // Unknown keycode.
	};

	// Key modifier bitmasks
	enum class keymod : u16 {
		none = 0x0000,
		shift = 0x0003,
		ctrl = 0x00C0,
		alt = 0x0300,
		unknown = 0xFFFF
	};
	TR_DEFINE_ENUM_BITMASK_OPERATORS(keymod);

	// Converts a string to a scancode.
	constexpr scancode to_scancode(std::string_view str);
	// Converts a string to a keycode.
	constexpr keycode to_keycode(cstring_view str);
	// Gets the name of a scancode.
	constexpr cstring_view name(scancode scan);
	// Gets the name of a keycode.
	std::string name(keycode scan);

	// Scancode chord.
	struct scan_chord {
		// The chord modifiers.
		keymod mods{keymod::none};
		// The chord scancode.
		scancode scan{scancode::unknown};

		// Default-constructs a scan chord.
		constexpr scan_chord() = default;
		// Constructs a scan chord without modifiers.
		constexpr scan_chord(scancode scan);
		// Constructs a scan chord from components.
		constexpr scan_chord(keymod mods, scancode scan);
		// Converts a string to a scan chord. An invalid string results in a chord with scancode::unknown.
		constexpr explicit scan_chord(std::string_view str);

		constexpr bool operator==(const scan_chord&) const = default;

		// Gets the name of the scan chord.
		constexpr std::string name() const;
	};
	// Keycode chord.
	struct key_chord {
		// The chord modifiers.
		keymod mods{keymod::none};
		// The chord keycode.
		keycode key{keycode::unknown};

		// Default-constructs a key chord.
		constexpr key_chord() = default;
		// Constructs a key chord without modifiers.
		constexpr key_chord(keycode key);
		// Constructs a key chord from components.
		constexpr key_chord(keymod mods, keycode key);
		// Converts a string to a key chord. An invalid string results in a chord with keycode::unknown.
		constexpr explicit key_chord(cstring_view cstr);

		constexpr bool operator==(const key_chord&) const = default;

		// Gets the name of the key chord.
		std::string name() const;
	};

	// Keyboard key state.
	class scan_state {
	  public:
		// Constructs an empty key state.
		scan_state() = default;

		// Gets whether a key is held.
		bool held(scancode scan) const;

		// Updates the key state.
		void handle_event(const event& event);
		// Updates the key state.
		void handle_event(const key_down_event& event);
		// Updates the key state.
		void handle_event(const key_up_event& event);
		// Forces a key to be considered held down.
		void force_down(scancode scan);
		// Forces a key to the considered up.
		void force_up(scancode scan);

	  private:
		// Sentinel for an invalid key state index.
		static constexpr int invalid_key_state_index{-1};

		// The state is stored in the form of bitflags.
		std::array<std::byte, 14> buffer{};

		// Converts a scancode into a key state index.
		static constexpr int to_index(scancode key);
	};

	// Keyboard key and modifier state.
	class keyboard_state : public scan_state {
	  public:
		// The currently held modifiers.
		keymod mods{keymod::none};

		// Constructs an empty keyboard state.
		keyboard_state() = default;

		// Gets whether one or more modifiers is held.
		bool held(keymod mods) const;
		// Gets whether a chord is held.
		bool held(scan_chord chord) const;

		// Updates the key state.
		void handle_event(const event& event);
		// Updates the key state.
		void handle_event(const key_down_event& event);
		// Updates the key state.
		void handle_event(const key_up_event& event);

	  private:
		using scan_state::handle_event;
	};

	// Keyboard literals.
	inline namespace keyboard_literals {
		// Creates a scancode matching a string.
		consteval scancode operator""_s(const char* str, usize size);
		// Creates a scancode chord matching a string.
		consteval scan_chord operator""_sc(const char* str, usize size);
		// Creates a keycode matching a string.
		// Keycodes corresponding to unicode characters cannot be currently created using this.
		consteval keycode operator""_k(const char* str, usize size);
		// Creates a keycode chord matching a string.
		// Keycodes corresponding to unicode characters cannot be currently created using this.
		consteval key_chord operator""_kc(const char* str, usize size);
	} // namespace keyboard_literals

	// Gets whether the clipboard is empty.
	bool clipboard_empty();
	// Gets the clipboard text.
	std::string clipboard_text();
	// Sets the clipboard text.
	void set_clipboard_text(cstring_view text);
} // namespace tr::sys

namespace tr {
	inline namespace literals {
		using namespace tr::sys::keyboard_literals;
	} // namespace literals
} // namespace tr

// Scancode hasher.
template <> struct std::hash<tr::sys::scancode> {
	std::size_t operator()(tr::sys::scancode code) const;
};
// Keycode hasher.
template <> struct std::hash<tr::sys::keycode> {
	std::size_t operator()(tr::sys::keycode code) const;
};
// Scan chord hasher.
template <> struct std::hash<tr::sys::scan_chord> {
	std::size_t operator()(tr::sys::scan_chord chord) const;
};
// Key chord hasher.
template <> struct std::hash<tr::sys::key_chord> {
	std::size_t operator()(tr::sys::key_chord chord) const;
};

// Scancode formatter.
template <> struct TR_FMT::formatter<tr::sys::scancode> : public TR_FMT::formatter<const char*> {
	// Formats a scancode.
	template <typename FormatContext> constexpr auto format(tr::sys::scancode scan, FormatContext& ctx) const;
};
// Scan chord formatter.
template <> struct TR_FMT::formatter<tr::sys::scan_chord> : public TR_FMT::formatter<std::string> {
	// Formats a scan chord.
	template <typename FormatContext> constexpr auto format(tr::sys::scan_chord chord, FormatContext& ctx) const;
};
// Keycode formatter.
template <> struct TR_FMT::formatter<tr::sys::keycode> : public TR_FMT::formatter<std::string> {
	// Formats a keycode.
	template <typename FormatContext> constexpr auto format(tr::sys::keycode key, FormatContext& ctx) const;
};
// Key chord formatter.
template <> struct TR_FMT::formatter<tr::sys::key_chord> : public TR_FMT::formatter<std::string> {
	// Formats a key chord.
	template <typename FormatContext> constexpr auto format(tr::sys::key_chord chord, FormatContext& ctx) const;
};

// Scancode binary reader.
template <> struct tr::binary_reader<tr::sys::scancode> : tr::raw_binary_reader<tr::sys::scancode> {};
// Scancode binary writer.
template <> struct tr::binary_writer<tr::sys::scancode> : tr::raw_binary_writer<tr::sys::scancode> {};

// Scan chord binary reader.
template <> struct tr::binary_reader<tr::sys::scan_chord> {
	void operator()(std::istream& is, tr::sys::scan_chord& out) const;
};
// Scan chord binary writer.
template <> struct tr::binary_writer<tr::sys::scan_chord> {
	void operator()(std::ostream& os, const tr::sys::scan_chord& in) const;
};

// Keycode bianry reader.
template <> struct tr::binary_reader<tr::sys::keycode> : tr::raw_binary_reader<tr::sys::keycode> {};
// Keycode binary writer.
template <> struct tr::binary_writer<tr::sys::keycode> : tr::raw_binary_writer<tr::sys::keycode> {};

// Key chord binary reader.
template <> struct tr::binary_reader<tr::sys::key_chord> {
	void operator()(std::istream& is, tr::sys::key_chord& out) const;
};
// Key chord binary writer.
template <> struct tr::binary_writer<tr::sys::key_chord> {
	void operator()(std::ostream& os, const tr::sys::key_chord& in) const;
};

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "keyboard_impl.hpp" // IWYU pragma: keep