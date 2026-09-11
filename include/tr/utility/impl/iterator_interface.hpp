/// @file
/// @brief Implements iterator_interface.hpp.

#pragma once
#include "../iterator_interface.hpp"

//

template <typename Iterator, typename DeducedBrackets>
constexpr Iterator& tr::iterator_interface<Iterator, DeducedBrackets>::base() noexcept
{
	return static_cast<Iterator&>(*this);
}

template <typename Iterator, typename DeducedBrackets>
constexpr const Iterator& tr::iterator_interface<Iterator, DeducedBrackets>::base() const noexcept
{
	return static_cast<const Iterator&>(*this);
}

//

namespace tr
{
	template <typename Iterator, typename DeducedBrackets>
	constexpr Iterator operator++(iterator_interface<Iterator, DeducedBrackets>& it, int) noexcept(noexcept(++it.base()))
		requires(requires { ++it.base(); })
	{
		Iterator copy{it.base()};
		++it.base();
		return copy;
	}

	template <typename Iterator, typename DeducedBrackets>
	constexpr Iterator operator--(iterator_interface<Iterator, DeducedBrackets>& it, int) noexcept(noexcept(--it.base()))
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
	constexpr Iterator& operator-=(iterator_interface<Iterator, DeducedBrackets>& it,
								   Difference diff) noexcept(noexcept(it.base() += -diff))
		requires(requires { it.base() += -diff; })
	{
		return it.base() += -diff;
	}

	template <typename Iterator, typename DeducedBrackets, typename Difference>
	[[nodiscard]] constexpr Iterator operator+(const iterator_interface<Iterator, DeducedBrackets>& it,
											   Difference diff) noexcept(noexcept(std::declval<Iterator>() += diff))
		requires(requires(Iterator i) { i += diff; })
	{
		Iterator copy{it.base()};
		copy += diff;
		return copy;
	}

	template <typename Iterator, typename DeducedBrackets>
	[[nodiscard]] constexpr Iterator operator+(auto diff, const iterator_interface<Iterator, DeducedBrackets>& it) noexcept(
		noexcept(std::declval<Iterator>() += diff))
		requires(requires(Iterator i) { i += diff; })
	{
		return it + diff;
	}

	template <typename Iterator, typename DeducedBrackets, typename Difference>
	[[nodiscard]] constexpr Iterator operator-(const iterator_interface<Iterator, DeducedBrackets>& it,
											   Difference diff) noexcept(noexcept(std::declval<Iterator>() -= diff))
		requires(requires(Iterator i) { i -= diff; })
	{
		Iterator copy{it.base()};
		copy -= diff;
		return copy;
	}
} // namespace tr

//

template <typename Iterator, typename DeducedBrackets>
constexpr auto tr::iterator_interface<Iterator, DeducedBrackets>::operator->() const noexcept(noexcept(*base()))
	requires(requires {
		{ *base() } -> lvalue_reference;
	})
{
	return &*base();
}

template <typename Iterator, typename DeducedBrackets>
template <typename Difference>
constexpr decltype(auto) tr::iterator_interface<Iterator, DeducedBrackets>::operator[](Difference diff) const
	noexcept(noexcept(*(base() + diff)))
	requires(requires { *(base() + diff); })
{
	return *(base() + diff);
}