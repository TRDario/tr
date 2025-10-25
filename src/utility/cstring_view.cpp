#include "../../include/tr/utility/cstring_view.hpp"

tr::cstring_view::operator std::filesystem::path() const
{
	return m_cstr;
}

//

std::strong_ordering tr::operator<=>(cstring_view l, cstring_view r)
{
	int comp{std::strcmp(l, r)};
	if (comp < 0) {
		return std::strong_ordering::less;
	}
	else if (comp > 0) {
		return std::strong_ordering::greater;
	}
	else {
		return std::strong_ordering::equal;
	}
}

std::strong_ordering tr::operator<=>(cstring_view l, const char* r)
{
	return l <=> cstring_view{r};
}

std::strong_ordering tr::operator<=>(const char* l, cstring_view r)
{
	return cstring_view{l} <=> r;
}

bool tr::operator==(cstring_view l, cstring_view r)
{
	return l <=> r == std::strong_ordering::equal;
}

bool tr::operator==(const char* l, cstring_view r)
{
	return l <=> r == std::strong_ordering::equal;
}

bool tr::operator==(cstring_view l, const char* r)
{
	return l <=> r == std::strong_ordering::equal;
}

//

inline std::ostream& tr::operator<<(std::ostream& os, const cstring_view& str)
{
	return os << str.data();
}

std::filesystem::path tr::operator/(const std::filesystem::path& l, tr::cstring_view r)
{
	return l / std::filesystem::path{r};
}