/// @file
/// @brief Implements iterator_interface.hpp.

#pragma once
#include "../iterator_interface.hpp"

//

template <typename Iterator, typename DeducedBrackets>
constexpr Iterator& tr::iterator_interface<Iterator, DeducedBrackets>::base()
{
	return static_cast<Iterator&>(*this);
}

template <typename Iterator, typename DeducedBrackets>
constexpr const Iterator& tr::iterator_interface<Iterator, DeducedBrackets>::base() const
{
	return static_cast<const Iterator&>(*this);
}

//

namespace tr
{
	template <typename Iterator, typename DeducedBrackets>
	constexpr Iterator operator++(iterator_interface<Iterator, DeducedBrackets>& it, int)
		requires(requires { ++it.base(); })
	{
		Iterator copy{it.base()};
		++it.base();
		return copy;
	}

	template <typename Iterator, typename DeducedBrackets>
	constexpr Iterator operator--(iterator_interface<Iterator, DeducedBrackets>& it, int)
		requires(requires { --it.base(); })
	{
		Iterator copy{it.base()};
		--it.base();
		return copy;
	}
} // namespace tr

//

namespace tr
{
	template <typename Iterator, typename DeducedBrackets, typename Difference>
	constexpr Iterator& operator-=(iterator_interface<Iterator, DeducedBrackets>& it, Difference diff)
		requires(requires { it.base() += -diff; })
	{
		return it.base() += -diff;
	}

	template <typename Iterator, typename DeducedBrackets, typename Difference>
	constexpr Iterator operator+(const iterator_interface<Iterator, DeducedBrackets>& it, Difference diff)
		requires(requires(Iterator i) { i += diff; })
	{
		Iterator copy{it.base()};
		copy += diff;
		return copy;
	}

	template <typename Iterator, typename DeducedBrackets>
	constexpr Iterator operator+(auto diff, const iterator_interface<Iterator, DeducedBrackets>& it)
		requires(requires(Iterator i) { i += diff; })
	{
		return it + diff;
	}

	template <typename Iterator, typename DeducedBrackets, typename Difference>
	constexpr Iterator operator-(const iterator_interface<Iterator, DeducedBrackets>& it, Difference diff)
		requires(requires(Iterator i) { i -= diff; })
	{
		Iterator copy{it.base()};
		copy -= diff;
		return copy;
	}
} // namespace tr

//

template <typename Iterator, typename DeducedBrackets>
constexpr auto tr::iterator_interface<Iterator, DeducedBrackets>::operator->() const
	requires(requires {
		{ *base() } -> lvalue_reference;
	})
{
	return &*base();
}

template <typename Iterator, typename DeducedBrackets>
template <typename Difference>
constexpr decltype(auto) tr::iterator_interface<Iterator, DeducedBrackets>::operator[](Difference diff) const
	requires(requires(Iterator it) { it + diff; })
{
	return *(base() + diff);
}