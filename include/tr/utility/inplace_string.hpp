/// @file
/// @brief Provides `tr::basic_inplace_string`.

#pragma once
#include <tr/utility/binary_io_specializations.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/pointer_iterator.hpp>

//

namespace tr
{
	/// Inplace-allocated fixed-capacity string.
	/// @details
	/// `tr::inplace_string` mirrors the interface of a standard string, but its characters are stored inplace within the object.
	/// Additionally, unlike a standard string, it is not guaranteed to be NUL-terminated.
	/// @tparam CharT Character type used by the string.
	/// @tparam Capacity Maximum capacity of the string.
	/// @tparam Traits Character traits used by the string.
	template <typename CharT, usize Capacity, typename Traits = std::char_traits<CharT>>
	class basic_inplace_string
	{
	  public:
		/// Value type used by the container.
		using value_type = CharT;

		/// Size type used by the container.
		using size_type = size_type_t<Capacity>;

		/// Difference type used by the container.
		using difference_type = std::make_signed_t<size_type>;

		/// Reference type used by the container.
		using reference = CharT&;

		/// Constant reference type used by the container.
		using const_reference = const CharT&;

		/// Pointer type used by the container.
		using pointer = CharT*;

		/// Constant pointer used by the container.
		using const_pointer = const CharT*;

		/// Constant iterator type used by the container.
		struct const_iterator : public pointer_iterator<const_iterator, const_pointer>
		{
			using pointer_iterator<const_iterator, const_pointer>::pointer_iterator;
		};

		/// Iterator type used by the container.
		struct iterator : public pointer_iterator<iterator, pointer>
		{
			using pointer_iterator<iterator, pointer>::pointer_iterator;

			/// Converts the iterator into a constant iterator.
			/// @return Constant iterator equivalent to `*this`.
			[[nodiscard]] constexpr operator const_iterator() const noexcept
			{
				return const_iterator{this->base()};
			}
		};

		/// @name Constructors
		/// @{

		/// Creates an empty string.
		[[nodiscard]] constexpr basic_inplace_string() noexcept = default;

		/// Creates a string with a starting size.
		/// @param size Initial size of the string.
		/// @param chr Character to fill the string with.
		[[nodiscard]] explicit constexpr basic_inplace_string(size_type size, value_type chr = CharT{0}) noexcept
			: m_size{size}
		{
			std::ranges::fill(begin(), end(), chr);
		}

		/// Creates a string by copying from a string literal.
		/// @param literal String literal to copy.
		template <usize Size>
		[[nodiscard]] constexpr basic_inplace_string(const value_type (&literal)[Size]) noexcept
			: m_buffer{}
			, m_size{static_cast<size_type>(Size - 1)}
		{
			static_assert(Size - 1 <= Capacity, "Tried to initialize an inplace string with a string literal that would be too long.");
			std::ranges::copy(literal, literal + Size - 1, m_buffer.begin());
		}

		/// Creates a string by copying from another string.
		/// @param str String to copy.
		/// @pre `str.size()` must be less than or equal to `Capacity`.
		[[nodiscard]] explicit constexpr basic_inplace_string(std::basic_string_view<CharT, Traits> str) noexcept
			: m_buffer{}
			, m_size{static_cast<size_type>(str.size())}
		{
			TR_ASSERT(str.size() <= Capacity, "Tried to copy a string of size {} into an inplace string of capacity {}.", str.size(),
					  Capacity);
			std::ranges::copy(str, m_buffer.begin());
		}

		/// Creates a string by copying from another string.
		/// @param str String to copy.
		/// @pre `str.size()` must be less than or equal to `Capacity`.
		template <std::convertible_to<std::basic_string_view<CharT, Traits>> String>
		[[nodiscard]] explicit constexpr basic_inplace_string(String&& str) noexcept
			: basic_inplace_string{std::basic_string_view<CharT, Traits>{str}}
		{
		}

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets a string view over the string.
		/// @return View over the string.
		[[nodiscard]] constexpr operator std::basic_string_view<CharT, Traits>() const noexcept
		{
			return std::basic_string_view<CharT, Traits>{m_buffer.data(), m_size};
		}

