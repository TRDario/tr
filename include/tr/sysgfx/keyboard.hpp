/// @file
/// @brief Provides keyboard datatypes and functionality.

#pragma once
#include <tr/sysgfx/internal/keyboard.hpp>
#include <tr/utility/enum.hpp>

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
	[[nodiscard]] constexpr scancode to_scancode(std::string_view str) noexcept
	{
		for (usize i = 0; i < internal::scancode_names.size(); ++i) {
			if (!internal::scancode_names[i].empty() && internal::scancode_names[i] == str) {
				return static_cast<scancode>(i);
			}
		}
		return tr::scancode::unknown;
	}

	/// Converts a string to a keycode.
	/// @param str Keycode string.
	/// @return Keycode associated with the string, or `keycode::unknown`.
	[[nodiscard]] constexpr keycode to_keycode(zstring_view str) noexcept
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

		return internal::to_keycode(str);
	}

	/// Gets the name of a scancode.
	/// @param scan Scancode to get the name of.
	/// @return String associated with the scancode.
	[[nodiscard]] constexpr zstring_view name(scancode scan) noexcept
	{
		return internal::scancode_names[std::to_underlying(scan)];
	}

	/// Gets the name of a keycode.
	/// @param key Keycode to get the name of.
	/// @return String associated with the keycode.
	[[nodiscard]] std::string name(keycode key);

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
		[[nodiscard]] constexpr scan_chord() noexcept = default;

		/// Constructs a scan chord without modifiers.
		/// @param scan Chord scancode.
		[[nodiscard]] constexpr scan_chord(scancode scan) noexcept
			: scan{scan}
		{
		}

		/// Constructs a scan chord from components.
		/// @param mods Chord modifiers.
		/// @param scan Chord scancode.
		[[nodiscard]] constexpr scan_chord(keymod mods, scancode scan) noexcept
			: mods{mods}
			, scan{scan}
		{
		}

		/// Converts a string to a scan chord. An invalid string results in a chord with `scancode::unknown`.
		/// @param str Chord string.
		[[nodiscard]] constexpr explicit scan_chord(std::string_view str) noexcept
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

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two chords for equality.
		/// @param lhs, rhs Chords to compare.
		/// @return Whether the chords are equal.
		[[nodiscard]] friend constexpr bool operator==(scan_chord lhs, scan_chord rhs) noexcept = default;

		/// @}
		/// @name Name
		/// @{

		/// Gets the name of the scan chord.
		/// @return Chord string.
		[[nodiscard]] constexpr std::string name() const
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
		[[nodiscard]] constexpr key_chord() noexcept = default;

		/// Constructs a key chord without modifiers.
		/// @param key Chord keycode.
		[[nodiscard]] constexpr key_chord(keycode key) noexcept
			: key{key}
		{
		}

		/// Constructs a key chord from components.
		/// @param mods Chord modifiers.
		/// @param key Chord keycode.
		[[nodiscard]] constexpr key_chord(keymod mods, keycode key) noexcept
			: mods{mods}
			, key{key}
		{
		}

		/// Converts a string to a key chord. An invalid string results in a chord with `keycode::unknown`.
		/// @param cstr Chord string.
		[[nodiscard]] constexpr explicit key_chord(zstring_view cstr) noexcept
		{
			const zstring_view::iterator first_delimiter_pos{std::ranges::find(cstr, '+')};
			if (first_delimiter_pos == cstr.end()) {
				key = to_keycode(cstr);
				return;
			}

			std::string_view substr{cstr.begin(), first_delimiter_pos};
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
				substr = {first_delimiter_pos + 1, std::ranges::find(first_delimiter_pos + 1, cstr.end(), '+')};
			}
			key = to_keycode(substr.data());
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two chords for equality.
		/// @param lhs, rhs Chords to compare.
		/// @return Whether the chords are equal.
		[[nodiscard]] friend constexpr bool operator==(key_chord lhs, key_chord rhs) noexcept = default;

		/// @}
		/// @name Name
		/// @{

		/// Gets the name of the key chord.
		/// @return Chord string.
		[[nodiscard]] std::string name() const;

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
			[[nodiscard]] consteval scancode operator""_s(const char* str, usize size) noexcept
			{
				scancode result{to_scancode(std::string_view{str, size})};
				if (result == scancode::unknown) {
					std::unreachable();
				}
				return result;
			}

			/// Creates a scancode chord matching a string.
			/// @param str Chord string.
			/// @param size Chord string length.
			/// @return Matching scan chord.
			[[nodiscard]] consteval scan_chord operator""_sc(const char* str, usize size) noexcept
			{
				tr::scan_chord chord{std::string_view{str, size}};
				if (chord.scan == scancode::unknown || chord.mods == keymod::unknown) {
					std::unreachable();
				}
				return chord;
			}

			/// Creates a keycode matching a string.
			/// @note Keycodes corresponding to unicode characters cannot be currently created using this.
			/// @param str Keycode string.
			/// @return Matching keycode.
			[[nodiscard]] consteval keycode operator""_k(const char* str, usize) noexcept
			{
				keycode result{to_keycode(str)};
				if (result == keycode::unknown) {
					std::unreachable();
				}
				return result;
			}

			/// Creates a keycode chord matching a string.
			/// @note Keycodes corresponding to unicode characters cannot be currently created using this.
			/// @param str Chord string.
			/// @return Matching key chord.
			[[nodiscard]] consteval key_chord operator""_kc(const char* str, usize) noexcept
			{
				tr::key_chord chord{str};
				if (chord.key == keycode::unknown || chord.mods == keymod::unknown) {
					std::unreachable();
				}
				return chord;
			}

			/// @}
		} // namespace keyboard_literals
	} // namespace literals
} // namespace tr

