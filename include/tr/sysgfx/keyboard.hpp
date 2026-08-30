/// @file
/// @brief Provides keyboard datatypes and functionality.

#pragma once
#include "../utility/enum.hpp"
#include "../utility/zstring_view.hpp"

namespace tr
{
	struct key_down_event;
	struct key_up_event;
	class event;
} // namespace tr

//

namespace tr
{
	/// Physical keycode.
	enum class scancode
	{
		/// Unknown scancode.
		unknown
	};

	/// Labelled keycode.
	enum class keycode
	{
		/// Unknown keycode.
		unknown,
	};

	/// Key modifier bitmasks.
	enum class keymod : u16
	{
		/// No held keyboard modifiers.
		none = 0x0000,
		/// Left-or-right shift key held.
		shift = 0x0003,
		/// Left-or-right ctrl key held.
		ctrl = 0x00C0,
		/// Left-or-right alt key held.
		alt = 0x0300,
		/// Unknown keyboard modifiers.
		unknown = 0xFFFF
	};

	/// @cond implementation_details
	TR_DEFINE_ENUM_BITMASK_OPERATORS(keymod);
	/// @endcond

	/// @name Keyboard
	/// @{

	/// Converts a string to a scancode.
	/// @param str Scancode string.
	/// @return Scancode associated with the string, or `scancode::unknown`.
	constexpr scancode to_scancode(std::string_view str);

	/// Converts a string to a keycode.
	/// @param str Keycode string.
	/// @return Keycode associated with the string, or `keycode::unknown`.
	constexpr keycode to_keycode(zstring_view str);

	/// Gets the name of a scancode.
	/// @param scan Scancode to get the name of.
	/// @return String associated with the scancode.
	constexpr zstring_view name(scancode scan);

	/// Gets the name of a keycode.
	/// @param key Keycode to get the name of.
	/// @return String associated with the keycode.
	std::string name(keycode key);

	/// @}

	/// Scancode chord.
	struct scan_chord
	{
		// Chord modifiers.
		keymod mods{keymod::none};

		// Chord scancode.
		scancode scan{scancode::unknown};

		/// @name Constructors
		/// @{

		/// Default-constructs a scan chord.
		constexpr scan_chord() = default;

		/// Constructs a scan chord without modifiers.
		/// @param scan Chord scancode.
		constexpr scan_chord(scancode scan);

		/// Constructs a scan chord from components.
		/// @param mods Chord modifiers.
		/// @param scan Chord scancode.
		constexpr scan_chord(keymod mods, scancode scan);

		/// Converts a string to a scan chord. An invalid string results in a chord with `scancode::unknown`.
		/// @param str Chord string.
		constexpr explicit scan_chord(std::string_view str);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two chords for equality.
		/// @param lhs, rhs Chords to compare.
		/// @return Whether the chords are equal.
		friend constexpr bool operator==(scan_chord lhs, scan_chord rhs) = default;

		/// @}
		/// @name Name
		/// @{

		/// Gets the name of the scan chord.
		/// @return Chord string.
		constexpr std::string name() const;

		/// @}
	};

	/// Keycode chord.
	struct key_chord
	{
		// Chord modifiers.
		keymod mods{keymod::none};

		// Chord keycode.
		keycode key{keycode::unknown};

		/// @name Constructors
		/// @{

		/// Default-constructs a key chord.
		constexpr key_chord() = default;

		/// Constructs a key chord without modifiers.
		/// @param key Chord keycode.
		constexpr key_chord(keycode key);

		/// Constructs a key chord from components.
		/// @param mods Chord modifiers.
		/// @param key Chord keycode.
		constexpr key_chord(keymod mods, keycode key);

		/// Converts a string to a key chord. An invalid string results in a chord with `keycode::unknown`.
		/// @param cstr Chord string.
		constexpr explicit key_chord(zstring_view cstr);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two chords for equality.
		/// @param lhs, rhs Chords to compare.
		/// @return Whether the chords are equal.
		friend constexpr bool operator==(key_chord lhs, key_chord rhs) = default;

		/// @}
		/// @name Name
		/// @{

		/// Gets the name of the key chord.
		/// @return Chord string.
		std::string name() const;

		/// @}
	};

	//

	/// Keyboard key state.
	class scan_state
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an empty key state.
		scan_state() = default;

		/// @}
		/// @name Queries
		/// @{

		/// Gets whether a key is held.
		/// @param scan Scancode to check the status of.
		/// @return `true` if the scancode is held, `false` otherwise.
		bool held(scancode scan) const;

		/// @}
		/// @name Events
		/// @{

		/// Updates the key state.
		/// @param event Event to handle.
		void handle_event(const event& event);

		/// Updates the key state.
		/// @param event Event to handle.
		void handle_event(const key_down_event& event);

		/// Updates the key state.
		/// @param event Event to handle.
		void handle_event(const key_up_event& event);

		/// Forces a key to be considered held down.
		/// @param scan Scancode to force down.
		void force_down(scancode scan);

		/// Forces a key to the considered up.
		/// @param scan Scancode to force up.
		void force_up(scancode scan);

		/// @}

	  private:
		/// Keyboard state is stored in the form of bitflags.
		std::array<std::byte, 14> buffer{};
	};

	/// Keyboard key and modifier state.
	class keyboard_state : private scan_state
	{
	  public:
		/// Currently held modifiers.
		keymod mods{keymod::none};

		/// @name Constructors
		/// @{

		/// Constructs an empty keyboard state.
		keyboard_state() = default;

		/// @}
		/// @name Queries
		/// @{

		using scan_state::held;

		/// Gets whether one or more modifiers is held.
		/// @param mods Bitmask of modifiers to check the status of.
		/// @return `true` if all of the modifiers are held, `false` otherwise.
		bool held(keymod mods) const;

		/// Gets whether a chord is held.
		/// @param chord Chord to check the status of.
		/// @return `true` if the chord is held, `false` otherwise.
		bool held(scan_chord chord) const;

		/// @}
		/// @name Events
		/// @{

		/// Updates the key state.
		/// @param event Event to handle.
		void handle_event(const event& event);

		/// Updates the key state.
		/// @param event Event to handle.
		void handle_event(const key_down_event& event);

		/// Updates the key state.
		/// @param event Event to handle.
		void handle_event(const key_up_event& event);

		using scan_state::force_down;

		using scan_state::force_up;

		/// @}
	};

	//

	inline namespace literals
	{
		// Namespace containing keyboard literals.
		inline namespace keyboard_literals
		{
			/// @name Literals
			/// @{

			/// Creates a scancode matching a string.
			/// @param str Scancode string.
			/// @param size Scancode string length.
			/// @return Matching scancode.
			consteval scancode operator""_s(const char* str, usize size);

			/// Creates a scancode chord matching a string.
			/// @param str Chord string.
			/// @param size Chord string length.
			/// @return Matching scan chord.
			consteval scan_chord operator""_sc(const char* str, usize size);

			/// Creates a keycode matching a string.
			/// @note Keycodes corresponding to unicode characters cannot be currently created using this.
			/// @param str Keycode string.
			/// @param size Keycode string length.
			/// @return Matching keycode.
			consteval keycode operator""_k(const char* str, usize size);

			/// Creates a keycode chord matching a string.
			/// @note Keycodes corresponding to unicode characters cannot be currently created using this.
			/// @param str Chord string.
			/// @param size Chord string length.
			/// @return Matching key chord.
			consteval key_chord operator""_kc(const char* str, usize size);

			/// @}
		} // namespace keyboard_literals
	} // namespace literals
} // namespace tr

#include "impl/keyboard.hpp" // IWYU pragma: export