		/// Gets a standard string copy of the string.
		/// @return Standard string copy of the string.
		[[nodiscard]] constexpr explicit operator std::basic_string<CharT, Traits>() const
		{
			return std::basic_string<CharT, Traits>{std::basic_string_view<CharT, Traits>{*this}};
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two inplace strings for equality.
		/// @param rhs String to compare with.
		/// @return Ordering of the strings.
		[[nodiscard]] constexpr std::strong_ordering operator<=>(const basic_inplace_string& rhs) const noexcept
		{
			return std::basic_string_view<CharT, Traits>{*this} <=> std::basic_string_view<CharT, Traits>{rhs};
		}

		/// Compares two inplace strings for equality.
		/// @param rhs String to compare with.
		/// @return Whether the strings are identical.
		[[nodiscard]] constexpr bool operator==(const basic_inplace_string& rhs) const noexcept
		{
			return std::basic_string_view<CharT, Traits>{*this} == std::basic_string_view<CharT, Traits>{rhs};
		}

		/// Compares two strings.
		/// @param rhs String to compare with.
		/// @return Ordering of the strings.
		template <std::convertible_to<std::basic_string_view<CharT, Traits>> String>
		[[nodiscard]] constexpr std::strong_ordering operator<=>(const String& rhs) const noexcept
		{
			return std::basic_string_view<CharT, Traits>{*this} <=> std::basic_string_view<CharT, Traits>{rhs};
		}

		/// Compares two strings for equality.
		/// @param rhs String to compare with.
		/// @return Whether the strings are identical.
		template <std::convertible_to<std::basic_string_view<CharT, Traits>> String>
		[[nodiscard]] constexpr bool operator==(const String& rhs) const noexcept
		{
			return std::basic_string_view<CharT, Traits>{*this} == std::basic_string_view<CharT, Traits>{rhs};
		}

		/// @}
		/// @name Access
		/// @{

		/// Gets a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr reference operator[](size_type offset) noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in inplace string of size {}.", offset, m_size);

			return m_buffer[offset];
		}

		/// Gets a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr const_reference operator[](size_type offset) const noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to get out-of-bounds element {} in inplace string of size {}.", offset, m_size);

			return m_buffer[offset];
		}

		/// Gets a reference to the first character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the first character in the string.
		[[nodiscard]] constexpr reference front() noexcept
		{
			TR_ASSERT(!empty(), "Tried to get front element of an empty inplace string.");

			return *begin();
		}

		/// Gets a reference to the first character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the first character in the string.
		[[nodiscard]] constexpr const_reference front() const noexcept
		{
			TR_ASSERT(!empty(), "Tried to get front element of an empty inplace string.");

			return *begin();
		}

		/// Gets a reference to the last character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the last character in the string.
		[[nodiscard]] constexpr reference back() noexcept
		{
			TR_ASSERT(!empty(), "Tried to get back element of an empty inplace string.");

			return *std::prev(end());
		}

		/// Gets a reference to the last character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the last character in the string.
		[[nodiscard]] constexpr const_reference back() const noexcept
		{
			TR_ASSERT(!empty(), "Tried to get back element of an empty inplace string.");

			return *std::prev(end());
		}

		/// Gets a pointer to the data of the string.
		/// @return Pointer to the data of the string.
		[[nodiscard]] constexpr pointer data() noexcept
		{
			return m_buffer.data();
		}

