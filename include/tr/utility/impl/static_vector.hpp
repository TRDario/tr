///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements static_vector.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../static_vector.hpp"

////////////////////////////////////////////////////////////// STATIC VECTOR //////////////////////////////////////////////////////////////

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(size_type size)
	requires(std::default_initializable<Element>)
	: m_size{size_type(size)}
{
	TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

	for (usize i = 0; i < m_size; ++i) {
		new (m_buffer + sizeof(Element) * i) Element{};
	}
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::static_vector(size_type size, const Element& v)
	requires(std::copy_constructible<Element>)
	: m_size{size_type(size)}
{
	TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

	for (usize i = 0; i < m_size; ++i) {
		new (m_buffer + sizeof(Element) * i) Element{v};
	}
}

template <typename Element, tr::usize Capacity>
template <std::input_iterator Iterator>
	requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
tr::static_vector<Element, Capacity>::static_vector(Iterator first, Iterator last)
	: m_size{std::distance(first, last)}
{
	TR_ASSERT(m_size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", m_size, Capacity);

	std::move(first, last, begin());
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
	for (usize i = 0; i < m_size; ++i) {
		((Element*)m_buffer + i)->~Element();
	}
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
	return (Element*)m_buffer;
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::const_pointer tr::static_vector<Element, Capacity>::data() const
{
	return (const Element*)m_buffer;
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
	for (usize i = 0; i < m_size; ++i) {
		at(i).~Element();
	}
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
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::insert(const_iterator where, Iterator first,
																							Iterator last)
{
	const auto range_size{std::distance(first, last)};

	TR_ASSERT(m_size + range_size <= Capacity, "Tried to do an insert into a static vector that would put it past its capacity of {}.",
			  Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	std::move_backward(iterator(where), end(), end() + range_size);
	std::move(first, last, iterator(where));
	m_size = size_type(m_size + range_size);
	return iterator(where);
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
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::emplace(const_iterator where, Args&&... args)
{
	TR_ASSERT(m_size < Capacity, "Tried to insert into a static vector that is already at its capacity of {}.", Capacity);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	std::move_backward(iterator(where), end(), end() + 1);
	new (&*iterator(where)) Element{std::forward<Args>(args)...};
	++m_size;
	return iterator(where);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::erase(const_iterator where)
{
	TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to static_vector::erase.");

	iterator(where)->~Element();
	std::move(iterator(where) + 1, end(), iterator(where));
	--m_size;
	return iterator(where);
}

template <typename Element, tr::usize Capacity>
tr::static_vector<Element, Capacity>::iterator tr::static_vector<Element, Capacity>::erase(const_iterator first, const_iterator last)
{
	TR_ASSERT(first >= begin() && first < end(), "Tried to pass an invalid start iterator to static_vector::erase.");
	TR_ASSERT(last >= begin() && last <= end(), "Tried to pass an invalid end iterator to static_vector::erase.");

	for (iterator it = iterator(first); it != last; ++it) {
		it->~Element();
	}
	std::move(iterator(first) + (first - last), end(), iterator(first));
	m_size = size_type(m_size - (first - last));
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
	new (data() + m_size++) Element{std::forward<Args>(args)...};
	return back();
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
	back()->~Element();
	--m_size;
}

template <typename Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::resize(size_type size)
	requires(std::default_initializable<Element>)
{
	if (size < m_size) {
		for (size_type i = size; i != m_size; ++i) {
			at(i).~Element();
		}
	}
	else if (size > m_size) {
		for (size_type i = m_size; i != size; ++i) {
			new (data() + i) Element{};
		}
	}
	m_size = size;
}

template <typename Element, tr::usize Capacity>
void tr::static_vector<Element, Capacity>::resize(size_type size, const Element& v)
	requires(std::copy_constructible<Element>)
{
	if (size < m_size) {
		for (size_type i = size; i != m_size; ++i) {
			at(i).~Element();
		}
	}
	else if (size > m_size) {
		for (size_type i = m_size; i != size; ++i) {
			new (data() + i) Element{v};
		}
	}
	m_size = size;
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