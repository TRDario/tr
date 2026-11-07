/// @file
/// @brief Provides `tr::pointer_iterator`.

#pragma once
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief CRTP base class that may be inherited from to define an iterator based on a wrapped pointer.
	/// @details This may be used to ensure that an iterator is unique to a class.
	/// @tparam Iterator Derived iterator type.
	/// @tparam Pointer Raw pointer type.
	template <typename Iterator, typename Pointer>
		requires std::is_pointer_v<Pointer>
	class pointer_iterator
	{
	  public:
		/// @name Iterator traits
		/// @{

		/// @brief Difference type used by the iterator.
		using difference_type = std::pointer_traits<Pointer>::difference_type;

		/// @brief Value type used by the iterator.
		using value_type = std::pointer_traits<Pointer>::element_type;

		/// @brief Pointer type used by the iterator.
		using pointer = Pointer;

		/// @brief Reference type used by the iterator.
		using reference = value_type&;

		/// @brief Iterator category of the iterator.
		using iterator_category = std::random_access_iterator_tag;

		/// @brief Iterator concept of the iterator.
		using iterator_concept = std::contiguous_iterator_tag;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a singular iterator.
		[[nodiscard]] constexpr pointer_iterator() noexcept = default;

		/// @brief Constructs an iterator.
		/// @param base Base pointer.
		[[nodiscard]] explicit constexpr pointer_iterator(Pointer base) noexcept
			: m_base{base}
		{
		}

		/// @}

		/// @brief Compares two iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		[[nodiscard]] friend constexpr std::strong_ordering operator<=>(pointer_iterator lhs, pointer_iterator rhs) noexcept = default;

		/// @brief Checks two iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return `true` if the iterators are equal, `false` otherwise.
		[[nodiscard]] friend constexpr bool operator==(pointer_iterator lhs, pointer_iterator rhs) noexcept = default;

		//

		/// @brief Dereferences the iterator.
		/// @return Reference to the dereferenced value.
		[[nodiscard]] constexpr reference operator*() const noexcept
		{
			TR_ASSERT(m_base, "Tried to dereference singular pointer iterator.");

			return *m_base;
		}

		/// @brief Dereferences the iterator.
		/// @return Pointer to the dereferenced value.
		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
			TR_ASSERT(m_base, "Tried to dereference singular pointer iterator.");

			return m_base;
		}

		/// @brief Dereferences the iterator with a subscript.
		/// @param diff Offset to the target value.
		/// @return Reference to the dereferenced value.
		[[nodiscard]] constexpr reference operator[](difference_type diff) const noexcept
		{
			TR_ASSERT(m_base, "Tried to dereference singular pointer iterator.");

			return m_base[diff];
		}

		//

		/// @brief Pre-increments the iterator.
		/// @return Reference to `*this`.
		constexpr Iterator& operator++() noexcept
		{
			TR_ASSERT(m_base, "Tried to increment singular pointer iterator.");

			++m_base;
			return static_cast<Iterator&>(*this);
		}

		/// @brief Post-increments the iterator.
		/// @return Copy of `*this` before it was incremented.
		constexpr Iterator operator++(int) noexcept
		{
			TR_ASSERT(m_base, "Tried to increment singular pointer iterator.");

			const Iterator copy{static_cast<Iterator&>(*this)};
			++m_base;
			return copy;
		}

		/// @brief Pre-decrements the iterator.
		/// @return Reference to `*this`.
		constexpr Iterator& operator--() noexcept
		{
			TR_ASSERT(m_base, "Tried to decrement singular pointer iterator.");

			--m_base;
			return static_cast<Iterator&>(*this);
		}

		/// @brief Post-decrements the iterator.
		/// @return Copy of `*this` before it was decremented.
		constexpr Iterator operator--(int) noexcept
		{
			TR_ASSERT(m_base, "Tried to decrement singular pointer iterator.");

			const Iterator copy{static_cast<Iterator&>(*this)};
			--m_base;
			return copy;
		}

		//

		/// @brief Advances the iterator.
		/// @param diff Amount of steps to advance by.
		/// @return Reference to `*this`.
		constexpr Iterator& operator+=(difference_type diff) noexcept
		{
			TR_ASSERT(m_base, "Tried to advance singular pointer iterator.");

			m_base += diff;
			return static_cast<Iterator&>(*this);
		}

		/// @brief Subtracts from the iterator.
		/// @param diff Amount of steps to subtract by.
		/// @return Reference to `*this`.
		constexpr Iterator& operator-=(difference_type diff) noexcept
		{
			TR_ASSERT(m_base, "Tried to advance singular pointer iterator.");

			m_base -= diff;
			return static_cast<Iterator&>(*this);
		}

		/// @brief Adds to an iterator.
		/// @param it Iterator to add to.
		/// @param diff Amount of steps to advance by.
		/// @return Copy of `it` advanced by `diff`.
		[[nodiscard]] friend constexpr Iterator operator+(const pointer_iterator& it, pointer_iterator::difference_type diff) noexcept
		{
			TR_ASSERT(it.m_base, "Tried to perform addition with singular pointer iterator.");

			Iterator copy{static_cast<const Iterator&>(it)};
			copy += diff;
			return copy;
		}

		/// @brief Adds to an iterator.
		/// @param diff Amount of steps to advance by.
		/// @param it Iterator to add to.
		/// @return Copy of `it` advanced by `diff`.
		[[nodiscard]] friend constexpr Iterator operator+(pointer_iterator::difference_type diff, const pointer_iterator& it) noexcept
		{
			return it + diff;
		}

		/// @brief Subtracts from an iterator.
		/// @param it Iterator to subtract from.
		/// @param diff Amount of steps to subtract by.
		/// @return Copy of `it` subtracted by `diff`.
		[[nodiscard]] friend constexpr Iterator operator-(const pointer_iterator& it, pointer_iterator::difference_type diff) noexcept
		{
			TR_ASSERT(it.m_base, "Tried to perform subtraction with singular pointer iterator.");

			Iterator copy{static_cast<const Iterator&>(it)};
			copy -= diff;
			return copy;
		}

		/// @brief Returns the difference between two iterators.
		/// @param lhs, rhs Iterators to get the difference between.
		/// @return Distance between `lhs` and `rhs`.
		[[nodiscard]] friend constexpr pointer_iterator::difference_type operator-(pointer_iterator lhs,
																				   std::type_identity_t<pointer_iterator> rhs) noexcept
		{
			TR_ASSERT(lhs.m_base, "Tried to perform subtraction with singular pointer iterator.");

			return lhs.m_base - rhs.m_base;
		}

		//

		/// @brief Unwraps the iterator.
		/// @return Base pointer.
		[[nodiscard]] Pointer base() const noexcept
		{
			return m_base;
		}

	  private:
		/// @brief Base pointer.
		Pointer m_base;
	};
} // namespace tr