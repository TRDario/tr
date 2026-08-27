/// @file
/// @brief Provides `tr::static_string`.

#pragma once
#include "pointer_iterator.hpp"

//

namespace tr
{
	/// Inplace-allocated fixed-capacity string.
	/// @details
	/// `tr::static_string` mirrors the interface of a standard string, but its characters are stored inplace within the object.
	/// Additionally, unlike a standard string, it is not guaranteed to be NUL-terminated.
	/// @tparam Capacity Maximum capacity of the string.
	template <usize Capacity>
	class static_string
	{
	  public:
		/// Value type used by the container.
		using value_type = char;

		/// Size type used by the container.
		using size_type = size_type_t<Capacity>;

		/// Difference type used by the container.
		using difference_type = std::make_signed_t<size_type>;

		/// Reference type used by the container.
		using reference = char&;

		/// Constant reference type used by the container.
		using const_reference = const char&;

		/// Pointer type used by the container.
		using pointer = char*;

		/// Constant pointer used by the container.
		using const_pointer = const char*;

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
			constexpr operator const_iterator() const;
		};

		/// @name Constructors
		/// @{

		/// Creates an empty string.
		constexpr static_string() = default;

		/// Creates a string with a starting size.
		/// @param size Initial size of the string.
		/// @param chr Character to fill the string with.
		explicit constexpr static_string(size_type size, char chr = '\0');

		/// Creates a string by copying from a string literal.
		/// @param literal String literal to copy.
		template <usize Size>
		constexpr static_string(const char (&literal)[Size]);

		/// Creates a string by copying from another string.
		/// @param str String to copy.
		/// @pre `str.size()` must be less than or equal to `Capacity`.
		constexpr static_string(std::string_view str);

		/// Creates a string by copying from another string.
		/// @param str String to copy.
		/// @pre `str.size()` must be less than or equal to `Capacity`.
		template <std::convertible_to<std::string_view> String>
		constexpr static_string(String&& str);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets a string view over the string.
		/// @return View over the string.
		constexpr operator std::string_view() const;

		/// Gets a standard string copy of the string.
		/// @return Standard string copy of the string.
		constexpr explicit operator std::string() const;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two static strings for equality.
		/// @param rhs String to compare with.
		/// @return Ordering of the strings.
		constexpr std::strong_ordering operator<=>(const static_string& rhs) const;

		/// Compares two static strings for equality.
		/// @param rhs String to compare with.
		/// @return Whether the strings are identical.
		constexpr bool operator==(const static_string& rhs) const;

		/// Compares two strings.
		/// @param rhs String to compare with.
		/// @return Ordering of the strings.
		template <std::convertible_to<std::string_view> String>
		constexpr std::strong_ordering operator<=>(const String& rhs) const;

		/// Compares two strings for equality.
		/// @param rhs String to compare with.
		/// @return Whether the strings are identical.
		template <std::convertible_to<std::string_view> String>
		constexpr bool operator==(const String& rhs) const;

		/// @}
		/// @name Access
		/// @{

		/// Gets a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		constexpr reference operator[](size_type offset);

		/// Gets a reference to a character in the string.
		/// @param offset Index of the character within the string.
		/// @pre `offset` must be less than the size of the string.
		/// @return Reference to a character within the string.
		constexpr const_reference operator[](size_type offset) const;

		/// Gets a reference to the first character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the first character in the string.
		constexpr reference front();

		/// Gets a reference to the first character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the first character in the string.
		constexpr const_reference front() const;

		/// Gets a reference to the last character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the last character in the string.
		constexpr reference back();

		/// Gets a reference to the last character in the string.
		/// @pre The string must not be empty.
		/// @return Reference to the last character in the string.
		constexpr const_reference back() const;

		/// Gets a pointer to the data of the string.
		/// @return Pointer to the data of the string.
		constexpr pointer data();

		/// Gets a pointer to the data of the string.
		/// @return Pointer to the data of the string.
		constexpr const_pointer data() const;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		constexpr iterator begin();

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		constexpr const_iterator begin() const;

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		constexpr const_iterator cbegin() const;