//

#ifndef TR_DOXYGEN_SKIP
/// Scancode hasher.
template <>
struct boost::hash<tr::scancode>
{
	/// Hashes a scancode.
	/// @param code Scancode to hash.
	/// @return Scancode hash.
	constexpr std::size_t operator()(tr::scancode code) const noexcept
	{
		return static_cast<std::size_t>(code);
	}
};

/// Keycode hasher.
template <>
struct boost::hash<tr::keycode>
{
	/// Hashes a keycode.
	/// @param code Keycode to hash.
	/// @return Keycode hash.
	constexpr std::size_t operator()(tr::keycode code) const noexcept
	{
		return static_cast<std::size_t>(code);
	}
};

/// Scan chord hasher.
template <>
struct boost::hash<tr::scan_chord>
{
	/// Hashes a scan chord.
	/// @param chord Chord to hash.
	/// @return Scan chord hash.
	constexpr std::size_t operator()(tr::scan_chord chord) const noexcept
	{
		return (static_cast<std::size_t>(chord.scan) << 32) | static_cast<std::size_t>(chord.mods);
	}
};

/// Key chord hasher.
template <>
struct boost::hash<tr::key_chord>
{
	/// Hashes a key chord.
	/// @param chord Chord to hash.
	/// @return Key chord hash.
	constexpr std::size_t operator()(tr::key_chord chord) const noexcept
	{
		return (static_cast<std::size_t>(chord.key) << 32) | static_cast<std::size_t>(chord.mods);
	}
};
#endif

//

/// Scancode formatter.
template <>
struct std::formatter<tr::scancode>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid scancode format specification."};
		}
		return context.begin();
	}

	/// Formats a scancode.
	/// @tparam FormatContext Formatting context type.
	/// @param scan Scancode to format.
	/// @param context Formatting context.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::scancode scan, FormatContext& context) const
	{
		return std::ranges::copy(name(scan), context.out()).out;
	}
};

/// Scan chord formatter.
template <>
struct std::formatter<tr::scan_chord>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid scan chord format specification."};
		}
		return context.begin();
	}

	/// Formats a scan chord.
	/// @tparam FormatContext Formatting context type.
	/// @param chord Scan chord to format.
	/// @param context Formatting context.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::scan_chord chord, FormatContext& context) const
	{
		return std::ranges::copy(chord.name(), context.out()).out;
	}
};

/// Keycode formatter.
template <>
struct std::formatter<tr::keycode>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid keycode format specification."};
		}
		return context.begin();
	}

	/// Formats a keycode.
	/// @tparam FormatContext Formatting context type.
	/// @param key Keycode to format.
	/// @param context Formatting context.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::keycode key, FormatContext& context) const
	{
		return std::ranges::copy(name(key), context.out()).out;
	}
};

/// Key chord formatter.
template <>
struct std::formatter<tr::key_chord>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid key chord format specification."};
		}
		return context.begin();
	}

	/// Formats a key chord.
	/// @tparam FormatContext Formatting context type.
	/// @param chord Key chord to format.
	/// @param context Formatting context.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::key_chord chord, FormatContext& context) const
	{
		return std::ranges::copy(chord.name(), context.out()).out;
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
	/// @param os Output stream.
	/// @param in Input parameter.
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
	/// @param os Output stream.
	/// @param in Input parameter.
	void operator()(std::ostream& os, const tr::key_chord& in) const;
};