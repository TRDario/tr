/// @file
/// @brief Provides `tr::iterator_interface`.

#pragma once
#include <tr/utility/concepts.hpp>

//

namespace tr
{
	/// @brief Interface an iterator class can inherit from to automatically derive various functions from a small subset of basic operators
	/// (`operator++`, `operator--`, `operator+=`, `operator*`).
	/// @details
	/// `operator++(int)` is synthesized when `operator++` is defined.\n
	/// `operator--(int)` is synthesized when `operator--` is defined.\n
	/// `operator+`, `operator-`, and `operator-=` are synthesized when `operator+=` is defined.\n
	/// `operator->` is synthesized when `operator*` is defined and returns a reference type.\n
	/// `operator[]` is synthesized when `operator+` and `operator*` are defined and/or synthesized.
	/// @tparam Iterator Iterator type to implement the interface for.
	/// @tparam DeducedBrackets Optional type used in automatic deduction of bracketed values in `operator[]`.
	template <typename Iterator, typename DeducedBrackets = void>
	class iterator_interface
	{
	  private:
		/// Gets a reference to the base iterator.
		/// @return Reference to the base iterator.
		[[nodiscard]] constexpr Iterator& base() noexcept
		{
			return static_cast<Iterator&>(*this);
		}

		/// Gets a reference to the base iterator.
		/// @return Reference to the base iterator.
		[[nodiscard]] constexpr const Iterator& base() const noexcept
		{
			return static_cast<const Iterator&>(*this);
		}

	  public:
		/// @cond implementation_details

		/// (no-op)
		[[nodiscard]] friend constexpr auto operator<=>(const iterator_interface& lhs, const iterator_interface& rhs) noexcept = default;

		/// (no-op)
		[[nodiscard]] friend constexpr bool operator==(const iterator_interface& lhs, const iterator_interface& rhs) noexcept = default;

		/// @endcond

		/// Synthesized post-incrementation operator.
		/// @param it Iterator to increment.
		/// @return Copy of `it` before it was incremented.
		friend constexpr Iterator operator++(iterator_interface& it, int) noexcept(noexcept(++it.base()))
			requires(requires { ++it.base(); })
		{
			Iterator copy{it.base()};
			++it.base();
			return copy;
		}

		/// Synthesized post-decrementation operator.
		/// @param it Iterator to decrement.
		/// @return Copy of `it` before it was decremented.
		friend constexpr Iterator operator--(iterator_interface& it, int) noexcept(noexcept(--it.base()))
			requires(requires { --it.base(); })
		{
			Iterator copy{it.base()};
			--it.base();
			return copy;
		}

		//

		/// Synthesized subtraction assignment operator.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Reference to `it`.
		template <typename Difference>
		friend constexpr Iterator& operator-=(iterator_interface& it, Difference diff) noexcept(noexcept(it.base() += -diff))
			requires(requires { it.base() += -diff; })
		{
			return it.base() += -diff;
		}

		/// Synthesized addition operator.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param it Iterator to add to.
		/// @param diff Amount of steps to advance by.
		/// @return Copy of `it` advanced by `diff`.
		template <typename Difference>
		[[nodiscard]] friend constexpr Iterator operator+(const iterator_interface& it, Difference diff)
			noexcept(noexcept(Iterator{it.base()} += diff))
			requires(requires { Iterator{it.base()} += diff; })
		{
			Iterator copy{it.base()};
			copy += diff;
			return copy;
		}

		/// Synthesized addition operator.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param diff Amount of steps to advance by.
		/// @param it Iterator to add to.
		/// @return Copy of `it` advanced by `diff`.
		template <typename Difference>
		[[nodiscard]] friend constexpr Iterator operator+(Difference diff, const iterator_interface& it)
			noexcept(noexcept(Iterator{it.base()} += diff))
			requires(requires { Iterator{it.base()} += diff; })
		{
			Iterator copy{it.base()};
			copy += diff;
			return copy;
		}

		/// Synthesized subtraction operator.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Copy of `it` subtracted by `diff`.
		template <typename Difference>
		[[nodiscard]] friend constexpr Iterator operator-(const iterator_interface& it, Difference diff)
			noexcept(noexcept(Iterator{it.base()} -= diff))
			requires(requires { Iterator{it.base()} -= diff; })
		{
			Iterator copy{it.base()};
			copy -= diff;
			return copy;
		}

		//

		/// Synthesized pointer dereferencing operator.
		/// @return Pointer to the dereferenced value.
		[[nodiscard]] constexpr auto operator->() const noexcept(noexcept(*base()))
			requires(requires {
				{ *base() } -> lvalue_reference;
			})
		{
			return &*base();
		}

		/// Synthesized subscripting operator.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param diff Amount of steps to advance by.
		/// @return `*(*this + diff)`.
		template <typename Difference = DeducedBrackets>
		[[nodiscard]] constexpr decltype(auto) operator[](Difference diff) const noexcept(noexcept(*(base() + diff)))
			requires(requires { *(base() + diff); })
		{
			return *(base() + diff);
		}
	};
} // namespace tr