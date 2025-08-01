#pragma once
#include "static_string.hpp"

namespace tr {
	// Datatype used for Unicode codepoints.
	using codepoint = std::uint32_t;
} // namespace tr

// Namespace containing UTF-8-related functionality.
namespace tr::utf8 {
	// Iterator types supported by the UTF-8 functions.
	template <class T>
	concept base_iterator = std::same_as<T, char*> || std::same_as<T, const char*> || std::same_as<T, std::string_view::iterator> ||
							std::same_as<T, std::string::iterator> || std::same_as<T, std::string::const_iterator>;
	// String types supported by the UTF-8 functions.
	template <class T>
	concept string = std::same_as<T, std::string> || std::same_as<T, static_string<T::max_size()>>;

	// Converts a UTF-8 sequence to a Unicode codepoint.
	template <base_iterator T> constexpr codepoint to_cp(T it);
	// Gets an iterator to the next character.
	template <base_iterator T> constexpr T next(T it);
	// Gets an iterator to the previous character.
	template <base_iterator T> constexpr T prev(T it);
	// Inserts a character into a UTF-8-encoded string.
	template <string String> constexpr String::iterator insert(String& str, typename String::iterator where, codepoint cp);
	// Erases a character from a UTF-8-encoded string.
	template <string String> constexpr String::iterator erase(String& str, typename String::iterator where);
	// Pops back a character from a UTF-8 encoded string.
	template <string String> constexpr void pop_back(String& str);

	// Unicode codepoint const iterator for UTF-8 string views.
	class const_it {
	  public:
		using value_type = codepoint;
		using difference_type = std::ptrdiff_t;

		constexpr const_it() = default;
		constexpr const_it(const char* ptr);

		constexpr friend auto operator<=>(const const_it&, const const_it&) = default;
		constexpr friend bool operator==(const const_it&, const const_it&) = default;

		constexpr codepoint operator*() const;

		constexpr const_it& operator++();
		constexpr const_it operator++(int);
		constexpr const_it& operator--();
		constexpr const_it operator--(int);

		// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		constexpr const char* base() const;

	  private:
		// Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};
	};
	using const_reverse_it = std::reverse_iterator<const_it>;

	//

	// Creates a beginning codepoint iterator for a UTF-8 string view.
	constexpr const_it begin(std::string_view str);
	// Creates an ending codepoint iterator for a UTF-8 UTF-8 string view.
	constexpr const_it end(std::string_view str);
	// Creates a reversed beginning codepoint iterator for a UTF-8 string view.
	constexpr const_reverse_it rbegin(std::string_view str);
	// Creates an reserved ending codepoint iterator for a UTF-8 UTF-8 string view.
	constexpr const_reverse_it rend(std::string_view str);
	// Creates a codepoint iterator range pair for a UTF-8 string view.
	constexpr std::ranges::subrange<const_it> range(std::string_view str);
	// Counts the number of unicode characters in a UTF-8 string view.
	constexpr std::size_t length(std::string_view str);
} // namespace tr::utf8

#include "utf8_impl.hpp"