		/// Gets an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		constexpr iterator end();

		/// Gets an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		constexpr const_iterator end() const;

		/// Gets an iterator to the end of the string.
		/// @return Iterator to the end of the string.
		constexpr const_iterator cend() const;

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the string is empty.
		/// @return `true` if the string is empty, `false` otherwise.
		constexpr bool empty() const;

		/// Gets the current size of the string.
		/// @return Size of the string.
		constexpr size_type size() const;

		/// Gets the maximum capacity of the string.
		/// @return Maximum capacity of the string.
		static constexpr size_type max_size();

		/// @}
		/// @name Manipulation
		/// @{

		/// Erases all characters from the string.
		constexpr void clear();

		/// Appends a character to the string.
		/// @param chr Character to append to the string.
		/// @pre The size of the string must be less than `Capacity`.
		constexpr void push_back(char chr);

		/// Appends a character to the string.
		/// @param chr Character to append to the string.
		/// @pre The size of the string must be less than `Capacity`.
		constexpr void append(char chr);

		/// Appends a character to the string.
		/// @param chr Character to append to the string.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to `*this`.
		constexpr static_string& operator+=(char chr);

		/// Appends a range to the string.
		/// @tparam Iterator Character input iterator type.
		/// @param begin Beginning iterator of the range to append.
		/// @param end Ending iterator of the range to append.
		/// @pre `this->size() + std::distance(begin, end)` must be less than or equal to `Capacity`.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
		constexpr void append(Iterator begin, Iterator end);

		/// Appends a substring to the string.
		/// @pre `this->size() + str.size()` must be less than or equal to `Capacity`.
		constexpr void append(std::string_view str);

		/// Appends a substring to the string.
		/// @pre `this->size() + str.size()` must be less than or equal to `Capacity`.
		/// @return Reference to `*this`.
		constexpr static_string& operator+=(std::string_view str);

		/// Inserts a character into the string.
		/// @param where Position within the string to insert the character.
		/// @param chr Character to insert into the string.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the character was inserted.
		constexpr iterator insert(const_iterator where, char chr);

		/// Inserts a range into the string.
		/// @param where Position within the string to insert the range.
		/// @param first Beginning iterator of the range to insert.
		/// @param last Ending iterator of the range to insert.
		/// @pre `this->size() + std::distance(first, last)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the range was inserted.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
		constexpr iterator insert(const_iterator where, Iterator first, Iterator last);

		/// Inserts a substring into the string.
		/// @param where Position within the string to insert the substring.
		/// @param str String to insert.
		/// @pre `this->size() + str.size()` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the substring was inserted.
		constexpr iterator insert(const_iterator where, std::string_view str);

		/// Erases a character from the end of the string.
		constexpr void pop_back();

		/// Erases a character from the string.
		/// @param where Iterator to the character to erase.
		/// @return Iterator to the position where the character was erased.
		constexpr iterator erase(const_iterator where);

		/// Erases a portion of the string.
		/// @param start Iterator to the first character to erase.
		/// @param end Iterator to one past the last charaater to erase.
		/// @return Iterator to the position where the first character was erased.
		constexpr iterator erase(const_iterator start, const_iterator end);

		/// Resizes the string, filling it with a character if necessary.
		/// @param size New size of the string.
		/// @param chr Character to fill the string with.
		/// @pre `size` must be less than or equal to `Capacity`.
		constexpr void resize(size_type size, char chr = '\0');

		/// @}

	  private:
		/// String character buffer.
		std::array<char, Capacity> m_buffer{};

		/// Size of the string.
		size_type m_size{0};
	};

	/// @name Overloaded operators
	/// @{

	/// Output stream formatter for static strings.
	/// @tparam Capacity Maximum capacity of the string.
	/// @param os Output stream.
	/// @param str String to output to the stream.
	/// @return Reference to `os`.
	template <usize Capacity>
	inline std::ostream& operator<<(std::ostream& os, const static_string<Capacity>& str);

	/// @}
} // namespace tr

#include "impl/static_string.hpp" // IWYU pragma: export