/// @file
/// @brief Implements zstring_view.hpp.

#pragma once
#include "../zstring_view.hpp"

//

/// Designates basic_zstring_view as a view.
/// @tparam CharT Character type used by the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_view<tr::basic_zstring_view<CharT, Traits>> = true;

/// Designates basic_zstring_view as a borrowed range.
/// @tparam CharT Character type used by the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_borrowed_range<tr::basic_zstring_view<CharT, Traits>> = true;

//

/// Formatter for NUL-terminated string views.
/// @tparam CharT Character type used by the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, typename Traits>
struct std::formatter<tr::basic_zstring_view<CharT, Traits>> : std::formatter<std::basic_string_view<CharT, Traits>>
{
};

/// NUL-terminated string view hasher.
/// @tparam CharT Character type used by the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, typename Traits>
struct boost::hash<tr::basic_zstring_view<CharT, Traits>> : boost::hash<std::basic_string_view<CharT, Traits>>
{
};

/// NUL-terminated string view binary writer.
/// @tparam CharT Character type used by the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, typename Traits>
struct tr::binary_writer<tr::basic_zstring_view<CharT, Traits>> : binary_writer<std::basic_string_view<CharT, Traits>>
{
};

//

template <typename CharT, typename Traits>
constexpr bool tr::basic_zstring_view<CharT, Traits>::iterator::operator==(end_sentinel) const
{
	return *this == iterator{} || Traits::eq(**this, CharT{});
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::basic_zstring_view(const CharT* ptr)
	: m_ptr{ptr}
{
}

template <typename CharT, typename Traits>
template <typename Allocator>
constexpr tr::basic_zstring_view<CharT, Traits>::basic_zstring_view(const std::basic_string<CharT, Traits, Allocator>& str)
	: m_ptr{str.c_str()}
{
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::operator std::basic_string_view<CharT, Traits>() const
{
	return !empty() ? m_ptr : std::string_view{};
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::iterator tr::basic_zstring_view<CharT, Traits>::begin() const
{
	return iterator{m_ptr};
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_iterator tr::basic_zstring_view<CharT, Traits>::cbegin() const
{
	return const_iterator{m_ptr};
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::end_sentinel tr::basic_zstring_view<CharT, Traits>::end() const
{
	return {};
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::end_sentinel tr::basic_zstring_view<CharT, Traits>::cend() const
{
	return {};
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_reference tr::basic_zstring_view<CharT, Traits>::operator[](size_type index) const
{
	return m_ptr[index];
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_reference tr::basic_zstring_view<CharT, Traits>::at(size_type index) const
{
	return m_ptr[index];
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_reference tr::basic_zstring_view<CharT, Traits>::front() const
{
	return *m_ptr;
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_pointer tr::basic_zstring_view<CharT, Traits>::data() const
{
	return m_ptr;
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_pointer tr::basic_zstring_view<CharT, Traits>::c_str() const
{
	return m_ptr;
}

//

template <typename CharT, typename Traits>
constexpr tr::usize tr::basic_zstring_view<CharT, Traits>::length() const
{
	return std::string_view{*this}.length();
}

template <typename CharT, typename Traits>
constexpr bool tr::basic_zstring_view<CharT, Traits>::empty() const
{
	return m_ptr == nullptr || Traits::eq(*m_ptr, CharT{});
}

//

template <typename CharT, typename Traits>
constexpr void tr::basic_zstring_view<CharT, Traits>::remove_prefix(size_type n)
{
	m_ptr += n;
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits> tr::basic_zstring_view<CharT, Traits>::substr(size_type pos) const
{
	return m_ptr + pos;
}

//

constexpr tr::zstring_view tr::operator""_zsv(const char* str, std::size_t) noexcept
{
	return str;
}

//

template <typename CharT, typename Traits>
constexpr std::strong_ordering tr::basic_zstring_view<CharT, Traits>::operator<=>(this basic_zstring_view lhs,
																				  std::type_identity_t<basic_zstring_view> rhs)
{
	typename basic_zstring_view<CharT, Traits>::const_iterator lhs_it{lhs.begin()};
	typename basic_zstring_view<CharT, Traits>::const_iterator rhs_it{rhs.begin()};
	while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
		const bool equal{Traits::eq(*lhs_it, *rhs_it)};
		if (!equal) {
			const bool lt{Traits::lt(*lhs_it, *rhs_it)};
			return lt ? std::strong_ordering::less : std::strong_ordering::greater;
		}
		++lhs_it, ++rhs_it;
	}
	return lhs_it != lhs.end()   ? std::strong_ordering::greater
		   : rhs_it != rhs.end() ? std::strong_ordering::less
								 : std::strong_ordering::equal;
}

template <typename CharT, typename Traits>
constexpr bool tr::basic_zstring_view<CharT, Traits>::operator==(this basic_zstring_view lhs, std::type_identity_t<basic_zstring_view> rhs)
{
	typename basic_zstring_view<CharT, Traits>::const_iterator lhs_it{lhs.begin()};
	typename basic_zstring_view<CharT, Traits>::const_iterator rhs_it{rhs.begin()};
	while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
		const bool equal{Traits::eq(*lhs_it++, *rhs_it++)};
		if (!equal) {
			return false;
		}
	}
	return lhs_it == lhs.end() && rhs_it == rhs.end();
}

//

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& tr::operator<<(std::basic_ostream<CharT, Traits>& os, basic_zstring_view<CharT, Traits> zstr)
{
	return os << zstr.c_str();
}

//

template <typename CharT, typename Traits>
std::filesystem::path tr::operator/(const std::filesystem::path& lhs, basic_zstring_view<CharT, Traits> rhs)
{
	return lhs / std::filesystem::path{rhs.c_str()};
}