/// @file
/// @brief Provides `tr::pointer_iterator`.

#pragma once
#include <tr/utility/concepts.hpp>

//

namespace tr
{
	/// CRTP base class that may be inherited from to define an iterator based on a wrapped pointer.
	/// @details This may be used to ensure that an iterator is unique to a class.
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
		[[nodiscard]] constexpr pointer_iterator() noexcept = default;

		/// Constructs an iterator.
		/// @param base Base pointer.
		[[nodiscard]] explicit constexpr pointer_iterator(Pointer base) noexcept
			: m_base{base}
		{
		}

		//

		/// Compares two iterators.
		/// @param lhs, rhs Compared iterators.
		/// @return Ordering of the iterators.
		[[nodiscard]] friend constexpr std::strong_ordering operator<=>(pointer_iterator lhs, pointer_iterator rhs) noexcept = default;

		/// Compares two iterators for equality.
		/// @param lhs, rhs Compared iterators.
		/// @return Whether the iterators are equal.
		[[nodiscard]] friend constexpr bool operator==(pointer_iterator lhs, pointer_iterator rhs) noexcept = default;

		//

		/// Dereferences the iterator.
		/// @return Reference to the dereferenced value.
		[[nodiscard]] constexpr reference operator*() const noexcept
		{
			return *m_base;
		}

		/// Dereferences the iterator.
		/// @return Pointer to the dereferenced value.
		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
			return m_base;
		}

		/// Dereferences the iterator with a subscript.
		/// @param diff Offset to the target value.
		/// @return Reference to the dereferenced value.
		[[nodiscard]] constexpr reference operator[](difference_type diff) const noexcept
		{
			return m_base[diff];
		}

		//

		/// Pre-increments the iterator.
		/// @return Reference to `*this`.
		constexpr Iterator& operator++() noexcept
		{
			++m_base;
			return static_cast<Iterator&>(*this);
		}

		/// Post-increments the iterator.
		/// @return Copy of `*this` before it was incremented.
		constexpr Iterator operator++(int) noexcept
		{
			const Iterator copy{static_cast<Iterator&>(*this)};
			++m_base;
			return copy;
		}

		/// Pre-decrements the iterator.
		/// @return Reference to `*this`.
		constexpr Iterator& operator--() noexcept
		{
			--m_base;
			return static_cast<Iterator&>(*this);
		}

		/// Post-decrements the iterator.
		/// @return Copy of `*this` before it was decremented.
		constexpr Iterator operator--(int) noexcept
		{
			const Iterator copy{static_cast<Iterator&>(*this)};
			--m_base;
			return copy;
		}

		//

		/// Advances the iterator.
		/// @param diff Amount of steps to advance by.
		/// @return Reference to `*this`.
		constexpr Iterator& operator+=(difference_type diff) noexcept
		{
			m_base += diff;
			return static_cast<Iterator&>(*this);
		}

		/// Subtracts from the iterator.
		/// @param diff Amount of steps to subtract by.
		/// @return Reference to `*this`.
		constexpr Iterator& operator-=(difference_type diff) noexcept
		{
			m_base -= diff;
			return static_cast<Iterator&>(*this);
		}

		/// Adds to an iterator.
		/// @param it Iterator to add to.
		/// @param diff Amount of steps to advance by.
		/// @return Copy of `it` advanced by `diff`.
		[[nodiscard]] friend constexpr Iterator operator+(const pointer_iterator& it, pointer_iterator::difference_type diff) noexcept
		{
			Iterator copy{static_cast<const Iterator&>(it)};
			copy += diff;
			return copy;
		}

		/// Adds to an iterator.
		/// @param diff Amount of steps to advance by.
		/// @param it Iterator to add to.
		/// @return Copy of `it` advanced by `diff`.
		[[nodiscard]] friend constexpr Iterator operator+(pointer_iterator::difference_type diff, const pointer_iterator& it) noexcept
		{
			return it + diff;
		}

		/// Subtracts from an iterator.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Copy of `it` subtracted by `diff`.
		[[nodiscard]] friend constexpr Iterator operator-(const pointer_iterator& it, pointer_iterator::difference_type diff) noexcept
		{
			Iterator copy{static_cast<const Iterator&>(it)};
			copy -= diff;
			return copy;
		}

		/// Gets the difference between two iterators.
		/// @param lhs, rhs Iterators to get the difference between.
		/// @return Distance between `lhs` and `rhs`.
		[[nodiscard]] friend constexpr pointer_iterator::difference_type operator-(pointer_iterator lhs,
																				   std::type_identity_t<pointer_iterator> rhs) noexcept
		{
			return lhs.m_base - rhs.m_base;
		}

		//

		/// Unwraps the iterator
		/// @return Base pointer.
		[[nodiscard]] Pointer base() const noexcept
		{
			return m_base;
		}

	  private:
		/// Base pointer.
		Pointer m_base;
	};

} // namespace tr