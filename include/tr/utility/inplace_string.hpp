/// @file
/// @brief Provides `tr::basic_inplace_string`.

/// @ingroup containers
/// @defgroup inplace_string Inplace string
/// @brief Contains `tr::basic_inplace_string` and related functionality.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/iterator_concepts.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/pointer_iterator.hpp>

//

namespace tr
{
	/// @ingroup inplace_string
	/// @brief Inplace-allocated, fixed-capacity string.
	/// @details
	/// `tr::basic_inplace_string` mirrors the interface of `std::basic_string`, but its characters are stored inplace within the object.
	/// This eliminates any overhead or allocation uncertainty associated with dynamic allocation, but caps the maximum capacity of the
	/// container.
	///
	/// The invalidation rules of `tr::basic_inplace_string` largely mirror those of `std::basic_string`, but without the total invalidation
	/// when `std::basic_string` has to reallocate its buffer. The only exception to this is that moving `tr::basic_inplace_string` is done
	/// in linear time, as it is done identically to copying.
	/// @tparam CharT Character type used by the string.
	/// @tparam Capacity Maximum capacity of the string.
	/// @tparam Traits Character traits used by the string.
	template <typename CharT, usize Capacity, typename Traits = std::char_traits<CharT>>
	class basic_inplace_string
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Value type used by the container.
		using value_type = CharT;

		/// @brief Size type used by the container.
		using size_type = size_type_t<Capacity>;

		/// @brief Difference type used by the container.
		using difference_type = std::make_signed_t<size_type>;

		/// @brief Reference type used by the container.
		using reference = CharT&;

		/// @brief Constant reference type used by the container.
		using const_reference = const CharT&;

		/// @brief Pointer type used by the container.
		using pointer = CharT*;

		/// @brief Constant pointer used by the container.
		using const_pointer = const CharT*;

		/// @brief Constant iterator type used by the container.
		struct const_iterator : public pointer_iterator<const_iterator, const_pointer>
		{
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<const_iterator, const_pointer>::pointer_iterator;

			/// @}
		};

		/// @brief Iterator type used by the container.
		struct iterator : public pointer_iterator<iterator, pointer>
		{
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<iterator, pointer>::pointer_iterator;

			/// @}
			/// @name Observers
			/// @{

			/// @brief Converts the iterator into a constant iterator.
			/// @return Equivalent constant iterator.
			[[nodiscard]] constexpr operator const_iterator() const noexcept
			{
				return const_iterator{this->base()};
			}

			/// @}
		};

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty string.
		[[nodiscard]] constexpr basic_inplace_string() noexcept = default;

		/// @brief Constructs a string with filled characters.
		/// @param size Initial size of the string.
		/// @param character Character to fill the string with.
		/// @pre `%size` must not exceed the capacity of the string.
		/// @par Complexity
		/// Linear time with respect to `%size`.
		[[nodiscard]] explicit constexpr basic_inplace_string(size_type size, value_type character = CharT{0}) noexcept
			: m_size{size}
		{
			std::ranges::fill(begin(), end(), character);
		}

		/// @brief Constructs a string by copying the contents of a string literal.
		/// @tparam Size Size of the string literal.
		/// @param literal String literal to copy.
		/// @pre The size of the copied string literal (not counting the NUL terminator) must not exceed the capacity of the string.
		/// @par Complexity
		/// Linear time with respect to the size of the copied string literal.
		template <usize Size>
		[[nodiscard]] constexpr basic_inplace_string(const value_type (&literal)[Size]) noexcept
			: m_buffer{}
			, m_size{static_cast<size_type>(Size - 1)}
		{
			static_assert(Size - 1 <= Capacity, "Tried to initialize an inplace string with a string literal that would be too long.");

			std::ranges::copy(literal, literal + Size - 1, m_buffer.begin());
		}

		/// @brief Constructs a string by copying the contents of another string.
		/// @param string String to copy.
		/// @pre The size of the copied string must not exceed the capacity of the string.
		/// @par Complexity
		/// Linear time with respect to the size of the copied string.
		[[nodiscard]] explicit constexpr basic_inplace_string(std::basic_string_view<CharT, Traits> string) noexcept
			: m_buffer{}
			, m_size{static_cast<size_type>(string.size())}
		{
			TR_ASSERT(string.size() <= Capacity, "Tried to copy a string of size {} into an inplace string of capacity {}.", string.size(),
					  Capacity);
			std::ranges::copy(string, m_buffer.begin());
		}

