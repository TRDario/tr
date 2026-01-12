#pragma once
#include "static_vector.hpp"

template <class T, tr::usize S>
tr::static_vector<T, S>::static_vector(size_type size)
	requires(std::default_initializable<T>)
	: m_size{size_type(size)}
{
	TR_ASSERT(size <= S, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, S);

	for (usize i = 0; i < m_size; ++i) {
		new (m_buffer + sizeof(T) * i) T{};
	}
}

template <class T, tr::usize S>
tr::static_vector<T, S>::static_vector(size_type size, const T& v)
	requires(std::copy_constructible<T>)
	: m_size{size_type(size)}
{
	TR_ASSERT(size <= S, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, S);

	for (usize i = 0; i < m_size; ++i) {
		new (m_buffer + sizeof(T) * i) T{v};
	}
}

template <class T, tr::usize S>
template <std::input_iterator It>
	requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
tr::static_vector<T, S>::static_vector(It first, It last)
	: m_size{std::distance(first, last)}
{
	TR_ASSERT(m_size <= S, "Tried to create a static vector of size {} but with a max capacity of only {}.", m_size, S);

	std::move(first, last, begin());
}

template <class T, tr::usize S>
template <std::ranges::input_range Range>
	requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
tr::static_vector<T, S>::static_vector(Range&& range)
	: static_vector{std::ranges::begin(range), std::ranges::end(range)}
{
}

template <class T, tr::usize S>
tr::static_vector<T, S>::static_vector(std::initializer_list<T> init)
	requires(std::copy_constructible<T>)
	: static_vector{init.begin(), init.end()}
{
}

template <class T, tr::usize S>
tr::static_vector<T, S>::static_vector(const static_vector& r)
	requires(std::copy_constructible<T>)
	: static_vector{r.begin(), r.end()}
{
}

template <class T, tr::usize S>
tr::static_vector<T, S>::static_vector(static_vector&& r) noexcept(std::is_nothrow_move_constructible_v<T>)
	requires(std::move_constructible<T>)
	: static_vector{std::move_iterator{r.begin()}, std::move_iterator{r.end()}}
{
	r.clear();
}

template <class T, tr::usize S> tr::static_vector<T, S>::~static_vector<T, S>()
{
	for (usize i = 0; i < m_size; ++i) {
		((T*)m_buffer + i)->~T();
	}
}

//

template <class T, tr::usize S> tr::static_vector<T, S>::reference tr::static_vector<T, S>::at(size_type offset)
{
	TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);
	return data()[offset];
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reference tr::static_vector<T, S>::at(size_type offset) const
{
	TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);
	return data()[offset];
}

