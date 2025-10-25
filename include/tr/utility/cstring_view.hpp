#pragma once
#include "binary_io.hpp"

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
		constexpr const char* data() const;
		// Gets a pointer to the data of the string.
		constexpr operator const char*() const;

		// Gets a view over the string.
		constexpr operator std::string_view() const;

		// Creates a new string from the view.
		constexpr explicit operator std::string() const;

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

	// Output stream formatter for static strings.
	std::ostream& operator<<(std::ostream& os, const cstring_view& str);

	// Static string binary writer.
	template <> struct binary_writer<cstring_view> : binary_writer<std::string_view> {};
} // namespace tr

// Formatter for static strings.
template <> struct TR_FMT::formatter<tr::cstring_view> : TR_FMT::formatter<const char*> {};

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

constexpr const char* tr::cstring_view::data() const
{
	return m_cstr;
}

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