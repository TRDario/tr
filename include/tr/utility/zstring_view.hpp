///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a null-terminated string view class template.                                                                                //
//                                                                                                                                       //
// Null-terminated string views can be default-constructed into an empty state or constructed from character pointers and strings:       //
//     - tr::zstring_view str{}; str.empty() -> true                                                                                     //
//     - tr::zstring_view str{"literal"}                                                                                                 //
//     - std::string str{"test"}; tr::zstring_view view{str}                                                                             //
//                                                                                                                                       //
// Null-terminated string views can be converted into standard string views.                                                             //
//                                                                                                                                       //
// Null-terminated string views can be iterated through or randomly accessed, and a raw pointer to their data may be taken with .data()  //
// or .c_str():                                                                                                                          //
//     - tr::zstring_view str{"test"}; for (char c : str) { ... }                                                                        //
//     - tr::zstring_view str{"test"}; str[0] -> 't'                                                                                     //
//     - tr::zstring_view str{"test"}; str.front() -> 't'                                                                                //
//     - tr::zstring_view str{"test"}; str.data() -> raw pointer to "test"                                                               //
//     - tr::zstring_view str{"test"}; str.c_str() -> raw pointer to "test"                                                              //
//                                                                                                                                       //
// Null-terminated string views can be checked for being empty:                                                                          //
//     - tr::zstring_view str{}; str.empty() -> true                                                                                     //
//     - tr::zstring_view str{"test"}; str.empty() -> false                                                                              //
//                                                                                                                                       //
// A prefix may be removed from a null-terminated string view, or a substring without a prefix can be taken:                             //
//     - tr::zstring_view str{"testTEST"}; str.remove_prefix(4) -> str == "TEST"                                                         //
//     - tr::zstring_view str{"testTEST"}; str.substr(4) -> "TEST"                                                                       //
//                                                                                                                                       //
// Much like std::string_view, tr::zstring_view can be compared with other strings, including regular C-strings. It can also be hashed,  //
// output with std::format or ostreams, binary-written, or used to concatenate paths:                                                    //
//     - tr::zstring_view str{"test"}; str == "test" -> true                                                                             //
//     - tr::zstring_view str{"test"}; std::format("{}", str) -> "test"                                                                  //
//     - tr::zstring_view str{"test"}; tr::write_binary(out, str) -> "test" written to out                                               //
//     - tr::zstring_view str{"bin"}; std::filesystem::path{"/usr"} / str -> std::filesystem::path{"/usr/bin"}                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// End sentinel for null-terminated string views.
	struct zstring_view_end_sentinel {};

	// Null-terminated string view.
	template <typename CharT, typename Traits = std::char_traits<CharT>> class basic_zstring_view {
	  public:
		using traits_type = Traits;
		using value_type = CharT;
		using pointer = CharT*;
		using const_pointer = const CharT*;
		using reference = CharT&;
		using const_reference = const CharT&;
		using const_iterator = const_pointer;
		using iterator = const_iterator;
		using end_sentinel = zstring_view_end_sentinel;
		using size_type = usize;
		using difference_type = ssize;

		// Constructs an empty string view.
		constexpr basic_zstring_view() = default;
		// Constructs a string view from a null-terminated string pointer.
		constexpr basic_zstring_view(const CharT* ptr);
		// Constructs a string view from a standard string.
		template <typename Allocator> constexpr basic_zstring_view(const std::basic_string<CharT, Traits, Allocator>& str);
		// basic_zstring_view is trivially copyable.
		constexpr basic_zstring_view(const basic_zstring_view&) noexcept = default;
		// basic_zstring_view is trivially movable.
		constexpr basic_zstring_view(basic_zstring_view&&) noexcept = default;

		// basic_zstring_view is trivially copyable.
		constexpr basic_zstring_view& operator=(const basic_zstring_view& r) noexcept = default;
		// basic_zstring_view is trivially movable.
		constexpr basic_zstring_view& operator=(basic_zstring_view&& r) noexcept = default;

		// Converts the null-terminated string view into a standard string view.
		constexpr operator std::basic_string_view<CharT, Traits>() const;

		// Gets an iterator to the beginning of the string.
		constexpr iterator begin() const;
		// Gets an iterator to the beginning of the string.
		constexpr const_iterator cbegin() const;
		// Gets an iterator sentinel to the end of the string.
		constexpr end_sentinel end() const;
		// Gets an iterator sentinel to the end of the string.
		constexpr end_sentinel cend() const;

		// Accesses a specific character.
		constexpr const_reference operator[](size_type index) const;
		// Accesses a specific character.
		constexpr const_reference at(size_type index) const;
		// Accesses the first character in the string.
		constexpr const_reference front() const;
		// Returns a pointer to the first character in the string.
		constexpr const_pointer data() const;
		// Returns a pointer to the first character in the string.
		constexpr const_pointer c_str() const;

		// Gets the length of the string as if by strlen.
		constexpr usize length() const;
		// Gets whether the string view is empty.
		constexpr bool empty() const;

		// Moves the start of the view forward.
		constexpr void remove_prefix(size_type n);

		// Creates a sub-string view.
		constexpr basic_zstring_view substr(size_type pos = 0) const;

	  private:
		// Pointer to the null-terminated string.
		const_pointer m_ptr{nullptr};
	};
	// Null-terminated 8-bit character string view.
	using zstring_view = basic_zstring_view<char>;

	inline namespace literals {
		inline namespace string_view_literals {
			// Constructs a null-terminated string view.
			constexpr zstring_view operator""_zsv(const char* str, std::size_t len) noexcept;
		} // namespace string_view_literals
	} // namespace literals

	// Compares a null-terminated string view iterator to an end sentinel.
	template <typename CharT> constexpr bool operator==(const CharT* l, zstring_view_end_sentinel);
	// String equality comparison.
	template <typename CharT, typename Traits>
	constexpr bool operator==(basic_zstring_view<CharT, Traits> l, std::type_identity_t<basic_zstring_view<CharT, Traits>> r);
	// Three-way string comparison.
	template <typename CharT, typename Traits>
	constexpr std::strong_ordering operator<=>(basic_zstring_view<CharT, Traits> l,
											   std::type_identity_t<basic_zstring_view<CharT, Traits>> r);

	// Output stream formatter for null-terminated string views.
	template <typename CharT, typename Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, basic_zstring_view<CharT, Traits> v);

	// Path construction operator for null-terminated string views.
	template <typename CharT, typename Traits>
	std::filesystem::path operator/(const std::filesystem::path& l, basic_zstring_view<CharT, Traits> r);
} // namespace tr

// Designates basic_zstring_view as a view.
template <typename CharT, typename Traits> inline constexpr bool std::ranges::enable_view<tr::basic_zstring_view<CharT, Traits>> = true;
// Designates basic_zstring_view as a borrowed range.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_borrowed_range<tr::basic_zstring_view<CharT, Traits>> = true;

// Formatter for null-terminated string views.
template <typename CharT, typename Traits>
struct TR_FMT::formatter<tr::basic_zstring_view<CharT, Traits>> : TR_FMT::formatter<std::basic_string_view<CharT, Traits>> {};

// Null-terminated string view hasher.
template <typename CharT, typename Traits>
struct boost::hash<tr::basic_zstring_view<CharT, Traits>> : boost::hash<std::basic_string_view<CharT, Traits>> {};

// Null-terminated string view binary writer.
template <typename CharT, typename Traits>
struct tr::binary_writer<tr::basic_zstring_view<CharT, Traits>> : binary_writer<std::basic_string_view<CharT, Traits>> {};

#include "impl/zstring_view.hpp"