#pragma once
#include "macro.hpp"
#include "static_string.hpp"

template <std::size_t S>
constexpr tr::static_string<S>::static_string(size_type size, char chr)
	: m_size{size}
{
	std::fill(begin(), end(), chr);
}

template <std::size_t S>
template <std::size_t S1>
constexpr tr::static_string<S>::static_string(const char (&literal)[S1])
	: m_buffer{}, m_size{static_cast<size_type>(S1 - 1)}
{
	static_assert(S1 - 1 <= S, "Tried to initialize a static string with a stirng literal that would be too long.");
	std::copy(literal, literal + S1 - 1, m_buffer.begin());
}

template <std::size_t S>
constexpr tr::static_string<S>::static_string(std::string_view str)
	: m_buffer{}, m_size{static_cast<size_type>(str.size())}
{
	TR_ASSERT(str.size() <= S, "Tried to copy a string of size {} into a static string of capacity {}.", str.size(), S);
	std::ranges::copy(str, m_buffer.begin());
}

//

template <std::size_t S> constexpr tr::static_string<S>::operator std::string_view() const
{
	return {m_buffer.data(), m_size};
}

template <std::size_t S> constexpr tr::static_string<S>::operator std::string() const
{
	return std::string{std::string_view{*this}};
}

//

template <std::size_t S> constexpr std::strong_ordering tr::operator<=>(const static_string<S>& l, const static_string<S>& r)
{
	return std::string_view{l} <=> std::string_view{r};
}

template <std::size_t S> constexpr bool tr::operator==(const static_string<S>& l, const static_string<S>& r)
{
	return std::string_view{l} == std::string_view{r};
}

template <std::size_t S> constexpr std::strong_ordering tr::operator<=>(const static_string<S>& l, const std::string_view& r)
{
	return std::string_view{l} <=> r;
}

template <std::size_t S> constexpr bool tr::operator==(const static_string<S>& l, const std::string_view& r)
{
	return std::string_view{l} == r;
}

template <std::size_t S> constexpr std::strong_ordering tr::operator<=>(const std::string_view& l, const static_string<S>& r)
{
	return l <=> std::string_view{r};
}

template <std::size_t S> constexpr bool tr::operator==(const std::string_view& l, const static_string<S>& r)
{
	return l == std::string_view{r};
}

//

template <std::size_t S> constexpr tr::static_string<S>::reference tr::static_string<S>::operator[](size_type offset)
{
	TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in static string of size {}.", offset, m_size);

	return m_buffer[offset];
}

template <std::size_t S> constexpr tr::static_string<S>::const_reference tr::static_string<S>::operator[](size_type offset) const
{
	TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in static string of size {}.", offset, m_size);

	return m_buffer[offset];
}

template <std::size_t S> constexpr tr::static_string<S>::reference tr::static_string<S>::front()
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty static string.");

	return *begin();
}

template <std::size_t S> constexpr tr::static_string<S>::const_reference tr::static_string<S>::front() const
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty static string.");

	return *begin();
}

template <std::size_t S> constexpr tr::static_string<S>::reference tr::static_string<S>::back()
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty static string.");

	return *std::prev(end());
}

template <std::size_t S> constexpr tr::static_string<S>::const_reference tr::static_string<S>::back() const
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty static string.");

	return *std::prev(end());
}

template <std::size_t S> constexpr tr::static_string<S>::pointer tr::static_string<S>::data()
{
	return m_buffer.data();
}

template <std::size_t S> constexpr tr::static_string<S>::const_pointer tr::static_string<S>::data() const
{
	return m_buffer.data();
}

//

template <std::size_t S> constexpr tr::static_string<S>::iterator tr::static_string<S>::begin()
{
	return m_buffer.data();
}

template <std::size_t S> constexpr tr::static_string<S>::const_iterator tr::static_string<S>::begin() const
{
	return m_buffer.data();
}

template <std::size_t S> constexpr tr::static_string<S>::const_iterator tr::static_string<S>::cbegin() const
{
	return m_buffer.data();
}

template <std::size_t S> constexpr tr::static_string<S>::iterator tr::static_string<S>::end()
{
	return m_buffer.data() + m_size;
}

template <std::size_t S> constexpr tr::static_string<S>::const_iterator tr::static_string<S>::end() const
{
	return m_buffer.data() + m_size;
}

template <std::size_t S> constexpr tr::static_string<S>::const_iterator tr::static_string<S>::cend() const
{
	return m_buffer.data() + m_size;
}

//

template <std::size_t S> constexpr bool tr::static_string<S>::empty() const
{
	return m_size == 0;
}

template <std::size_t S> constexpr tr::static_string<S>::size_type tr::static_string<S>::size() const
{
	return m_size;
}

template <std::size_t S> constexpr tr::static_string<S>::size_type tr::static_string<S>::max_size()
{
	return S;
}

//

template <std::size_t S> constexpr void tr::static_string<S>::clear()
{
	m_size = 0;
}