		/// Gets a pointer to the data of the string.
		/// @return Pointer to the data of the string.
		[[nodiscard]] constexpr const_pointer data() const noexcept
		{
			return m_buffer.data();
		}

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr iterator begin() noexcept
		{
			return iterator{m_buffer.data()};
		}

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return const_iterator{m_buffer.data()};
		}

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return const_iterator{m_buffer.data()};
		}

		/// Gets an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		[[nodiscard]] constexpr iterator end() noexcept
		{
			return iterator{m_buffer.data() + m_size};
		}

		/// Gets an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return const_iterator{m_buffer.data() + m_size};
		}

		/// Gets an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return const_iterator{m_buffer.data() + m_size};
		}

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the string is empty.
		/// @return `true` if the string is empty, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_size == 0;
		}

		/// Gets the current size of the string.
		/// @return Size of the string.
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return m_size;
		}

		/// Gets the maximum capacity of the string.
		/// @return Maximum capacity of the string.
		[[nodiscard]] static constexpr size_type max_size() noexcept
		{
			return Capacity;
		}

		/// @}
		/// @name Manipulation
		/// @{

		/// Erases all characters from the string.
		constexpr void clear() noexcept
		{
			m_size = 0;
		}

		/// Appends a character to the string.
		/// @param chr Character to append to the string.
		/// @pre The size of the string must be less than `Capacity`.
		constexpr void push_back(value_type chr) noexcept
		{
			TR_ASSERT(m_size < Capacity, "Tried to push back into an inplace string that is already at its capacity of {}.", Capacity);
			m_buffer[m_size++] = chr;
		}

		/// Appends a character to the string.
		/// @param chr Character to append to the string.
		/// @pre The size of the string must be less than `Capacity`.
		constexpr void append(value_type chr) noexcept
		{
			push_back(chr);
		}

		/// Appends a character to the string.
		/// @param chr Character to append to the string.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to `*this`.
		constexpr basic_inplace_string& operator+=(value_type chr) noexcept
		{
			push_back(chr);
			return *this;
		}

		/// Appends a range to the string.
		/// @tparam Iterator Forward iterator type to an element convertible to `char`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param first Beginning iterator of the range to append.
		/// @param last Ending iterator of the range to append.
		/// @pre `this->size() + std::distance(begin, end)` must be less than or equal to `Capacity`.
		template <forward_iterator_to_convertible_to<value_type> Iterator, std::sentinel_for<Iterator> Sentinel>
		constexpr void append(Iterator first, Sentinel last) noexcept(noexcept(std::ranges::copy(first, last, end())))
		{
			std::ranges::copy(first, last, end());
			m_size = m_size + std::ranges::distance(first, last);
		}

		/// Appends a substring to the string.
		/// @pre `this->size() + str.size()` must be less than or equal to `Capacity`.
		constexpr void append(std::basic_string_view<CharT, Traits> str) noexcept
		{
			TR_ASSERT(m_size + str.size() <= Capacity,
					  "Tried to do an append onto an inplace string that would put it past its capacity of {}.", Capacity);

			std::ranges::copy(str, end());
			m_size = m_size + str.size();
		}

		/// Appends a substring to the string.
		/// @pre `this->size() + str.size()` must be less than or equal to `Capacity`.
		/// @return Reference to `*this`.
		constexpr basic_inplace_string& operator+=(std::basic_string_view<CharT, Traits> str) noexcept
		{
			append(str);
			return *this;
		}

		/// Inserts a character into the string.
		/// @param where Position within the string to insert the character.
		/// @param chr Character to insert into the string.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the character was inserted.
		constexpr iterator insert(const_iterator where, value_type chr) noexcept
		{
			TR_ASSERT(m_size < Capacity, "Tried to insert into an inplace string that is already at its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to basic_inplace_string::insert.");

			const iterator location{begin() + (where - begin())};
			std::ranges::copy_backward(location, end(), end() + 1);
			*location = chr;
			++m_size;
			return location;
		}

		/// Inserts a range into the string.
		/// @tparam Iterator Forward iterator type to an element convertible to `char`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param where Position within the string to insert the range.
		/// @param first Beginning iterator of the range to insert.
		/// @param last Ending iterator of the range to insert.
		/// @pre `this->size() + std::distance(first, last)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the range was inserted.
		template <forward_iterator_to_convertible_to<value_type> Iterator, std::sentinel_for<Iterator> Sentinel>
		constexpr iterator insert(const_iterator where, Iterator first, Sentinel last)
			noexcept(noexcept(std::ranges::copy(first, last, begin() + (where - begin()))))
		{
			const usize size{usize(std::ranges::distance(first, last))};

			TR_ASSERT(m_size + size <= Capacity, "Tried to do an insert into an inplace string that would put it past its capacity of {}.",
					  Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to basic_inplace_string::insert.");

			const iterator mut_where{begin() + (where - begin())};
			std::ranges::copy_backward(mut_where, end(), end() + size);
			std::ranges::copy(first, last, mut_where);
			m_size = m_size + size;
			return mut_where;
		}

		/// Inserts a substring into the string.
		/// @param where Position within the string to insert the substring.
		/// @param str String to insert.
		/// @pre `this->size() + str.size()` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the substring was inserted.
		constexpr iterator insert(const_iterator where, std::basic_string_view<CharT, Traits> str) noexcept
		{
			TR_ASSERT(m_size + str.size() <= Capacity,
					  "Tried to do an insert into an inplace string that would put it past its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to basic_inplace_string::insert.");

			const iterator mut_where{begin() + (where - begin())};
			std::ranges::copy_backward(begin() + mut_where, end(), end() + str.size());
			std::ranges::copy(str, mut_where);
			m_size = m_size + str.size();
			return mut_where;
		}

		/// Erases a character from the end of the string.
		constexpr void pop_back() noexcept
		{
			if (!empty()) {
				--m_size;
			}
		}

		/// Erases a character from the string.
		/// @param where Iterator to the character to erase.
		/// @return Iterator to the position where the character was erased.
		constexpr iterator erase(const_iterator where) noexcept
		{
			TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to basic_inplace_string::erase.");

			const iterator mut_where{begin() + (where - begin())};
			std::ranges::copy(mut_where + 1, end(), mut_where);
			--m_size;
			return mut_where;
		}

		/// Erases a portion of the string.
		/// @param start Iterator to the first character to erase.
		/// @param end Iterator to one past the last charaater to erase.
		/// @return Iterator to the position where the first character was erased.
		constexpr iterator erase(const_iterator start, const_iterator end) noexcept
		{
			TR_ASSERT(start >= begin() && start < this->end(), "Tried to pass an invalid start iterator to basic_inplace_string::erase.");
			TR_ASSERT(end >= begin() && end <= this->end(), "Tried to pass an invalid end iterator to basic_inplace_string::erase.");

			const iterator mut_start{begin() + (start - begin())};
			std::ranges::copy(mut_start + (end - start), this->end(), mut_start);
			m_size = m_size - (end - start);
			return mut_start;
		}

		/// Resizes the string, filling it with a character if necessary.
		/// @param size New size of the string.
		/// @param chr Character to fill the string with.
		/// @pre `size` must be less than or equal to `Capacity`.
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
		/// String character buffer.
		std::array<value_type, Capacity> m_buffer{};

		/// Size of the string.
		size_type m_size{0};
	};

	/// Basic 8-bit character inplace string alias.
	/// @tparam Capacity Maximum capacity of the string.
	template <usize Capacity>
	using inplace_string = basic_inplace_string<char, Capacity>;

	/// @name Overloaded operators
	/// @{

	/// Output stream formatter for inplace strings.
	/// @tparam CharT Character type used by the string.
	/// @tparam Capacity Maximum capacity of the string.
	/// @tparam Traits Character traits used by the string.
	/// @param os Output stream.
	/// @param str String to output to the stream.
	/// @return Reference to `os`.
	template <typename CharT, tr::usize Capacity, typename Traits>
	std::ostream& operator<<(std::ostream& os, const basic_inplace_string<CharT, Capacity, Traits>& str)
	{
		return os << std::string_view{str};
	}

	/// @}
} // namespace tr

