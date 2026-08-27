/// @file
/// @brief Provides `tr::iterator_interface`.

#pragma once
#include "concepts.hpp"

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
		constexpr Iterator& base();

		/// Gets a reference to the base iterator.
		/// @return Reference to the base iterator.
		constexpr const Iterator& base() const;

	  public:
		/// @cond __hidden

		/// (no-op)
		friend constexpr auto operator<=>(const iterator_interface& lhs, const iterator_interface& rhs) = default;

		/// (no-op)
		friend constexpr bool operator==(const iterator_interface& lhs, const iterator_interface& rhs) = default;

		/// @endcond

		/// Synthesized post-incrementation operator.
		/// @tparam Iterator_ Iterator type to implement the interface for.
		/// @tparam DeducedBrackets_ Optional type used in automatic deduction of bracketed values in `operator[]`.
		/// @param it Iterator to increment.
		/// @return Copy of `it` before it was incremented.
		template <typename Iterator_, typename DeducedBrackets_>
		friend constexpr Iterator_ operator++(iterator_interface<Iterator_, DeducedBrackets_>& it, int)
			requires(requires { ++it.base(); });

		/// Synthesized post-decrementation operator.
		/// @tparam Iterator_ Iterator type to implement the interface for.
		/// @tparam DeducedBrackets_ Optional type used in automatic deduction of bracketed values in `operator[]`.
		/// @param it Iterator to decrement.
		/// @return Copy of `it` before it was decremented.
		template <typename Iterator_, typename DeducedBrackets_>
		friend constexpr Iterator_ operator--(iterator_interface<Iterator_, DeducedBrackets_>& it, int)
			requires(requires { --it.base(); });

		//

		/// Synthesized subtraction assignment operator.
		/// @tparam Iterator_ Iterator type to implement the interface for.
		/// @tparam DeducedBrackets_ Optional type used in automatic deduction of bracketed values in `operator[]`.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Reference to `it`.
		template <typename Iterator_, typename DeducedBrackets_, typename Difference>
		friend constexpr Iterator_& operator-=(iterator_interface<Iterator_, DeducedBrackets_>& it, Difference diff)
			requires(requires { it.base() += -diff; });

		/// Synthesized addition operator.
		/// @tparam Iterator_ Iterator type to implement the interface for.
		/// @tparam DeducedBrackets_ Optional type used in automatic deduction of bracketed values in `operator[]`.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param it Iterator to add to.
		/// @param diff Amount of steps to advance by.
		/// @return Copy of `it` advanced by `diff`.
		template <typename Iterator_, typename DeducedBrackets_, typename Difference>
		friend constexpr Iterator_ operator+(const iterator_interface<Iterator_, DeducedBrackets_>& it, Difference diff)
			requires(requires(Iterator_ i) { i += diff; });

		/// Synthesized addition operator.
		/// @tparam Iterator_ Iterator type to implement the interface for.
		/// @tparam DeducedBrackets_ Optional type used in automatic deduction of bracketed values in `operator[]`.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param diff Amount of steps to advance by.
		/// @param it Iterator to add to.
		/// @return Copy of `it` advanced by `diff`.
		template <typename Iterator_, typename DeducedBrackets_, typename Difference>
		friend constexpr Iterator_ operator+(Difference diff, const iterator_interface& it)
			requires(requires(Iterator_ i) { i += diff; });

		/// Synthesized subtraction operator.
		/// @tparam Iterator_ Iterator type to implement the interface for.
		/// @tparam DeducedBrackets_ Optional type used in automatic deduction of bracketed values in `operator[]`.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Copy of `it` subtracted by `diff`.
		template <typename Iterator_, typename DeducedBrackets_, typename Difference>
		friend constexpr Iterator_ operator-(const iterator_interface& it, Difference diff)
			requires(requires(Iterator_ i) { i -= diff; });

		//

		/// Synthesized pointer dereferencing operator.
		/// @return Pointer to the dereferenced value.
		constexpr auto operator->() const
			requires(requires {
				{ *base() } -> lvalue_reference;
			});

		/// Synthesized subscripting operator.
		/// @tparam Difference Difference type accepted by `operator+=`.
		/// @param diff Amount of steps to advance by.
		/// @return `*(*this + diff)`.
		template <typename Difference = DeducedBrackets>
		constexpr decltype(auto) operator[](Difference diff) const
			requires(requires(Iterator it) { it + diff; });
	};
} // namespace tr

#include "impl/iterator_interface.hpp" // IWYU pragma: export