		/// @brief Constructs a string by copying the contents of another string.
		/// @param string String to copy.
		/// @pre The size of the copied string must not exceed the capacity of the string.
		/// @par Complexity
		/// Linear time with respect to the size of the copied string.
		template <std::convertible_to<std::basic_string_view<CharT, Traits>> String>
		[[nodiscard]] explicit constexpr basic_inplace_string(String&& string) noexcept
			: basic_inplace_string{std::basic_string_view<CharT, Traits>{std::forward<String>(string)}}
		{
		}

		/// @brief Replaces the contents of the string with a copy of those of another string.
		/// @param string String to copy.
		/// @pre The size of the copied string must not exceed the capacity of the string.
		/// @par Complexity
		/// Linear time with respect to the size of the copied string.
		template <std::convertible_to<std::basic_string_view<CharT, Traits>> String>
		[[nodiscard]] constexpr basic_inplace_string& operator=(String&& string) noexcept
		{
			const std::string_view view{static_cast<std::string_view>(std::forward<String>(string))};
			m_size = view.size();
			std::ranges::copy(view, m_buffer.begin());
			return *this;
		}

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr reference at(size_type offset) noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to get out-of-bounds character {} in inplace string of size {}.", offset, m_size);

			return m_buffer[offset];
		}

		/// @brief Returns a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr const_reference at(size_type offset) const noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to get out-of-bounds character {} in inplace string of size {}.", offset, m_size);

			return m_buffer[offset];
		}

		/// @brief Returns a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr reference operator[](size_type offset) noexcept
		{
			return at(offset);
		}

		/// @brief Returns a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr const_reference operator[](size_type offset) const noexcept
		{
			return at(offset);
		}

		/// @brief Returns a reference to the first character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the first character in the string.
		[[nodiscard]] constexpr reference front() noexcept
		{
			TR_ASSERT(!empty(), "Tried to get front character of an empty inplace string.");

			return m_buffer[0];
		}

		/// @brief Returns a reference to the first character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the first character in the string.
		[[nodiscard]] constexpr const_reference front() const noexcept
		{
			TR_ASSERT(!empty(), "Tried to get front character of an empty inplace string.");

			return m_buffer[0];
		}

		/// @brief Returns a reference to the last character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the last character in the string.
		[[nodiscard]] constexpr reference back() noexcept
		{
			TR_ASSERT(!empty(), "Tried to get back character of an empty inplace string.");

			return m_buffer[size() - 1];
		}

		/// @brief Returns a reference to the last character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the last character in the string.
		[[nodiscard]] constexpr const_reference back() const noexcept
		{
			TR_ASSERT(!empty(), "Tried to get back character of an empty inplace string.");

			return m_buffer[size() - 1];
		}

		/// @brief Returns a pointer to the data of the string.
		/// @return Pointer to the data of the string.
		[[nodiscard]] constexpr pointer data() noexcept
		{
			return m_buffer.data();
		}

		/// @brief Returns a pointer to the data of the string.
		/// @return Pointer to the data of the string.
		[[nodiscard]] constexpr const_pointer data() const noexcept
		{
			return m_buffer.data();
		}

		/// @brief Returns a string view over the string.
		/// @return View over the string.
		[[nodiscard]] constexpr operator std::basic_string_view<CharT, Traits>() const noexcept
		{
			return std::basic_string_view<CharT, Traits>{m_buffer.data(), m_size};
		}

		/// @brief Returns a standard string copy of the string.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Standard string copy of the string.
		[[nodiscard]] constexpr explicit operator std::basic_string<CharT, Traits>() const
		{
			return std::basic_string<CharT, Traits>{std::basic_string_view<CharT, Traits>{*this}};
		}

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr iterator begin() noexcept
		{
			return iterator{m_buffer.data()};
		}

		/// @brief Returns an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return const_iterator{m_buffer.data()};
		}

		/// @brief Returns an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return const_iterator{m_buffer.data()};
		}

		/// @brief Returns an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		[[nodiscard]] constexpr iterator end() noexcept
		{
			return iterator{m_buffer.data() + m_size};
		}

		/// @brief Returns an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return const_iterator{m_buffer.data() + m_size};
		}

		/// @brief Returns an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return const_iterator{m_buffer.data() + m_size};
		}

		/// @}
		/// @name Capacity
		/// @{

		/// @brief Checks whether the string is empty.
		/// @return `true` if the string is empty, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_size == 0;
		}

		/// @brief Returns the size of the string.
		/// @return Size of the string.
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return m_size;
		}

		/// @brief Returns the maximum capacity of the string.
		/// @return Maximum capacity of the string.
		[[nodiscard]] static constexpr size_type max_size() noexcept
		{
			return Capacity;
		}

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Erases all characters from the string.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to characters stored in the string are invalidated.
		constexpr void clear() noexcept
		{
			m_size = 0;
		}

		/// @brief Appends a character to the string.
		/// @param character Character to append to the string.
		/// @pre The string must not already be at capacity.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		constexpr void push_back(value_type character) noexcept
		{
			TR_ASSERT(m_size < Capacity, "Tried to push back into an inplace string that is already at its capacity of {}.", Capacity);
			m_buffer[m_size++] = character;
		}

		/// @brief Appends a character to the string.
		/// @param character Character to append to the string.
		/// @pre The string must not already be at capacity.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		constexpr void append(value_type character) noexcept
		{
			push_back(character);
		}

		/// @brief Appends a character to the string.
		/// @param character Character to append to the string.
		/// @pre The string must not already be at capacity.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @return Reference to `*this`.
		constexpr basic_inplace_string& operator+=(value_type character) noexcept
		{
			push_back(character);
			return *this;
		}

		/// @brief Appends a range of characters to the string.
		/// @tparam Iterator Forward iterator type to an element convertible to `CharT`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param first, last Pair of iterators defining the range of characters to append.
		/// @pre The combined sizes of the string and `[first, last)` must not exceed the capacity of the string.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)`.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		template <forward_iterator_to<value_type> Iterator, std::sentinel_for<Iterator> Sentinel>
		constexpr void append(Iterator first, Sentinel last) noexcept(noexcept(std::ranges::copy(first, last, end())))
		{
			const ssize size{static_cast<ssize>(std::ranges::distance(first, last))};

			TR_ASSERT(static_cast<usize>(m_size + size) <= Capacity,
					  "Tried to do an append onto an inplace string that would put it past its capacity of {}.", Capacity);

			std::ranges::copy(first, last, end());
			m_size += static_cast<size_type>(size);
		}

		/// @brief Appends a substring to the string.
		/// @param string String to append.
		/// @pre The combined sizes of the string and `string` must not exceed the capacity of the string.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `string`.
		/// @return Reference to `*this`.
		constexpr basic_inplace_string& append(std::basic_string_view<CharT, Traits> string) noexcept
		{
			TR_ASSERT(static_cast<usize>(m_size + string.size()) <= Capacity,
					  "Tried to do an append onto an inplace string that would put it past its capacity of {}.", Capacity);

			std::ranges::copy(string, end());
			m_size += static_cast<size_type>(string.size());
			return *this;
		}

		/// @brief Appends a substring to the string.
		/// @param string String to append.
		/// @pre The combined sizes of the string and `string` must not exceed the capacity of the string.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `string`.
		/// @return Reference to `*this`.
		constexpr basic_inplace_string& operator+=(std::basic_string_view<CharT, Traits> string) noexcept
		{
			return append(string);
		}

		/// @brief Inserts a character into the string.
		/// @param where Position within the string to insert the character.
		/// @param character Character to insert into the string.
		/// @pre The string must not already be at capacity.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to characters stored at or after `where` are invalidated.
		/// @par Complexity
		/// At most linear time with respect to the size of the string.
		/// @return Iterator to the position where the character was inserted.
		constexpr iterator insert(const_iterator where, value_type character) noexcept
		{
			TR_ASSERT(m_size < Capacity, "Tried to insert into an inplace string that is already at its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to basic_inplace_string::insert.");

			const iterator location{begin() + (where - begin())};
			std::ranges::copy_backward(location, end(), end() + 1);
			*location = character;
			++m_size;
			return location;
		}

		/// @brief Inserts a range of characters into the string.
		/// @tparam Iterator Forward iterator type to an element convertible to `CharT`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param where Position within the string to insert the range.
		/// @param first, last Pair of iterators defining the range of characters to insert.
		/// @pre The combined sizes of the string and `[first, last)` must not exceed the capacity of the string.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to characters stored at or after `where` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)` and at most linear time with respect to the size of the string.
		/// @return Iterator to the position where the range was inserted.
		template <forward_iterator_to<value_type> Iterator, std::sentinel_for<Iterator> Sentinel>
		constexpr iterator insert(const_iterator where, Iterator first, Sentinel last)
			noexcept(noexcept(std::ranges::copy(first, last, begin() + (where - begin()))))
		{
			const ssize size{static_cast<ssize>(std::ranges::distance(first, last))};

			TR_ASSERT(static_cast<usize>(m_size + size) <= Capacity,
					  "Tried to do an insert into an inplace string that would put it past its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to basic_inplace_string::insert.");

			const iterator mut_where{begin() + (where - begin())};
			std::ranges::copy_backward(mut_where, end(), end() + size);
			std::ranges::copy(first, last, mut_where);
			m_size += static_cast<size_type>(size);
			return mut_where;
		}

		/// @brief Inserts a substring into the string.
		/// @param where Position within the string to insert the substring.
		/// @param string String to insert.
		/// @pre The combined sizes of the string and `string` must not exceed the capacity of the string.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to characters stored at or after `where` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `string` and at most linear time with respect to the size of the string.
		/// @return Iterator to the position where the substring was inserted.
		constexpr iterator insert(const_iterator where, std::basic_string_view<CharT, Traits> string) noexcept
		{
			TR_ASSERT(static_cast<usize>(m_size + string.size()) <= Capacity,
					  "Tried to do an insert into an inplace string that would put it past its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to basic_inplace_string::insert.");

			const iterator mut_where{begin() + (where - begin())};
			std::ranges::copy_backward(mut_where, end(), end() + string.size());
			std::ranges::copy(string, mut_where);
			m_size += static_cast<size_type>(string.size());
			return mut_where;
		}

		/// @brief Erases a character from the end of the string.
		/// @pre The string must not be empty.
		/// @par Invalidation
		/// Iterators and references to the previous last character in the string, as well as the `end()` iterator, are invalidated.
		constexpr void pop_back() noexcept
		{
			TR_ASSERT(!empty(), "Tried to pop back from an empty inplace string.");

			--m_size;
		}

		/// @brief Erases a character from the string.
		/// @param where Iterator to the character to erase.
		/// @pre `where` must not be equal to `end()`.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to characters stored at or after `where` are invalidated.
		/// @par Complexity
		/// At most linear with respect to the size of the string.
		/// @return Iterator to the position where the character was erased.
		constexpr iterator erase(const_iterator where) noexcept
		{
			TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to basic_inplace_string::erase.");

			const iterator mut_where{begin() + (where - begin())};
			std::ranges::copy(mut_where + 1, end(), mut_where);
			--m_size;
			return mut_where;
		}

		/// @brief Erases a portion of the string.
		/// @param first, last Pair of iterators defining the range of characters to erase.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to characters stored at or after `first` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)` and at most linear time with respect to the size of the string.
		/// @return Iterator to the position where the first character was erased.
		constexpr iterator erase(const_iterator first, const_iterator last) noexcept
		{
			TR_ASSERT(first >= begin() && first < this->end(), "Tried to pass an invalid start iterator to basic_inplace_string::erase.");
			TR_ASSERT(last >= begin() && last <= this->end(), "Tried to pass an invalid end iterator to basic_inplace_string::erase.");

			const iterator mut_start{begin() + (first - begin())};
			std::ranges::copy(mut_start + (last - first), this->end(), mut_start);
			m_size -= static_cast<size_type>(last - first);
			return mut_start;
		}

		/// @brief Resizes the string, filling it with a character if necessary.
		/// @param size New size of the string.
		/// @param chr Character to fill the string with.
		/// @pre `%size` must be less than or equal to the capacity of the string.
		/// @par Invalidation
		/// If `%size` is less than the current size of the string, all iterators (including `end()`) and references to characters at or
		/// after `begin() + %size` are invalidated. Otherwise, if `%size` is greater than the current size of the string, only the `end()`
		/// iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the number of newly added characters.
		constexpr void resize(size_type size, value_type chr = '\0') noexcept
		{
			TR_ASSERT(size <= Capacity, "Tried to resize an inplace string past its capacity of {}.", Capacity);

			for (size_type i = m_size; i < size; ++i) {
				m_buffer[i] = chr;
			}
			m_size = size;
		}

		/// @}

	  private:
		/// @brief String character buffer.
		std::array<value_type, Capacity> m_buffer{};

		/// @brief Size of the string.
		size_type m_size{0};
	};

	/// @ingroup inplace_string
	/// @brief Inplace-allocated, fixed-capacity 8-bit character string.
	/// @tparam Capacity Maximum capacity of the string.
	template <usize Capacity>
	using inplace_string = basic_inplace_string<char, Capacity>;

	//

	/// @ingroup inplace_string
	/// @brief Compares two inplace strings.
	/// @tparam CharT Character type used by the strings.
	/// @tparam LhsCapacity, RhsCapacity Maximum capacity of the inplace strings.
	/// @tparam Traits Character traits used by the strings.
	/// @param lhs, rhs Strings to compare.
	/// @par Complexity
	/// Linear time with respect to `lhs.size()` and `rhs.size()`.
	/// @return Lexicographical ordering of the strings.
	template <typename CharT, usize LhsCapacity, usize RhsCapacity, typename Traits>
	[[nodiscard]] constexpr std::strong_ordering operator<=>(const basic_inplace_string<CharT, LhsCapacity, Traits>& lhs,
															 const basic_inplace_string<CharT, RhsCapacity, Traits>& rhs) noexcept
	{
		return std::basic_string_view<CharT, Traits>{lhs} <=> std::basic_string_view<CharT, Traits>{rhs};
	}

	/// @ingroup inplace_string
	/// @brief Compares an inplace string with another string.
	/// @tparam CharT Character type used by the strings.
	/// @tparam Capacity Maximum capacity of the inplace string.
	/// @tparam Traits Character traits used by the strings.
	/// @param lhs, rhs Strings to compare.
	/// @par Complexity
	/// Linear time with respect to `lhs.size()` and `rhs.size()`.
	/// @return Lexicographical ordering of the strings.
	template <typename CharT, tr::usize Capacity, typename Traits>
	[[nodiscard]] constexpr std::strong_ordering operator<=>(const basic_inplace_string<CharT, Capacity, Traits>& lhs,
															 std::type_identity_t<std::basic_string_view<CharT, Traits>> rhs) noexcept
	{
		return std::basic_string_view<CharT, Traits>{lhs} <=> rhs;
	}

	/// @ingroup inplace_string
	/// @brief Checks two inplace strings for equality.
	/// @tparam CharT Character type used by the strings.
	/// @tparam LhsCapacity, RhsCapacity Maximum capacity of the inplace strings.
	/// @tparam Traits Character traits used by the strings.
	/// @param lhs, rhs Strings to compare.
	/// @par Complexity
	/// Linear time with respect to `lhs.size()` and `rhs.size()`.
	/// @return `true` if the contents of the strings are identical, `false` otherwise.
	template <typename CharT, usize LhsCapacity, usize RhsCapacity, typename Traits>
	[[nodiscard]] constexpr bool operator==(const basic_inplace_string<CharT, LhsCapacity, Traits>& lhs,
											const basic_inplace_string<CharT, RhsCapacity, Traits>& rhs) noexcept
	{
		return std::basic_string_view<CharT, Traits>{lhs} == std::basic_string_view<CharT, Traits>{rhs};
	}

	/// @ingroup inplace_string
	/// @brief Checks an inplace string with another string for equality.
	/// @tparam CharT Character type used by the strings.
	/// @tparam Capacity Maximum capacity of the inplace string.
	/// @tparam Traits Character traits used by the strings.
	/// @param lhs, rhs Strings to compare.
	/// @par Complexity
	/// Linear time with respect to `lhs.size()` and `rhs.size()`.
	/// @return `true` if the contents of the strings are identical, `false` otherwise.
	template <typename CharT, tr::usize Capacity, typename Traits>
	[[nodiscard]] constexpr bool operator==(const basic_inplace_string<CharT, Capacity, Traits>& lhs,
											std::type_identity_t<std::basic_string_view<CharT, Traits>> rhs) noexcept
	{
		return std::basic_string_view<CharT, Traits>{lhs} == rhs;
	}
} // namespace tr

//

/// @ingroup inplace_string
/// @brief Formatter for inplace strings.
template <typename CharT, tr::usize Capacity, typename Traits>
struct std::formatter<tr::basic_inplace_string<CharT, Capacity, Traits>, CharT>
	: std::formatter<std::basic_string_view<CharT, Traits>, CharT>
{
};

#ifndef TR_DOXYGEN
/// @ingroup inplace_string
/// @brief Inplace string hasher.
template <typename CharT, tr::usize Capacity, typename Traits>
struct boost::hash<tr::basic_inplace_string<CharT, Capacity, Traits>> : boost::hash<std::basic_string_view<CharT, Traits>>
{
};
#endif