template <std::size_t S> constexpr void tr::static_string<S>::push_back(char chr)
{
	TR_ASSERT(m_size < S, "Tried to push back into a static string that is already at its capacity of {}.", S);
	m_buffer[m_size++] = chr;
}

template <std::size_t S> constexpr void tr::static_string<S>::append(char chr)
{
	push_back(chr);
}

template <std::size_t S> constexpr tr::static_string<S>& tr::static_string<S>::operator+=(char chr)
{
	push_back(chr);
	return *this;
}

template <std::size_t S>
template <std::input_iterator It>
	requires(std::same_as<typename std::iterator_traits<It>::value_type, char>)
constexpr void tr::static_string<S>::append(It begin, It end)
{
	std::copy(begin, end, this->end());
	m_size = static_cast<size_type>(m_size + std::distance(begin, end));
}

template <std::size_t S> constexpr void tr::static_string<S>::append(std::string_view str)
{
	TR_ASSERT(m_size + str.size() <= S, "Tried to do an append onto a static string that would put it past its capacity of {}.", S);

	std::ranges::copy(str, end());
	m_size = static_cast<size_type>(m_size + str.size());
}

template <std::size_t S> constexpr tr::static_string<S>& tr::static_string<S>::operator+=(std::string_view str)
{
	append(str);
	return *this;
}

template <std::size_t S> constexpr void tr::static_string<S>::insert(const_iterator where, char chr)
{
	TR_ASSERT(m_size < S, "Tried to insert into a static string that is already at its capacity of {}.", S);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	std::copy_backward(begin() + (where - begin()), end(), end() + 1);
	*(begin() + (where - begin())) = chr;
	++m_size;
}

template <std::size_t S>
template <std::input_iterator It>
	requires(std::same_as<typename std::iterator_traits<It>::value_type, char>)
constexpr void tr::static_string<S>::insert(const_iterator where, It first, It last)
{
	const auto size{std::distance(first, last)};

	TR_ASSERT(m_size + size <= S, "Tried to do an insert into a static string that would put it past its capacity of {}.", S);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	std::copy_backward(begin() + (where - begin()), end(), end() + size);
	std::copy(first, last, begin() + (where - begin()));
	m_size = static_cast<size_type>(m_size + size);
}

template <std::size_t S> constexpr void tr::static_string<S>::insert(const_iterator where, std::string_view str)
{
	TR_ASSERT(m_size + str.size() <= S, "Tried to do an insert into a static string that would put it past its capacity of {}.", S);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	std::copy_backward(begin() + (where - begin()), end(), end() + str.size());
	std::ranges::copy(str, begin() + (where - begin()));
	m_size = static_cast<size_type>(m_size + str.size());
}

template <std::size_t S> constexpr void tr::static_string<S>::pop_back()
{
	if (m_size > 0) {
		--m_size;
	}
}

template <std::size_t S> constexpr void tr::static_string<S>::erase(const_iterator where)
{
	TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to static_string::erase.");

	std::copy(begin() + (where - begin()) + 1, end(), begin() + (where - begin()));
	--m_size;
}

template <std::size_t S> constexpr void tr::static_string<S>::erase(const_iterator start, const_iterator end)
{
	TR_ASSERT(start >= begin() && start < this->end(), "Tried to pass an invalid start iterator to static_string::erase.");
	TR_ASSERT(end >= begin() && end <= this->end(), "Tried to pass an invalid end iterator to static_string::erase.");

	std::copy(begin() + (start - begin()) + (end - start), this->end(), begin() + (start - begin()));
	m_size = static_cast<size_type>(m_size - (end - start));
}

template <std::size_t S> constexpr void tr::static_string<S>::resize(size_type size, char chr)
{
	TR_ASSERT(size <= S, "Tried to resize a static string past its capacity of {}.", S);

	for (size_type i = m_size; i < size; ++i) {
		m_buffer[i] = chr;
	}
	m_size = size;
}

//

template <std::size_t S> void tr::binary_reader<tr::static_string<S>>::read_from_stream(std::istream& is, static_string<S>& out)
{
	out.resize(binary_read<typename static_string<S>::size_type>(is));
	is.read(out.data(), out.size());
}

template <std::size_t S>
std::span<const std::byte> tr::binary_reader<tr::static_string<S>>::read_from_span(std::span<const std::byte> span, static_string<S>& out)
{
	out.resize(binary_read<typename static_string<S>::size_type>(span));
	return tr::binary_reader<std::span<char>>::read_from_span(span, std::span{out});
}

template <std::size_t S> void tr::binary_writer<tr::static_string<S>>::write_to_stream(std::ostream& os, const static_string<S>& in)
{
	binary_write(os, in.size());
	binary_write(os, std::span{in});
}

template <std::size_t S>
std::span<std::byte> tr::binary_writer<tr::static_string<S>>::write_to_span(std::span<std::byte> span, const static_string<S>& in)
{
	span = binary_write(span, in.size());
	span = binary_write(span, std::span{in});
}

//

template <std::size_t S> inline std::ostream& tr::operator<<(std::ostream& os, const static_string<S>& str)
{
	return os << std::string_view{str};
}