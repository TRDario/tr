///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements static_string.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../static_string.hpp"

////////////////////////////////////////////////////////////// STATIC STRING //////////////////////////////////////////////////////////////

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::static_string(size_type size, char chr)
	: m_size{size}
{
	std::fill(begin(), end(), chr);
}

template <tr::usize Capacity>
template <tr::usize Size>
constexpr tr::static_string<Capacity>::static_string(const char (&literal)[Size])
	: m_buffer{}, m_size{size_type(Size - 1)}
{
	static_assert(Size - 1 <= Capacity, "Tried to initialize a static string with a string literal that would be too long.");
	std::copy(literal, literal + Size - 1, m_buffer.begin());
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::static_string(std::string_view str)
	: m_buffer{}, m_size{size_type(str.size())}
{
	TR_ASSERT(str.size() <= Capacity, "Tried to copy a string of size {} into a static string of capacity {}.", str.size(), Capacity);
	std::ranges::copy(str, m_buffer.begin());
}

template <tr::usize Capacity>
template <std::convertible_to<std::string_view> String>
constexpr tr::static_string<Capacity>::static_string(String&& str)
	: static_string{std::string_view{str}}
{
}

//

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::operator std::string_view() const
{
	return {m_buffer.data(), m_size};
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::operator std::string() const
{
	return std::string{std::string_view{*this}};
}

//

template <tr::usize Capacity>
constexpr std::strong_ordering tr::operator<=>(const static_string<Capacity>& l, const static_string<Capacity>& r)
{
	return std::string_view{l} <=> std::string_view{r};
}

template <tr::usize Capacity> constexpr bool tr::operator==(const static_string<Capacity>& l, const static_string<Capacity>& r)
{
	return std::string_view{l} == std::string_view{r};
}

template <tr::usize Capacity> constexpr std::strong_ordering tr::operator<=>(const static_string<Capacity>& l, const std::string_view& r)
{
	return std::string_view{l} <=> r;
}

template <tr::usize Capacity> constexpr bool tr::operator==(const static_string<Capacity>& l, const std::string_view& r)
{
	return std::string_view{l} == r;
}

template <tr::usize Capacity> constexpr std::strong_ordering tr::operator<=>(const std::string_view& l, const static_string<Capacity>& r)
{
	return l <=> std::string_view{r};
}

template <tr::usize Capacity> constexpr bool tr::operator==(const std::string_view& l, const static_string<Capacity>& r)
{
	return l == std::string_view{r};
}

//

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::reference tr::static_string<Capacity>::operator[](size_type offset)
{
	TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in static string of size {}.", offset, m_size);

	return m_buffer[offset];
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_reference tr::static_string<Capacity>::operator[](size_type offset) const
{
	TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in static string of size {}.", offset, m_size);

	return m_buffer[offset];
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::reference tr::static_string<Capacity>::front()
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty static string.");

	return *begin();
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_reference tr::static_string<Capacity>::front() const
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty static string.");

	return *begin();
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::reference tr::static_string<Capacity>::back()
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty static string.");

	return *std::prev(end());
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_reference tr::static_string<Capacity>::back() const
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty static string.");

	return *std::prev(end());
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::pointer tr::static_string<Capacity>::data()
{
	return m_buffer.data();
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_pointer tr::static_string<Capacity>::data() const
{
	return m_buffer.data();
}

//

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::begin()
{
	return m_buffer.data();
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::begin() const
{
	return m_buffer.data();
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::cbegin() const
{
	return m_buffer.data();
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::end()
{
	return m_buffer.data() + m_size;
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::end() const
{
	return m_buffer.data() + m_size;
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::cend() const
{
	return m_buffer.data() + m_size;
}

//

template <tr::usize Capacity> constexpr bool tr::static_string<Capacity>::empty() const
{
	return m_size == 0;
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::size_type tr::static_string<Capacity>::size() const
{
	return m_size;
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::size_type tr::static_string<Capacity>::max_size()
{
	return Capacity;
}

//

template <tr::usize Capacity> constexpr void tr::static_string<Capacity>::clear()
{
	m_size = 0;
}

template <tr::usize Capacity> constexpr void tr::static_string<Capacity>::push_back(char chr)
{
	TR_ASSERT(m_size < Capacity, "Tried to push back into a static string that is already at its capacity of {}.", Capacity);
	m_buffer[m_size++] = chr;
}

template <tr::usize Capacity> constexpr void tr::static_string<Capacity>::append(char chr)
{
	push_back(chr);
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>& tr::static_string<Capacity>::operator+=(char chr)
{
	push_back(chr);
	return *this;
}

template <tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
constexpr void tr::static_string<Capacity>::append(Iterator begin, Iterator end)
{
	std::copy(begin, end, this->end());
	m_size = size_type(m_size + std::distance(begin, end));
}

template <tr::usize Capacity> constexpr void tr::static_string<Capacity>::append(std::string_view str)
{
	TR_ASSERT(m_size + str.size() <= Capacity, "Tried to do an append onto a static string that would put it past its capacity of {}.",
			  Capacity);

	std::ranges::copy(str, end());
	m_size = size_type(m_size + str.size());
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>& tr::static_string<Capacity>::operator+=(std::string_view str)
{
	append(str);
	return *this;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::insert(const_iterator where, char chr)
{
	TR_ASSERT(m_size < Capacity, "Tried to insert into a static string that is already at its capacity of {}.", Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	const iterator location{begin() + (where - begin())};
	std::copy_backward(location, end(), end() + 1);
	*location = chr;
	++m_size;
	return location;
}

template <tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::insert(const_iterator where, Iterator first, Iterator last)
{
	const usize size{usize(std::distance(first, last))};

	TR_ASSERT(m_size + size <= Capacity, "Tried to do an insert into a static string that would put it past its capacity of {}.", Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	const iterator mut_where{begin() + (where - begin())};
	std::copy_backward(mut_where, end(), end() + size);
	std::copy(first, last, mut_where);
	m_size = size_type(m_size + size);
	return mut_where;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::insert(const_iterator where, std::string_view str)
{
	TR_ASSERT(m_size + str.size() <= Capacity, "Tried to do an insert into a static string that would put it past its capacity of {}.",
			  Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	const iterator mut_where{begin() + (where - begin())};
	std::copy_backward(begin() + mut_where, end(), end() + str.size());
	std::ranges::copy(str, mut_where);
	m_size = size_type(m_size + str.size());
	return mut_where;
}

template <tr::usize Capacity> constexpr void tr::static_string<Capacity>::pop_back()
{
	if (!empty()) {
		--m_size;
	}
}

template <tr::usize Capacity> constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::erase(const_iterator where)
{
	TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to static_string::erase.");

	const iterator mut_where{begin() + (where - begin())};
	std::copy(mut_where + 1, end(), mut_where);
	--m_size;
	return mut_where;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::erase(const_iterator start, const_iterator end)
{
	TR_ASSERT(start >= begin() && start < this->end(), "Tried to pass an invalid start iterator to static_string::erase.");
	TR_ASSERT(end >= begin() && end <= this->end(), "Tried to pass an invalid end iterator to static_string::erase.");

	const iterator mut_start{begin() + (start - begin())};
	std::copy(mut_start + (end - start), this->end(), mut_start);
	m_size = size_type(m_size - (end - start));
	return mut_start;
}

template <tr::usize Capacity> constexpr void tr::static_string<Capacity>::resize(size_type size, char chr)
{
	TR_ASSERT(size <= Capacity, "Tried to resize a static string past its capacity of {}.", Capacity);

	for (size_type i = m_size; i < size; ++i) {
		m_buffer[i] = chr;
	}
	m_size = size;
}

//////////////////////////////////////////////////////////////////// IO ///////////////////////////////////////////////////////////////////

template <tr::usize Capacity>
void tr::binary_reader<tr::static_string<Capacity>>::operator()(std::istream& is, static_string<Capacity>& out) const
{
	out.resize(read_binary<typename static_string<Capacity>::size_type>(is));
	is.read(out.data(), out.size());
}

template <tr::usize Capacity>
void tr::binary_writer<tr::static_string<Capacity>>::operator()(std::ostream& os, const static_string<Capacity>& in) const
{
	write_binary(os, in.size());
	write_binary(os, std::span{in});
}

template <tr::usize Capacity> inline std::ostream& tr::operator<<(std::ostream& os, const static_string<Capacity>& str)
{
	return os << std::string_view{str};
}