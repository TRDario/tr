/// @file
/// @brief Implements static_string.hpp.

#pragma once
#include "../binary_io.hpp"
#include "../macro.hpp"
#include "../static_string.hpp"

//

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator::operator const_iterator() const noexcept
{
	return const_iterator{this->base()};
}

//

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::static_string(size_type size, char chr) noexcept
	: m_size{size}
{
	std::fill(begin(), end(), chr);
}

template <tr::usize Capacity>
template <tr::usize Size>
constexpr tr::static_string<Capacity>::static_string(const char (&literal)[Size]) noexcept
	: m_buffer{}
	, m_size{static_cast<size_type>(Size - 1)}
{
	static_assert(Size - 1 <= Capacity, "Tried to initialize a static string with a string literal that would be too long.");
	std::copy(literal, literal + Size - 1, m_buffer.begin());
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::static_string(std::string_view str) noexcept
	: m_buffer{}
	, m_size{static_cast<size_type>(str.size())}
{
	TR_ASSERT(str.size() <= Capacity, "Tried to copy a string of size {} into a static string of capacity {}.", str.size(), Capacity);
	std::ranges::copy(str, m_buffer.begin());
}

template <tr::usize Capacity>
template <std::convertible_to<std::string_view> String>
constexpr tr::static_string<Capacity>::static_string(String&& str) noexcept
	: static_string{std::string_view{str}}
{
}

//

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::operator std::string_view() const noexcept
{
	return {m_buffer.data(), m_size};
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::operator std::string() const
{
	return std::string{std::string_view{*this}};
}

//

template <tr::usize Capacity>
constexpr std::strong_ordering tr::static_string<Capacity>::operator<=>(const static_string& rhs) const noexcept
{
	return std::string_view{*this} <=> std::string_view{rhs};
}

template <tr::usize Capacity>
constexpr bool tr::static_string<Capacity>::operator==(const static_string& rhs) const noexcept
{
	return std::string_view{*this} == std::string_view{rhs};
}

template <tr::usize Capacity>
template <std::convertible_to<std::string_view> String>
constexpr std::strong_ordering tr::static_string<Capacity>::operator<=>(const String& rhs) const noexcept
{
	return std::string_view{*this} <=> std::string_view{rhs};
}

template <tr::usize Capacity>
template <std::convertible_to<std::string_view> String>
constexpr bool tr::static_string<Capacity>::operator==(const String& rhs) const noexcept
{
	return std::string_view{*this} == std::string_view{rhs};
}

//

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::reference tr::static_string<Capacity>::operator[](size_type offset) noexcept
{
	TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in static string of size {}.", offset, m_size);

	return m_buffer[offset];
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_reference tr::static_string<Capacity>::operator[](size_type offset) const noexcept
{
	TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in static string of size {}.", offset, m_size);

	return m_buffer[offset];
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::reference tr::static_string<Capacity>::front() noexcept
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty static string.");

	return *begin();
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_reference tr::static_string<Capacity>::front() const noexcept
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty static string.");

	return *begin();
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::reference tr::static_string<Capacity>::back() noexcept
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty static string.");

	return *std::prev(end());
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_reference tr::static_string<Capacity>::back() const noexcept
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty static string.");

	return *std::prev(end());
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::pointer tr::static_string<Capacity>::data() noexcept
{
	return m_buffer.data();
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_pointer tr::static_string<Capacity>::data() const noexcept
{
	return m_buffer.data();
}

//

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::begin() noexcept
{
	return iterator{m_buffer.data()};
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::begin() const noexcept
{
	return const_iterator{m_buffer.data()};
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::cbegin() const noexcept
{
	return const_iterator{m_buffer.data()};
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::end() noexcept
{
	return iterator{m_buffer.data() + m_size};
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::end() const noexcept
{
	return const_iterator{m_buffer.data() + m_size};
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::const_iterator tr::static_string<Capacity>::cend() const noexcept
{
	return const_iterator{m_buffer.data() + m_size};
}

//

template <tr::usize Capacity>
constexpr bool tr::static_string<Capacity>::empty() const noexcept
{
	return m_size == 0;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::size_type tr::static_string<Capacity>::size() const noexcept
{
	return m_size;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::size_type tr::static_string<Capacity>::max_size() noexcept
{
	return Capacity;
}

//

template <tr::usize Capacity>
constexpr void tr::static_string<Capacity>::clear() noexcept
{
	m_size = 0;
}

template <tr::usize Capacity>
constexpr void tr::static_string<Capacity>::push_back(char chr) noexcept
{
	TR_ASSERT(m_size < Capacity, "Tried to push back into a static string that is already at its capacity of {}.", Capacity);
	m_buffer[m_size++] = chr;
}

template <tr::usize Capacity>
constexpr void tr::static_string<Capacity>::append(char chr) noexcept
{
	push_back(chr);
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>& tr::static_string<Capacity>::operator+=(char chr) noexcept
{
	push_back(chr);
	return *this;
}

template <tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
constexpr void tr::static_string<Capacity>::append(Iterator first, Iterator last) noexcept(noexcept(std::copy(first, last, end())))
{
	std::copy(first, last, end());
	m_size = m_size + std::distance(first, last);
}

template <tr::usize Capacity>
constexpr void tr::static_string<Capacity>::append(std::string_view str) noexcept
{
	TR_ASSERT(m_size + str.size() <= Capacity, "Tried to do an append onto a static string that would put it past its capacity of {}.",
			  Capacity);

	std::ranges::copy(str, end());
	m_size = m_size + str.size();
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>& tr::static_string<Capacity>::operator+=(std::string_view str) noexcept
{
	append(str);
	return *this;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::insert(const_iterator where, char chr) noexcept
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
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::insert(const_iterator where, Iterator first,
																					Iterator last) noexcept(noexcept(std::copy(first, last,
																															   where)))
{
	const usize size{usize(std::distance(first, last))};

	TR_ASSERT(m_size + size <= Capacity, "Tried to do an insert into a static string that would put it past its capacity of {}.", Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	const iterator mut_where{begin() + (where - begin())};
	std::copy_backward(mut_where, end(), end() + size);
	std::copy(first, last, mut_where);
	m_size = m_size + size;
	return mut_where;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::insert(const_iterator where, std::string_view str) noexcept
{
	TR_ASSERT(m_size + str.size() <= Capacity, "Tried to do an insert into a static string that would put it past its capacity of {}.",
			  Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_string::insert.");

	const iterator mut_where{begin() + (where - begin())};
	std::copy_backward(begin() + mut_where, end(), end() + str.size());
	std::ranges::copy(str, mut_where);
	m_size = m_size + str.size();
	return mut_where;
}

template <tr::usize Capacity>
constexpr void tr::static_string<Capacity>::pop_back() noexcept
{
	if (!empty()) {
		--m_size;
	}
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::erase(const_iterator where) noexcept
{
	TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to static_string::erase.");

	const iterator mut_where{begin() + (where - begin())};
	std::copy(mut_where + 1, end(), mut_where);
	--m_size;
	return mut_where;
}

template <tr::usize Capacity>
constexpr tr::static_string<Capacity>::iterator tr::static_string<Capacity>::erase(const_iterator start, const_iterator end) noexcept
{
	TR_ASSERT(start >= begin() && start < this->end(), "Tried to pass an invalid start iterator to static_string::erase.");
	TR_ASSERT(end >= begin() && end <= this->end(), "Tried to pass an invalid end iterator to static_string::erase.");

	const iterator mut_start{begin() + (start - begin())};
	std::copy(mut_start + (end - start), this->end(), mut_start);
	m_size = m_size - (end - start);
	return mut_start;
}

template <tr::usize Capacity>
constexpr void tr::static_string<Capacity>::resize(size_type size, char chr) noexcept
{
	TR_ASSERT(size <= Capacity, "Tried to resize a static string past its capacity of {}.", Capacity);

	for (size_type i = m_size; i < size; ++i) {
		m_buffer[i] = chr;
	}
	m_size = size;
}

//

/// Static string binary reader.
template <tr::usize Capacity>
struct tr::binary_reader<tr::static_string<Capacity>>
{
	void operator()(std::istream& is, static_string<Capacity>& out) const
	{
		out.resize(read_binary<typename static_string<Capacity>::size_type>(is));
		is.read(out.data(), out.size());
	}
};

/// Static string binary writer.
template <tr::usize Capacity>
struct tr::binary_writer<tr::static_string<Capacity>>
{
	void operator()(std::ostream& os, const static_string<Capacity>& in) const
	{
		write_binary(os, in.size());
		write_binary(os, std::span{in});
	}
};

template <tr::usize Capacity>
inline std::ostream& tr::operator<<(std::ostream& os, const static_string<Capacity>& str)
{
	return os << std::string_view{str};
}

/// Formatter for static strings.
template <tr::usize Capacity>
struct std::formatter<tr::static_string<Capacity>> : std::formatter<std::string_view>
{
};