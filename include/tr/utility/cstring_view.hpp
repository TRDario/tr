///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a wrapper representing a view over a C-string.                                                                               //
//                                                                                                                                       //
// Some functions in the library are built on C library functions that expect NUL-terminated string pointers. Exposing this requirement  //
// by requiring the user to pass a const char* directly is an option, but is ugly when dealing with std::string because .c_str() has to  //
// be manually called. tr::cstring_view essentially serves as a thin layer to provide a neater interface for cases like these:           //
//     - WITHOUT: tr::sys::set_window_title(std::format("Example {}", counter).c_str())                                                  //
//     - WITH: tr::sys::set_window_title(std::format("Example {}", counter))                                                             //
//                                                                                                                                       //
// A default-constructed C-string view, or one constructed with a nullptr, is considered "empty" and cannot be used for most other       //
// provided functionality (comparison, conversion to std::string, writing). A C-string can be checked for this with the .empty() method: //
//     - cstring_view{}.empty() -> true                                                                                                  //
//     - cstring_view{"example"}.empty() -> false                                                                                        //
//                                                                                                                                       //
// tr::cstring_view is implicitly convertible to const char* and std::string_view, and can be explicitly converted to std::string and    //
// std::filesystem::path:                                                                                                                //
//     - tr::cstring_view str{"test"}; std::strlen(test) -> 4                                                                            //
//     - tr::cstring_view str{"text.txt"}; tr::read_file_w(tr::filesystem::path{str}) -> works as expected                               //
//                                                                                                                                       //
// Much like std::string_view, tr::cstring_view can be compared with other strings, including regular C-strings. It can also be hashed,  //
// output with std::format or ostreams, binary-written, or used to concatenate paths:                                                    //
//     - tr::cstring_view str{"test"}; str == "test" -> true                                                                             //
//     - tr::cstring_view str{"test"}; std::format("{}", str) -> "test"                                                                  //
//     - tr::cstring_view str{"test"}; tr::write_binary(out, str) -> "test" written to out                                               //
//     - tr::cstring_view str{"bin"}; std::filesystem::path{"/usr"} / str -> std::filesystem::path{"/usr/bin"}                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Minimal wrapper representing a view over a C-string.
	class cstring_view {
	  public:
		// Constructs an empty view.
		constexpr cstring_view() = default;
		// Constructs a view from a C-string.
		constexpr cstring_view(const char* cstr);
		// Constructs a view from a standard string.
		constexpr cstring_view(const std::string& str);

		// Gets whether the view points to a string.
		// An empty cstring_view cannot be converted to a string view or string!
		constexpr bool empty() const;

		// Gets a pointer to the data of the string.
		constexpr operator const char*() const;
		// Gets a view over the string.
		constexpr operator std::string_view() const;
		// Creates a new string from the view.
		constexpr explicit operator std::string() const;
		// Creates a path from the view.
		explicit operator std::filesystem::path() const;

	  private:
		// Pointer to the character data.
		const char* m_cstr{nullptr};
	};

	// Compares two C-string views lexicographically.
	std::strong_ordering operator<=>(cstring_view l, cstring_view r);
	// Compares two C-string views lexicographically.
	std::strong_ordering operator<=>(const char* l, cstring_view r);
	// Compares two C-string views lexicographically.
	std::strong_ordering operator<=>(cstring_view l, const char* r);
	// Compares two C-string views for equality.
	bool operator==(cstring_view l, cstring_view r);
	// Compares two C-string views for equality.
	bool operator==(const char* l, cstring_view r);
	// Compares two C-string views for equality.
	bool operator==(cstring_view l, const char* r);

	// Output stream formatter for C-string views.
	std::ostream& operator<<(std::ostream& os, const cstring_view& str);
	// Path construction operator for C-string views.
	std::filesystem::path operator/(const std::filesystem::path& l, cstring_view r);

	// Static string binary writer.
	template <> struct binary_writer<cstring_view> : binary_writer<std::string_view> {};
} // namespace tr

// Formatter for static strings.
template <> struct TR_FMT::formatter<tr::cstring_view> : TR_FMT::formatter<const char*> {};

// C-string hasher.
template <> struct std::hash<tr::cstring_view> : std::hash<std::string_view> {};

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

constexpr tr::cstring_view::cstring_view(const char* cstr)
	: m_cstr{cstr}
{
}

constexpr tr::cstring_view::cstring_view(const std::string& str)
	: cstring_view{str.c_str()}
{
}

//

constexpr bool tr::cstring_view::empty() const
{
	return m_cstr == nullptr;
}

//

constexpr tr::cstring_view::operator const char*() const
{
	return m_cstr;
}

constexpr tr::cstring_view::operator std::string_view() const
{
	TR_ASSERT(!empty(), "Cannot convert an empty C-string view to a string view.");

	return m_cstr;
}

constexpr tr::cstring_view::operator std::string() const
{
	TR_ASSERT(!empty(), "Cannot convert an empty C-string view to a string.");

	return m_cstr;
}