///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements static_vector.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../static_vector.hpp"

////////////////////////////////////////////////////////////// STATIC VECTOR //////////////////////////////////////////////////////////////

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(size_type size)
	requires(std::default_initializable<Element>)
	: m_size{size}
{
	TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

	std::uninitialized_default_construct(begin(), end());
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(size_type size, const Element& v)
	requires(std::copy_constructible<Element>)
	: m_size{size}
{
	TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

	std::uninitialized_fill(begin(), end(), v);
}

template <typename Element, tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
tr::static_vector<Element, Capacity>::static_vector(Iterator first, Iterator last)
	: m_size{0}
{
	append(first, last);
}

template <typename Element, tr::usize Capacity>
template <std::ranges::input_range Range>
	requires(std::same_as<typename std::ranges::range_value_t<Range>, Element>)
tr::static_vector<Element, Capacity>::static_vector(Range&& range)
	: static_vector{std::ranges::begin(range), std::ranges::end(range)}
{
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(std::initializer_list<Element> init)
	requires(std::copy_constructible<Element>)
	: static_vector{init.begin(), init.end()}
{
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(const static_vector& r)
	requires(std::copy_constructible<Element>)
	: static_vector{r.begin(), r.end()}
{
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(static_vector&& r) noexcept(std::is_nothrow_move_constructible_v<Element>)
	requires(std::move_constructible<Element>)
	: static_vector{std::move_iterator{r.begin()}, std::move_iterator{r.end()}}
{
	r.clear();
}

template <typename Element, tr::usize Capacity> tr::static_vector<Element, Capacity>::~static_vector<Element, Capacity>()
{
	std::destroy(begin(), end());
}

//

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::at(size_type offset)
{
	TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);
	return data()[offset];
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::at(size_type offset) const
{
	TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);
	return data()[offset];
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::operator[](size_type offset)
{
	return at(offset);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::operator[](size_type offset) const
{
	return at(offset);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::front()
{
	return at(0);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::front() const
{
	return at(0);
}

template <typename Element, tr::usize Capacity> tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::back()
{
	return at(m_size - 1);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reference tr::static_vector<Element, Capacity>::back() const
{
	return at(m_size - 1);
}

template <typename Element, tr::usize Capacity> tr::static_vector<Element, Capacity>::pointer tr::static_vector<Element, Capacity>::data()
{
	return reinterpret_cast<Element*>(m_buffer);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_pointer tr::static_vector<Element, Capacity>::data() const
{
	return reinterpret_cast<const Element*>(m_buffer);
}

//

template <typename Element, tr::usize Capacity> tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::begin()
{
	return data();
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::begin() const
{
	return data();
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::cbegin() const
{
	return data();
}

template <typename Element, tr::usize Capacity> tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::end()
{
	return begin() + m_size;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::end() const
{
	return begin() + m_size;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_iterator tr::static_vector<Element, Capacity>::cend() const
{
	return begin() + m_size;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reverse_iterator tr::static_vector<Element, Capacity>::rbegin()
{
	return std::reverse_iterator{end()};
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::rbegin() const
{
	return std::reverse_iterator{end()};
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::crbegin() const
{
	return std::reverse_iterator{cend()};
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reverse_iterator tr::static_vector<Element, Capacity>::rend()
{
	return std::reverse_iterator{begin()};
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::rend() const
{
	return std::reverse_iterator{begin()};
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_reverse_iterator tr::static_vector<Element, Capacity>::crend() const
{
	return std::reverse_iterator{cbegin()};
}

//

template <typename Element, tr::usize Capacity> constexpr bool tr::static_vector<Element, Capacity>::empty() const
{
	return m_size == 0;
}

template <typename Element, tr::usize Capacity>
constexpr tr::static_vector<Element, Capacity>::size_type tr::static_vector<Element, Capacity>::size() const
{
	return m_size;
}

template <typename Element, tr::usize Capacity>
constexpr tr::static_vector<Element, Capacity>::size_type tr::static_vector<Element, Capacity>::max_size()
{
	return Capacity;
}

//

template <typename Element, tr::usize Capacity> void tr::static_vector<Element, Capacity>::clear()
{
	std::destroy(begin(), end());
	m_size = 0;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, const Element& v)
	requires(std::copy_constructible<Element>)
{
	return emplace(where, v);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, Element&& v)
	requires(std::move_constructible<Element>)
{
	return emplace(where, std::move(v));
}

template <typename Element, tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator cwhere, Iterator first,
																							Iterator last)
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

template <typename Element, tr::usize Capacity>
template <std::ranges::input_range Range>
	requires(std::same_as<typename std::ranges::range_value_t<Range>, Element>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, Range&& range)
{
	return insert(where, std::ranges::begin(range), std::ranges::end(range));
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where,
																							std::initializer_list<Element> init)
	requires(std::copy_constructible<Element>)
{
	return insert(where, init.begin(), init.end());
}

template <typename Element, tr::usize Capacity>
template <typename... Args>
	requires(std::constructible_from<Element, Args...>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::emplace(const_iterator cwhere, Args&&... args)
{
	TR_ASSERT(m_size < Capacity, "Tried to insert into a static vector that is already at its capacity of {}.", Capacity);
	TR_ASSERT(cwhere >= begin() && cwhere <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	const iterator where{begin() + std::distance(cbegin(), cwhere)};
	if (where != end()) {
		new (end()) Element{std::move(back())};
		std::move_backward(where, end() - 1, end());
		where->~Element();
	}
	new (where) Element{std::forward<Args>(args)...};
	++m_size;
	return where;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::erase(const_iterator cwhere)
{
	TR_ASSERT(cwhere >= begin() && cwhere < end(), "Tried to pass an invalid iterator to static_vector::erase.");

	const iterator where{begin() + (cwhere - begin())};
	std::move(where + 1, end(), where);
	where->~Element();
	--m_size;
	return where;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::erase(const_iterator cfirst, const_iterator clast)
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

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::push_back(const Element& v)
	requires(std::copy_constructible<Element>)
{
	return emplace_back(v);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::push_back(Element&& v)
	requires(std::move_constructible<Element>)
{
	return emplace_back(std::move(v));
}

template <typename Element, tr::usize Capacity>
template <typename... Args>
	requires(std::constructible_from<Element, Args...>)
tr::static_vector<Element, Capacity>::reference tr::static_vector<Element, Capacity>::emplace_back(Args&&... args)
{
	return *emplace(end(), std::forward<Args>(args)...);
}

template <typename Element, tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::append(Iterator first, Iterator last)
{
	return insert(end(), first, last);
}

template <typename Element, tr::usize Capacity>
template <std::ranges::input_range Range>
	requires(std::same_as<typename std::ranges::range_value_t<Range>, Element>)
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::append(Range&& range)
{
	return append(std::ranges::begin(range), std::ranges::end(range));
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::append(std::initializer_list<Element> init)
	requires(std::copy_constructible<Element>)
{
	return append(init.begin(), init.end());
}

template <typename Element, tr::usize Capacity> void tr::static_vector<Element, Capacity>::pop_back()
{
	back().~Element();
	--m_size;
}

template <typename Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::resize(size_type size)
	requires(std::default_initializable<Element>)
{
	const iterator old_end{end()};
	m_size = size;
	if (end() < old_end) {
		std::destroy(end(), old_end);
	}
	else if (end() > old_end) {
		std::uninitialized_default_construct(old_end, end());
	}
}

template <typename Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::resize(size_type size, const Element& v)
	requires(std::copy_constructible<Element>)
{
	const iterator old_end{end()};
	m_size = size;
	if (end() < old_end) {
		std::destroy(end(), old_end);
	}
	else if (end() > old_end) {
		std::uninitialized_fill(old_end, end(), v);
	}
}

//////////////////////////////////////////////////////////////// BINARY IO ////////////////////////////////////////////////////////////////

template <tr::binary_constructible Element, tr::usize Capacity>
void tr::binary_reader<tr::static_vector<Element, Capacity>>::operator()(std::istream& is, static_vector<Element, Capacity>& out) const
{
	out.resize(read_binary<typename tr::static_vector<Element, Capacity>::size_type>(is));
	read_binary(is, std::span{out});
}

template <tr::binary_writable Element, tr::usize Capacity>
void tr::binary_writer<tr::static_vector<Element, Capacity>>::operator()(std::ostream& os, const static_vector<Element, Capacity>& in) const
{
	write_binary(os, in.size());
	write_binary(os, std::span{in});
}