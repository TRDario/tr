/// @file
/// @brief Implements pointer_iterator.hpp.

#pragma once
#include "../pointer_iterator.hpp"

//

template <typename Iterator, tr::pointer Pointer>
constexpr tr::pointer_iterator<Iterator, Pointer>::pointer_iterator(Pointer base)
	: m_base{base}
{
}

//

template <typename Iterator, tr::pointer Pointer>
constexpr tr::pointer_iterator<Iterator, Pointer>::reference tr::pointer_iterator<Iterator, Pointer>::operator*() const
{
	return *m_base;
}

template <typename Iterator, tr::pointer Pointer>
constexpr tr::pointer_iterator<Iterator, Pointer>::pointer tr::pointer_iterator<Iterator, Pointer>::operator->() const
{
	return m_base;
}

template <typename Iterator, tr::pointer Pointer>
constexpr tr::pointer_iterator<Iterator, Pointer>::reference tr::pointer_iterator<Iterator, Pointer>::operator[](difference_type diff) const
{
	return m_base[diff];
}

//

template <typename Iterator, tr::pointer Pointer>
constexpr Iterator& tr::pointer_iterator<Iterator, Pointer>::operator++()
{
	++m_base;
	return static_cast<Iterator&>(*this);
}

template <typename Iterator, tr::pointer Pointer>
constexpr Iterator tr::pointer_iterator<Iterator, Pointer>::operator++(int)
{
	const Iterator copy{static_cast<Iterator&>(*this)};
	++m_base;
	return copy;
}

template <typename Iterator, tr::pointer Pointer>
constexpr Iterator& tr::pointer_iterator<Iterator, Pointer>::operator--()
{
	--m_base;
	return static_cast<Iterator&>(*this);
}

template <typename Iterator, tr::pointer Pointer>
constexpr Iterator tr::pointer_iterator<Iterator, Pointer>::operator--(int)
{
	const Iterator copy{static_cast<Iterator&>(*this)};
	--m_base;
	return copy;
}

//

template <typename Iterator, tr::pointer Pointer>
constexpr Iterator& tr::pointer_iterator<Iterator, Pointer>::operator+=(difference_type diff)
{
	m_base += diff;
	return static_cast<Iterator&>(*this);
}

template <typename Iterator, tr::pointer Pointer>
constexpr Iterator& tr::pointer_iterator<Iterator, Pointer>::operator-=(difference_type diff)
{
	m_base -= diff;
	return static_cast<Iterator&>(*this);
}

namespace tr
{
	template <typename Iterator, tr::pointer Pointer>
	constexpr Iterator operator+(const pointer_iterator<Iterator, Pointer>& it,
								 typename pointer_iterator<Iterator, Pointer>::difference_type diff)
	{
		Iterator copy{static_cast<const Iterator&>(it)};
		copy += diff;
		return copy;
	}

	template <typename Iterator, tr::pointer Pointer>
	constexpr Iterator operator+(typename pointer_iterator<Iterator, Pointer>::difference_type diff,
								 const pointer_iterator<Iterator, Pointer>& it)
	{
		return it + diff;
	}

	template <typename Iterator, tr::pointer Pointer>
	constexpr Iterator operator-(const pointer_iterator<Iterator, Pointer>& it,
								 typename pointer_iterator<Iterator, Pointer>::difference_type diff)
	{
		Iterator copy{static_cast<const Iterator&>(it)};
		copy -= diff;
		return copy;
	}

	template <typename Iterator, tr::pointer Pointer>
	constexpr pointer_iterator<Iterator, Pointer>::difference_type operator-(pointer_iterator<Iterator, Pointer> lhs,
																			 pointer_iterator<Iterator, Pointer> rhs)
	{
		return lhs.m_base - rhs.m_base;
	}
} // namespace tr