/// @file
/// @brief Implements static_vector.hpp.

#pragma once
#include "../binary_io.hpp"
#include "../macro.hpp"
#include "../static_vector.hpp"

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator::operator const_iterator() const noexcept
{
	return const_iterator{this->base()};
}

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(size_type size) noexcept(std::is_nothrow_default_constructible_v<Element>)
	requires(std::default_initializable<Element>)
	: m_size{size}
{
	TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

	std::uninitialized_default_construct(begin(), end());
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(size_type size, const Element& value)
	noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
	: m_size{size}
{
	TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

	std::uninitialized_fill(begin(), end(), value);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <tr::forward_iterator_to_convertible_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
tr::static_vector<Element, Capacity>::static_vector(Iterator first, Sentinel last)
	noexcept(nothrow_forward_iterator_to_convertible_to<Iterator, Element>)
	: m_size{static_cast<size_type>(std::ranges::distance(first, last))}
{
	std::uninitialized_copy(first, last, begin());
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <tr::forward_range_to_convertible_to<Element> Range>
tr::static_vector<Element, Capacity>::static_vector(Range&& range) noexcept(nothrow_forward_range_to_convertible_to<Range, Element>)
	: static_vector{std::ranges::begin(range), std::ranges::end(range)}
{
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(std::initializer_list<Element> init)
	noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
	: static_vector{init.begin(), init.end()}
{
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(const static_vector& rhs) noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
	: static_vector{rhs.begin(), rhs.end()}
{
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(static_vector&& rhs) noexcept(std::is_nothrow_move_constructible_v<Element>)
	requires(std::move_constructible<Element>)
	: static_vector{std::move_iterator{rhs.begin()}, std::move_iterator{rhs.end()}}
{
	rhs.clear();
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::~static_vector<Element, Capacity>() noexcept
{
	std::destroy(begin(), end());
}

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>& tr::static_vector<Element, Capacity>::operator=(const static_vector& rhs)
	noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
{
	std::destroy(begin(), end());
	std::uninitialized_copy(rhs.begin(), rhs.end(), begin());
	m_size = rhs.m_size;
	return *this;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>& tr::static_vector<Element, Capacity>::operator=(static_vector&& rhs)
	noexcept(std::is_nothrow_move_constructible_v<Element>)
	requires(std::move_constructible<Element>)
{
	std::destroy(begin(), end());
	std::uninitialized_move(rhs.begin(), rhs.end(), begin());
	m_size = rhs.m_size;
	rhs.clear();
	return *this;
}

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::at(size_type offset) noexcept
{
	TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);
	return data()[offset];
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::at(size_type offset) const noexcept
{
	TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);
	return data()[offset];
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::operator[](size_type offset) noexcept
{
	return at(offset);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::operator[](size_type offset) const noexcept
{
	return at(offset);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::front() noexcept
{
	return at(0);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::front() const noexcept
{
	return at(0);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::back() noexcept
{
	return at(m_size - 1);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::back() const noexcept
{
	return at(m_size - 1);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::pointer tr::static_vector<Element, Capacity>::data() noexcept
{
	return reinterpret_cast<Element*>(m_buffer);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_pointer tr::static_vector<Element, Capacity>::data() const noexcept
{
	return reinterpret_cast<const Element*>(m_buffer);
}

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::begin() noexcept
{
	return iterator{data()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::begin() const noexcept
{
	return const_iterator{data()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::cbegin() const noexcept
{
	return begin();
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::end() noexcept
{
	return begin() + m_size;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::end() const noexcept
{
	return begin() + m_size;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::cend() const noexcept
{
	return begin() + m_size;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reverse_iterator tr::static_vector<Element, Capacity>::rbegin() noexcept
{
	return std::reverse_iterator{end()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::rbegin() const noexcept
{
	return std::reverse_iterator{end()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::crbegin() const noexcept
{
	return std::reverse_iterator{cend()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reverse_iterator tr::static_vector<Element, Capacity>::rend() noexcept
{
	return std::reverse_iterator{begin()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::rend() const noexcept
{
	return std::reverse_iterator{begin()};
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::crend() const noexcept
{
	return std::reverse_iterator{cbegin()};
}

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
constexpr bool tr::static_vector<Element, Capacity>::empty() const noexcept
{
	return m_size == 0;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
constexpr tr::static_vector<Element, Capacity>::size_type tr::static_vector<Element, Capacity>::size() const noexcept
{
	return m_size;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
constexpr tr::static_vector<Element, Capacity>::size_type tr::static_vector<Element, Capacity>::max_size() noexcept
{
	return Capacity;
}

//

template <tr::nothrow_destructible Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::clear() noexcept
{
	std::destroy(begin(), end());
	m_size = 0;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>

tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, const Element& value)
	noexcept(nothrow_movable<Element> && std::is_nothrow_copy_constructible_v<Element>)
	requires(std::move_constructible<Element> && move_assignable<Element> && std::copy_constructible<Element>)
{
	return emplace(where, value);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, Element&& value)
	noexcept(nothrow_movable<Element>)
	requires(std::move_constructible<Element> && move_assignable<Element>)
{
	return emplace(where, std::move(value));
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <tr::forward_iterator_to_convertible_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
	requires(std::move_constructible<Element> && tr::move_assignable<Element>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator cwhere, Iterator first,
																							Sentinel last)
	noexcept(nothrow_movable<Element> && nothrow_forward_iterator_to_convertible_to<Iterator, Element>)
{
	const auto inserted_elements{std::distance(first, last)};

	TR_ASSERT(static_cast<usize>(m_size + inserted_elements) <= Capacity,
			  "Tried to do an insert into a static vector that would put it past its capacity of {}.", Capacity);
	TR_ASSERT(cwhere >= begin() && cwhere <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	const iterator where{begin() + std::distance(cbegin(), cwhere)};
	const difference_type affected_elements{std::distance(where, end())};
	if (affected_elements > 0) {
		if (inserted_elements < affected_elements) {
			std::uninitialized_move(end() - inserted_elements, end(), end());
			std::move_backward(where, end() - inserted_elements, end());
			std::destroy_n(where, inserted_elements);
		}
		else {
			std::uninitialized_move(where, end(), where + inserted_elements);
			std::destroy(where, end());
		}
	}
	std::uninitialized_copy(first, last, where);
	m_size += inserted_elements;
	return where;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <tr::forward_range_to_convertible_to<Element> Range>
	requires(std::move_constructible<Element> && tr::move_assignable<Element>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, Range&& range)
	noexcept(nothrow_movable<Element> && nothrow_forward_range_to_convertible_to<Range, Element>)
{
	return insert(where, std::ranges::begin(range), std::ranges::end(range));
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where,
																							std::initializer_list<Element> init)
	noexcept(nothrow_movable<Element> && std::is_nothrow_copy_constructible_v<Element>)
	requires(std::move_constructible<Element> && move_assignable<Element> && std::copy_constructible<Element>)
{
	return insert(where, init.begin(), init.end());
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <typename... Args>
	requires(std::move_constructible<Element> && tr::move_assignable<Element> && std::constructible_from<Element, Args...>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::emplace(const_iterator cwhere, Args&&... args)
	noexcept(nothrow_movable<Element> && std::is_nothrow_constructible_v<Element, Args...>)
{
	TR_ASSERT(m_size < Capacity, "Tried to insert into a static vector that is already at its capacity of {}.", Capacity);
	TR_ASSERT(cwhere >= begin() && cwhere <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	const iterator where{begin() + std::distance(cbegin(), cwhere)};
	if (where != end()) {
		new (end().base()) Element{std::move(back())};
		std::move_backward(where, end() - 1, end());
		where->~Element();
	}
	new (where.base()) Element{std::forward<Args>(args)...};
	++m_size;
	return where;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::erase(const_iterator cwhere)
	noexcept(std::is_nothrow_move_assignable_v<Element>)
	requires(move_assignable<Element>)
{
	TR_ASSERT(cwhere >= begin() && cwhere < end(), "Tried to pass an invalid iterator to static_vector::erase.");

	const iterator where{begin() + (cwhere - begin())};
	std::move(where + 1, end(), where);
	back().~Element();
	--m_size;
	return where;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::erase(const_iterator cfirst, const_iterator clast)
	noexcept(std::is_nothrow_move_assignable_v<Element>)
	requires(move_assignable<Element>)
{
	TR_ASSERT(cfirst >= begin() && cfirst < end(), "Tried to pass an invalid start iterator to static_vector::erase.");
	TR_ASSERT(clast >= begin() && clast <= end(), "Tried to pass an invalid end iterator to static_vector::erase.");

	const iterator first{begin() + (cfirst - begin())};
	const iterator last{begin() + (clast - begin())};
	const difference_type erased_elements{std::distance(first, last)};
	std::move(last, end(), first);
	std::destroy(end() - erased_elements, end());
	m_size -= erased_elements;
	return first;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::push_back(const Element& value)
	noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
{
	return emplace_back(value);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::push_back(Element&& value)
	noexcept(std::is_nothrow_move_constructible_v<Element>)
	requires(std::move_constructible<Element>)
{
	return emplace_back(std::move(value));
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <typename... Args>
	requires(std::constructible_from<Element, Args...>)
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::emplace_back(Args&&... args)
	noexcept(std::is_nothrow_constructible_v<Element, Args...>)
{
	return *emplace(end(), std::forward<Args>(args)...);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <tr::forward_iterator_to_convertible_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::append(Iterator first, Sentinel last)
	noexcept(nothrow_forward_iterator_to_convertible_to<Iterator, Element>)
{
	return insert(end(), first, last);
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
template <tr::forward_range_to_convertible_to<Element> Range>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::append(Range&& range)
	noexcept(nothrow_forward_range_to_convertible_to<Range, Element>)
{
	return append(std::ranges::begin(range), std::ranges::end(range));
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::append(std::initializer_list<Element> init)
	noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
{
	return append(init.begin(), init.end());
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::pop_back() noexcept
{
	back().~Element();
	--m_size;
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::resize(size_type size) noexcept(std::is_nothrow_default_constructible_v<Element>)
	requires(std::default_initializable<Element>)
{
	TR_ASSERT(size <= max_size(), "Tried to resize a static vector with capacity {} to size {}.", max_size(), size);

	const iterator old_end{end()};
	m_size = size;
	if (end() < old_end) {
		std::destroy(end(), old_end);
	}
	else if (end() > old_end) {
		std::uninitialized_default_construct(old_end, end());
	}
}

template <tr::nothrow_destructible Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::resize(size_type size, const Element& value)
	noexcept(std::is_nothrow_copy_constructible_v<Element>)
	requires(std::copy_constructible<Element>)
{
	TR_ASSERT(size <= max_size(), "Tried to resize a static vector with capacity {} to size {}.", max_size(), size);

	const iterator old_end{end()};
	m_size = size;
	if (end() < old_end) {
		std::destroy(end(), old_end);
	}
	else if (end() > old_end) {
		std::uninitialized_fill(old_end, end(), value);
	}
}

//

/// Static vector binary reader.
template <tr::binary_constructible Element, tr::usize Capacity>
struct tr::binary_reader<tr::static_vector<Element, Capacity>>
{
	void operator()(std::istream& is, static_vector<Element, Capacity>& out) const
	{
		out.resize(read_binary<typename static_vector<Element, Capacity>::size_type>(is));
		read_binary(is, std::span{out});
	}
};

/// Static vector binary writer.
template <tr::binary_writable Element, tr::usize Capacity>
struct tr::binary_writer<tr::static_vector<Element, Capacity>>
{
	void operator()(std::ostream& os, const static_vector<Element, Capacity>& in) const
	{
		write_binary(os, in.size());
		write_binary(os, std::span{in});
	}
};