template <class T, tr::usize S> tr::static_vector<T, S>::reference tr::static_vector<T, S>::operator[](size_type offset)
{
	return at(offset);
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reference tr::static_vector<T, S>::operator[](size_type offset) const
{
	return at(offset);
}

template <class T, tr::usize S> tr::static_vector<T, S>::reference tr::static_vector<T, S>::front()
{
	return at(0);
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reference tr::static_vector<T, S>::front() const
{
	return at(0);
}

template <class T, tr::usize S> tr::static_vector<T, S>::reference tr::static_vector<T, S>::back()
{
	return at(m_size - 1);
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reference tr::static_vector<T, S>::back() const
{
	return at(m_size - 1);
}

template <class T, tr::usize S> tr::static_vector<T, S>::pointer tr::static_vector<T, S>::data()
{
	return (T*)m_buffer;
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_pointer tr::static_vector<T, S>::data() const
{
	return (const T*)m_buffer;
}

//

template <class T, tr::usize S> tr::static_vector<T, S>::iterator tr::static_vector<T, S>::begin()
{
	return data();
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_iterator tr::static_vector<T, S>::begin() const
{
	return data();
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_iterator tr::static_vector<T, S>::cbegin() const
{
	return data();
}

template <class T, tr::usize S> tr::static_vector<T, S>::iterator tr::static_vector<T, S>::end()
{
	return begin() + m_size;
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_iterator tr::static_vector<T, S>::end() const
{
	return begin() + m_size;
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_iterator tr::static_vector<T, S>::cend() const
{
	return begin() + m_size;
}

template <class T, tr::usize S> tr::static_vector<T, S>::reverse_iterator tr::static_vector<T, S>::rbegin()
{
	return std::reverse_iterator{end()};
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reverse_iterator tr::static_vector<T, S>::rbegin() const
{
	return std::reverse_iterator{end()};
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reverse_iterator tr::static_vector<T, S>::crbegin() const
{
	return std::reverse_iterator{cend()};
}

template <class T, tr::usize S> tr::static_vector<T, S>::reverse_iterator tr::static_vector<T, S>::rend()
{
	return std::reverse_iterator{begin()};
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reverse_iterator tr::static_vector<T, S>::rend() const
{
	return std::reverse_iterator{begin()};
}

template <class T, tr::usize S> tr::static_vector<T, S>::const_reverse_iterator tr::static_vector<T, S>::crend() const
{
	return std::reverse_iterator{cbegin()};
}

//

template <class T, tr::usize S> constexpr bool tr::static_vector<T, S>::empty() const
{
	return m_size == 0;
}

template <class T, tr::usize S> constexpr tr::static_vector<T, S>::size_type tr::static_vector<T, S>::size() const
{
	return m_size;
}

template <class T, tr::usize S> constexpr tr::static_vector<T, S>::size_type tr::static_vector<T, S>::max_size()
{
	return S;
}

//

template <class T, tr::usize S> void tr::static_vector<T, S>::clear()
{
	for (usize i = 0; i < m_size; ++i) {
		((T*)m_buffer + i)->~T();
	}
	m_size = 0;
}

template <class T, tr::usize S>
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::insert(const_iterator where, const T& v)
	requires(std::copy_constructible<T>)
{
	return emplace(where, v);
}

template <class T, tr::usize S>
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::insert(const_iterator where, T&& v)
	requires(std::move_constructible<T>)
{
	return emplace(where, std::move(v));
}

template <class T, tr::usize S>
template <std::input_iterator It>
	requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::insert(const_iterator where, It first, It last)
{
	const auto range_size{std::distance(first, last)};

	TR_ASSERT(m_size + range_size <= S, "Tried to do an insert into a static vector that would put it past its capacity of {}.", S);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	std::move_backward(iterator(where), end(), end() + range_size);
	std::move(first, last, iterator(where));
	m_size = size_type(m_size + range_size);
	return iterator(where);
}

template <class T, tr::usize S>
template <std::ranges::input_range Range>
	requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::insert(const_iterator where, Range&& range)
{
	return insert(where, std::ranges::begin(range), std::ranges::end(range));
}

template <class T, tr::usize S>
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::insert(const_iterator where, std::initializer_list<T> init)
	requires(std::copy_constructible<T>)
{
	return insert(where, init.begin(), init.end());
}

template <class T, tr::usize S>
template <class... Args>
	requires(std::constructible_from<T, Args...>)
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::emplace(const_iterator where, Args&&... args)
{
	TR_ASSERT(m_size < S, "Tried to insert into a static vector that is already at its capacity of {}.", S);
	TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

	std::move_backward(iterator(where), end(), end() + 1);
	new (&*iterator(where)) T{std::forward<Args>(args)...};
	++m_size;
	return iterator(where);
}

template <class T, tr::usize S> tr::static_vector<T, S>::iterator tr::static_vector<T, S>::erase(const_iterator where)
{
	TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to static_vector::erase.");

	iterator(where)->~T();
	std::move(iterator(where) + 1, end(), iterator(where));
	--m_size;
	return iterator(where);
}

template <class T, tr::usize S> tr::static_vector<T, S>::iterator tr::static_vector<T, S>::erase(const_iterator first, const_iterator last)
{
	TR_ASSERT(first >= begin() && first < end(), "Tried to pass an invalid start iterator to static_vector::erase.");
	TR_ASSERT(last >= begin() && last <= end(), "Tried to pass an invalid end iterator to static_vector::erase.");

	for (iterator it = iterator(first); it != last; ++it) {
		it->~T();
	}
	std::move(iterator(first) + (first - last), end(), iterator(first));
	m_size = size_type(m_size - (first - last));
	return first;
}

template <class T, tr::usize S>
tr::static_vector<T, S>::reference tr::static_vector<T, S>::push_back(const T& v)
	requires(std::copy_constructible<T>)
{
	return emplace_back(v);
}

template <class T, tr::usize S>
tr::static_vector<T, S>::reference tr::static_vector<T, S>::push_back(T&& v)
	requires(std::move_constructible<T>)
{
	return emplace_back(std::move(v));
}

template <class T, tr::usize S>
template <class... Args>
	requires(std::constructible_from<T, Args...>)
tr::static_vector<T, S>::reference tr::static_vector<T, S>::emplace_back(Args&&... args)
{
	new (m_buffer + m_size++ * sizeof(T)) T{std::forward<Args>(args)...};
	return back();
}

template <class T, tr::usize S>
template <std::input_iterator It>
	requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::append(It first, It last)
{
	return insert(end(), first, last);
}

template <class T, tr::usize S>
template <std::ranges::input_range Range>
	requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::append(Range&& range)
{
	return append(std::ranges::begin(range), std::ranges::end(range));
}

template <class T, tr::usize S>
tr::static_vector<T, S>::iterator tr::static_vector<T, S>::append(std::initializer_list<T> init)
	requires(std::copy_constructible<T>)
{
	return append(init.begin(), init.end());
}

template <class T, tr::usize S> void tr::static_vector<T, S>::pop_back()
{
	back()->~T();
	--m_size;
}

template <class T, tr::usize S>
void tr::static_vector<T, S>::resize(size_type size)
	requires(std::default_initializable<T>)
{
	if (size < m_size) {
		for (size_type i = size; i != m_size; ++i) {
			(data() + i)->~T();
		}
	}
	else if (size > m_size) {
		for (size_type i = m_size; i != size; ++i) {
			new ((T*)m_buffer + i) T{};
		}
	}
	m_size = size;
}

template <class T, tr::usize S>
void tr::static_vector<T, S>::resize(size_type size, const T& v)
	requires(std::copy_constructible<T>)
{
	if (size < m_size) {
		for (size_type i = size; i != m_size; ++i) {
			(data() + i)->~T();
		}
	}
	else if (size > m_size) {
		for (size_type i = m_size; i != size; ++i) {
			new ((T*)m_buffer + i) T{v};
		}
	}
	m_size = size;
}

//

template <tr::binary_constructible T, tr::usize S>
void tr::binary_reader<tr::static_vector<T, S>>::operator()(std::istream& is, static_vector<T, S>& out) const
{
	out.resize(binary_read<typename tr::static_vector<T, S>::size_type>(is));
	binary_read(is, std::span{out});
}

template <tr::binary_writable T, tr::usize S>
void tr::binary_writer<tr::static_vector<T, S>>::operator()(std::ostream& os, const static_vector<T, S>& in) const
{
	binary_write(os, in.size());
	binary_write(os, std::span{in});
}