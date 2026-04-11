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
// tr::utf8::iterator provides a bidirectional iterator interface over UTF-8 strings, allowing the appearance of a codepoint range,      //
// while tr::utf8::reverse_iterator is the equivalent reverse iterator. Indexed versions of these iterators exist as                     //
// tr::utf8::indexed_iterator and tr::utf8::reverse_indexed_iterator. Iterators of these types can be gotten through tr::utf8::begin,    //
// tr::utf8::ibegin, tr::utf8::rbegin, tr::utf8::ribegin, tr::utf8::end, tr::utf8::iend, tr::utf8::rend, and tr::utf8::riend, while      //
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
	template <typename T>
	concept base_iterator = one_of<T, char*, const char*, std::string_view::iterator, std::string::iterator, std::string::const_iterator>;

	// String types supported by the UTF-8 functions.
	template <typename T>
	concept string = std::same_as<T, std::string> || specialization_of_v<T, static_string>;

	// Converts a UTF-8 sequence to a Unicode codepoint.
	template <base_iterator Iterator> constexpr codepoint to_cp(Iterator it);
	// Gets an iterator to the next character.
	template <base_iterator Iterator> constexpr Iterator next(Iterator it);
	// Gets an iterator to the nth next character.
	template <base_iterator Iterator> constexpr Iterator next(Iterator it, usize n);
	// Gets an iterator to the previous character.
	template <base_iterator Iterator> constexpr Iterator prev(Iterator it);
	// Gets an iterator to the nth previous character.
	template <base_iterator Iterator> constexpr Iterator prev(Iterator it, usize n);
	// Inserts a character into a UTF-8-encoded string.
	template <string String> constexpr String::iterator insert(String& str, typename String::iterator where, codepoint cp);
	// Erases a character from a UTF-8-encoded string.
	template <string String> constexpr String::iterator erase(String& str, typename String::iterator where);
	// Pops back a character from a UTF-8 encoded string.
	template <string String> constexpr void pop_back(String& str);

	// Unicode codepoint iterator for UTF-8 string views.
	class iterator {
	  public:
		using value_type = codepoint;
		using difference_type = ssize;

		constexpr iterator() = default;
		constexpr iterator(const char* ptr);

		constexpr friend auto operator<=>(const iterator&, const iterator&) = default;
		constexpr friend bool operator==(const iterator&, const iterator&) = default;

		constexpr codepoint operator*() const;

		constexpr iterator& operator++();
		constexpr iterator operator++(int);
		constexpr iterator& operator--();
		constexpr iterator operator--(int);

		// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		constexpr const char* base() const;

	  private:
		// Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};
	};
	// Unicode codepoint const reverse iterator for UTF-8 string views.
	using reverse_iterator = std::reverse_iterator<iterator>;

	// Indexed Unicode codepoint iterator for UTF-8 string views.
	class indexed_iterator {
	  public:
		using value_type = codepoint;
		using difference_type = ssize;

		constexpr indexed_iterator() = default;
		constexpr indexed_iterator(const char* ptr, ssize index);

		constexpr friend auto operator<=>(const indexed_iterator& l, const indexed_iterator& r);
		constexpr friend bool operator==(const indexed_iterator& l, const indexed_iterator& r);

		constexpr codepoint operator*() const;

		constexpr indexed_iterator& operator++();
		constexpr indexed_iterator operator++(int);
		constexpr indexed_iterator& operator--();
		constexpr indexed_iterator operator--(int);

		// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		constexpr const char* base() const;
		// The index of the iterator.
		constexpr ssize index() const;

	  private:
		// Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};
		// The index of the iterator.
		ssize m_index{0};
	};
	// Unicode codepoint const reverse iterator for UTF-8 string views.
	using reverse_indexed_iterator = std::reverse_iterator<indexed_iterator>;

	//

	// Creates a beginning codepoint iterator for a UTF-8 string view.
	constexpr iterator begin(std::string_view str);
	// Creates an indexed beginning codepoint iterator for a UTF-8 string view.
	constexpr indexed_iterator ibegin(std::string_view str);
	// Creates an ending codepoint iterator for a UTF-8 string view.
	constexpr iterator end(std::string_view str);
	// Creates an indexed ending codepoint iterator for a UTF-8 string view.
	constexpr indexed_iterator iend(std::string_view str);
	// Creates an indexed reversed beginning codepoint iterator for a UTF-8 string view.
	constexpr reverse_iterator rbegin(std::string_view str);
	// Creates a reversed beginning codepoint iterator for a UTF-8 string view.
	constexpr reverse_indexed_iterator ribegin(std::string_view str);
	// Creates an indexed reversed ending codepoint iterator for a UTF-8 string view.
	constexpr reverse_iterator rend(std::string_view str);
	// Creates a reversed ending codepoint iterator for a UTF-8 string view.
	constexpr reverse_indexed_iterator riend(std::string_view str);
	// Creates a codepoint iterator range pair for a UTF-8 string view.
	constexpr std::ranges::subrange<iterator> range(std::string_view str);
	// Counts the number of unicode characters in a UTF-8 string view.
	constexpr usize length(std::string_view str);
} // namespace tr::utf8

#include "impl/utf8.hpp" // IWYU pragma: export