/// inplace string binary reader.
/// @tparam CharT Character type used by the string.
/// @tparam Capacity Maximum capacity of the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, tr::usize Capacity, typename Traits>
struct tr::binary_reader<tr::basic_inplace_string<CharT, Capacity, Traits>>
{
	/// Reads a variable from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::istream& is, basic_inplace_string<CharT, Capacity, Traits>& out) const
	{
		out.resize(read_binary<typename basic_inplace_string<CharT, Capacity, Traits>::size_type>(is));
		is.read(out.data(), out.size());
	}
};

/// inplace string binary writer.
/// @tparam CharT Character type used by the string.
/// @tparam Capacity Maximum capacity of the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, tr::usize Capacity, typename Traits>
struct tr::binary_writer<tr::basic_inplace_string<CharT, Capacity, Traits>>
{
	/// Writes a variable to a stream.
	/// @param os Output stream.
	/// @param in Variable to write to the stream.
	void operator()(std::ostream& os, const basic_inplace_string<CharT, Capacity, Traits>& in) const
	{
		write_binary(os, in.size());
		write_binary(os, std::span{in});
	}
};

/// Formatter for inplace strings.
/// @tparam CharT Character type used by the string.
/// @tparam Capacity Maximum capacity of the string.
/// @tparam Traits Character traits used by the string.
template <typename CharT, tr::usize Capacity, typename Traits>
struct std::formatter<tr::basic_inplace_string<CharT, Capacity, Traits>, CharT>
	: std::formatter<std::basic_string_view<CharT, Traits>, CharT>
{
};