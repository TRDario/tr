///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functionality for handling UTF-8 char strings.                                                                               //
//                                                                                                                                       //
// tr::codepoint defines a Unicode codepoint (as a 32-bit unsigned integer value), tr::utf8::base_iterator defines an iterator type      //
// supported by the UTF-8 functions (char pointers and iterators to std::string_view or std::string), and tr::utf8::string defines a     //
// string type supported by the UTF-8 functions (std::string, tr::static_string).                                                        //
//                                                                                                                                       //
// The functions tr::utf8::prev and tr::utf8::next allow iterating backward and forward one or multiple steps between Unicode characters //
// in a UTF-8 string:                                                                                                                    //
//     Let ABCCDDDEFFGH be a UTF-8 string; 'CC', 'DDD', 'FF' are multibyte-encoded characters.                                           //
//     - 'it' points to AB[C]CDDDEFFGH; it = tr::utf8::next(it) -> 'it' now points to ABCC[D]DDEFFGH                                     //
//     - 'it' points to ABCC[D]DDEFFGH; it = tr::utf8::next(it, 3) -> 'it' now points to ABCCDDDEFF[G]H                                  //
//     - 'it' points to ABCCDDDEFF[G]H; it = tr::utf8::prev(it) -> 'it' now points to ABCCDDDE[F]FGH                                     //
//     - 'it' points to ABCCDDDE[F]FGH; it = tr::utf8::prev(it, 2) -> 'it' now points to ABCC[D]DDEFFGH                                  //
//                                                                                                                                       //
// The function tr::utf8::to_cp allows converting a UTF-8 sequence to a character codepoint, while the function tr::utf8::insert allows  //
// insertion of a Unicode character into a UTF-8 string. The functions tr::utf8::erase and tr::utf8::pop_back allow for removing one or  //
// more Unicode characters from a string:                                                                                                //
//     Let str = ABCCDDDEFFGH be a UTF-8 string; a, b, c, ... are Unicode characters while A, B, CC, ... are their UTF-8 representations.//
//     - 'it' points to ABCC[D]DDEFFGH; tr::utf8::to_cp(it) -> 'd'                                                                       //
//     - tr::utf8::insert(str, it, 'f') -> str is now ABCCFFDDDEFFGH                                                                     //
//     - tr::utf8::erase(str, it, 3) -> str is now ABCCFFGH                                                                              //
//     - tr::utf8::pop_back(str) -> str is now ABCCFFG                                                                                   //
//                                                                                                                                       //
// tr::utf8::const_it provides a bidirectional iterator interface over UTF-8 strings, allowing the appearance of a codepoint range,      //
// while tr::utf8::const_reverse_it is the equivalent reverse iterator.                                                                  //
// Iterators of these types can be gotten through tr::utf8::begin, tr::utf8::rbegin, tr::utf8::end, and tr::utf8::rend, while            //
// tr::utf8::range creates a codepoint view over a string, equivalent to std::ranges::subrange{tr::utf8::begin(str), tr::utf8::end(str)}://
//     Let str = ABCCDDDEFFGH be a UTF-8 string; a, b, c, ... are Unicode characters while A, B, CC, ... are their UTF-8 representations.//
//     - *tr::utf8::begin(str) -> 'a'                                                                                                    //
//     - *tr::utf8::rbegin(str) -> 'h'                                                                                                   //
//     - *std::prev(tr::utf8::end(str), 2) -> 'g'                                                                                        //
//     - *std::prev(tr::utf8::rend(str), 2) -> 'b'                                                                                       //
//     - tr::utf8::range(str) -> 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'                                                                  //
//                                                                                                                                       //
// The number of Unicode characters in a string can be gotten with tr::utf8::length:                                                     //
//     Let str = ABCCDDDEFFGH be a UTF-8 string; 'CC', 'DDD', 'FF' are multibyte-encoded characters.                                     //
//     - str.size(); tr::utf8::length(str) -> 12; 8                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "static_string.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Datatype used for Unicode codepoints.
	using codepoint = u32;
} // namespace tr

// Namespace containing UTF-8-related functionality.
namespace tr::utf8 {
	// Iterator types supported by the UTF-8 functions.
	template <class T>
	concept base_iterator = one_of<T, char*, const char*, std::string_view::iterator, std::string::iterator, std::string::const_iterator>;

	// String types supported by the UTF-8 functions.
	template <class T>
	concept string = std::same_as<T, std::string> || specialization_of_v<T, static_string>;

	// Converts a UTF-8 sequence to a Unicode codepoint.
	template <base_iterator T> constexpr codepoint to_cp(T it);
	// Gets an iterator to the next character.
	template <base_iterator T> constexpr T next(T it);
	// Gets an iterator to the nth next character.
	template <base_iterator T> constexpr T next(T it, usize n);
	// Gets an iterator to the previous character.
	template <base_iterator T> constexpr T prev(T it);
	// Gets an iterator to the nth previous character.
	template <base_iterator T> constexpr T prev(T it, usize n);
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
		using difference_type = ssize;

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
	// Unicode codepoint const reverse iterator for UTF-8 string views.
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
	constexpr usize length(std::string_view str);
} // namespace tr::utf8

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "utf8_impl.hpp" // IWYU pragma: keep