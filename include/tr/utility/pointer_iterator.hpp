/// @file
/// @brief Provides an iterator wrapper around a pointer that ensures the iterator is unique to a class.

#pragma once
#include "concepts.hpp"

//

namespace tr
{
	/// CRTP base class that may be inherited from to define an iterator based on a wrapped pointer.
	/// @tparam Iterator Derived iterator type.
	/// @tparam Pointer Raw pointer type.
	template <typename Iterator, pointer Pointer>
	class pointer_iterator
	{
	  public:
		/// Difference type used by the iterator.
		using difference_type = std::pointer_traits<Pointer>::difference_type;

		/// Value type used by the iterator.
		using value_type = std::pointer_traits<Pointer>::element_type;

		/// Pointer type used by the iterator.
		using pointer = Pointer;

		/// Reference type used by the iterator.
		using reference = value_type&;

		/// Iterator category the iterator belongs to.
		using iterator_category = std::contiguous_iterator_tag;

		//

		/// Default-constructs an iterator.
		constexpr pointer_iterator() = default;

		/// Constructs an iterator.
		/// @param base Base pointer.
		explicit constexpr pointer_iterator(Pointer base);

		//

		/// Compares two iterators.
		/// @param lhs, rhs Compared iterators.
		/// @return Ordering of the iterators.
		friend constexpr std::strong_ordering operator<=>(pointer_iterator lhs, pointer_iterator rhs) = default;

		/// Compares two iterators for equality.
		/// @param lhs, rhs Compared iterators.
		/// @return Whether the iterators are equal.
		friend constexpr bool operator==(pointer_iterator lhs, pointer_iterator rhs) = default;

		//

		/// Dereferences the iterator.
		/// @return Reference to the dereferenced value.
		constexpr reference operator*() const;

		/// Dereferences the iterator.
		/// @return Pointer to the dereferenced value.
		constexpr pointer operator->() const;

		/// Dereferences the iterator with a subscript.
		/// @param diff Offset to the target value.
		/// @return Reference to the dereferenced value.
		constexpr reference operator[](difference_type diff) const;

		//

		/// Pre-increments the iterator.
		/// @return Reference to `*this`.
		constexpr Iterator& operator++();

		/// Post-increments the iterator.
		/// @return Copy of `*this` before it was incremented.
		constexpr Iterator operator++(int);

		/// Pre-decrements the iterator.
		/// @return Reference to `*this`.
		constexpr Iterator& operator--();

		/// Post-decrements the iterator.
		/// @return Copy of `*this` before it was decremented.
		constexpr Iterator operator--(int);

		//

		/// Advances the iterator.
		/// @param diff Amount of steps to advance by.
		/// @return Reference to `*this`.
		constexpr Iterator& operator+=(difference_type diff);

		/// Subtracts from the iterator.
		/// @param diff Amount of steps to subtract by.
		/// @return Reference to `*this`.
		constexpr Iterator& operator-=(difference_type diff);

		/// Adds to an iterator.
		/// @tparam Iterator_ Derived iterator type.
		/// @tparam Pointer_ Raw pointer type.
		/// @param it Iterator to add to.
		/// @param diff Amount of steps to advance by.
		/// @return Copy of `it` advanced by `diff`.
		template <typename Iterator_, tr::pointer Pointer_>
		friend constexpr Iterator_ operator+(const pointer_iterator<Iterator_, Pointer_>& it,
											 pointer_iterator<Iterator_, Pointer_>::difference_type diff);

		/// Adds to an iterator.
		/// @tparam Iterator_ Derived iterator type.
		/// @tparam Pointer_ Raw pointer type.
		/// @param diff Amount of steps to advance by.
		/// @param it Iterator to add to.
		/// @return Copy of `it` advanced by `diff`.
		template <typename Iterator_, tr::pointer Pointer_>
		friend constexpr Iterator_ operator+(pointer_iterator<Iterator_, Pointer_>::difference_type diff,
											 const pointer_iterator<Iterator_, Pointer_>& it);

		/// Subtracts from an iterator.
		/// @tparam Iterator_ Derived iterator type.
		/// @tparam Pointer_ Raw pointer type.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Copy of `it` subtracted by `diff`.
		template <typename Iterator_, tr::pointer Pointer_>
		friend constexpr Iterator_ operator-(const pointer_iterator<Iterator_, Pointer_>& it,
											 pointer_iterator<Iterator_, Pointer_>::difference_type diff);

		/// Gets the difference between two iterators.
		/// @tparam Iterator_ Derived iterator type.
		/// @tparam Pointer_ Raw pointer type.
		/// @param lhs, rhs Iterators to get the difference between.
		/// @return Distance between `lhs` and `rhs`.
		template <typename Iterator_, tr::pointer Pointer_>
		friend constexpr pointer_iterator<Iterator_, Pointer_>::difference_type operator-(
			pointer_iterator<Iterator_, Pointer_> lhs, std::type_identity_t<pointer_iterator<Iterator_, Pointer_>> rhs);

		//

		/// Unwraps the iterator
		/// @return Base pointer.
		Pointer base() const;

	  private:
		/// Base pointer.
		Pointer m_base;
	};

} // namespace tr

#include "impl/pointer_iterator.hpp" // IWYU pragma: export