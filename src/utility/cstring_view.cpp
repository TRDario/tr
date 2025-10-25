#include "../../include/tr/utility/cstring_view.hpp"

std::strong_ordering tr::operator<=>(const cstring_view& l, const cstring_view& r)
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

inline std::ostream& tr::operator<<(std::ostream& os, const cstring_view& str)
{
	return os << str.data();
}