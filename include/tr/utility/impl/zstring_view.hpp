///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements zstring_view.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../zstring_view.hpp"

//////////////////////////////////////////////////// BASIC NULL-TERMINATED STRING VIEW ////////////////////////////////////////////////////

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

template <typename CharT> constexpr bool tr::operator==(const CharT* l, zstring_view_end_sentinel)
{
	return l == nullptr || *l == CharT{0};
}

template <typename CharT, typename Traits>
constexpr bool tr::operator==(basic_zstring_view<CharT, Traits> l, std::type_identity_t<basic_zstring_view<CharT, Traits>> r)
{
	return std::ranges::equal(l, r);
}

template <typename CharT, typename Traits>
constexpr std::strong_ordering tr::operator<=>(basic_zstring_view<CharT, Traits> l,
											   std::type_identity_t<basic_zstring_view<CharT, Traits>> r)
{
	typename basic_zstring_view<CharT, Traits>::const_iterator l_it{l.begin()};
	typename basic_zstring_view<CharT, Traits>::const_iterator r_it{r.begin()};
	while (l_it != l.end() && r_it != r.end()) {
		const std::strong_ordering cmp{*l_it++ <=> *r_it++};
		if (cmp != std::strong_ordering::equal) {
			return cmp;
		}
	}
	return l_it != l.end() ? std::strong_ordering::greater : r_it != r.end() ? std::strong_ordering::less : std::strong_ordering::equal;
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::operator std::basic_string_view<CharT, Traits>() const
{
	return m_ptr;
}

//

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::iterator tr::basic_zstring_view<CharT, Traits>::begin() const
{
	return m_ptr;
}

template <typename CharT, typename Traits>
constexpr tr::basic_zstring_view<CharT, Traits>::const_iterator tr::basic_zstring_view<CharT, Traits>::cbegin() const
{
	return m_ptr;
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

template <typename CharT, typename Traits> constexpr tr::usize tr::basic_zstring_view<CharT, Traits>::length() const
{
	return std::string_view{*this}.length();
}

template <typename CharT, typename Traits> constexpr bool tr::basic_zstring_view<CharT, Traits>::empty() const
{
	return m_ptr == nullptr || *m_ptr == CharT{0};
}

//

template <typename CharT, typename Traits> constexpr void tr::basic_zstring_view<CharT, Traits>::remove_prefix(size_type n)
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
std::basic_ostream<CharT, Traits>& tr::operator<<(std::basic_ostream<CharT, Traits>& os, basic_zstring_view<CharT, Traits> v)
{
	return os << v.c_str();
}

//

template <typename CharT, typename Traits>
std::filesystem::path tr::operator/(const std::filesystem::path& l, basic_zstring_view<CharT, Traits> r)
{
	return l / std::filesystem::path{r.c_str()};
}