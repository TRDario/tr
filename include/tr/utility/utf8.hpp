/// @file
/// @brief Provides UTF-8-handling utilities

#pragma once
#include "iterator.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr
{
	/// Datatype used for Unicode codepoints.
	using codepoint = u32;
} // namespace tr

/// Namespace containing UTF-8-related functionality.
namespace tr::utf8
{
	/// Supported input iterator type.
	template <typename T>
	concept input_iterator = std::input_iterator<T> && std::same_as<std::iter_value_t<T>, char>;

	/// Supported output string type.
	template <typename T>
	concept output_string = requires(T& str, T::iterator it, const std::array<char, 4>& chars) {
		{ str.empty() } -> std::same_as<bool>;
		str.insert(it, '\0');
		str.insert(it, chars.begin(), chars.end());
		str.erase(it);
		str.erase(it, it + 4);
		str.pop_back();
	};

	/// @name UTF-8 operations
	/// @{

	/// Converts a UTF-8 sequence to a Unicode codepoint.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the UTF-8 sequence.
	/// @return Codepoint representation of the sequence.
	template <input_iterator Iterator>
	constexpr codepoint to_cp(Iterator it);

	/// Gets an iterator to the next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the next character.
	template <input_iterator Iterator>
	constexpr Iterator next(Iterator it);

	/// Gets an iterator to the nth next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move forward by.
	/// @return Iterator to the beginning of the nth next character.
	template <input_iterator Iterator>
	constexpr Iterator next(Iterator it, usize n);

	/// Gets an iterator to the previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the previous character.
	template <input_iterator Iterator>
	constexpr Iterator prev(Iterator it);

	/// Gets an iterator to the nth previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move backward by.
	/// @return Iterator to the beginning of the nth previous character.
	template <input_iterator Iterator>
	constexpr Iterator prev(Iterator it, usize n);

	/// Inserts a character into a UTF-8-encoded string.
	/// @tparam String Output string type.
	/// @param str String to insert the UTF-8 sequence into.
	/// @param where Location to insert the UTF-8 sequence.
	/// @param cp Codepoint to insert as a UTF-8 sequence.
	/// @return Iterator to the location where the sequence was inserted.
	template <output_string String>
	constexpr String::iterator insert(String& str, typename String::iterator where, codepoint cp);

	/// Erases a character from a UTF-8-encoded string.
	/// @tparam String Output string type.
	/// @param str String to erase a character from.
	/// @param where Location to erase the character.
	/// @return Iterator to the location where the character was erased.
	template <output_string String>
	constexpr String::iterator erase(String& str, typename String::iterator where);

	/// Pops back a character from a UTF-8 encoded string.
	/// @tparam String Output string type.
	/// @param str String to erase a character from.
	template <output_string String>
	constexpr void pop_back(String& str);

	/// @}

	/// Unicode codepoint iterator for UTF-8 string views.
	class iterator : public iterator_interface<iterator>
	{
	  public:
		/// Value type used by the iterator.
		using value_type = codepoint;

		/// Difference type used by the iterator.
		using difference_type = ssize;

		/// @name Constructors
		/// @{

		/// Default-constructs a codepoint iterator.
		constexpr iterator() = default;

		/// Constructs a codepoint iterator.
		/// @param ptr Pointer to the beginning of a UTF-8 character sequence.
		constexpr iterator(const char* ptr);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		constexpr friend auto operator<=>(iterator lhs, iterator rhs) = default;

		/// Compares iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return Whether the iterators are equal.
		constexpr friend bool operator==(iterator lhs, iterator rhs) = default;

		/// @}
		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		constexpr codepoint operator*() const;

		/// Increments the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator++();

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator--();

		/// @}
		/// @name Unwrapping
		/// @{

		/// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		constexpr const char* base() const;

		/// @}

	  private:
		/// Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};
	};

	/// Unicode codepoint const reverse iterator for UTF-8 string views.
	using reverse_iterator = std::reverse_iterator<iterator>;

	//

	/// Indexed Unicode codepoint iterator for UTF-8 string views.
	class indexed_iterator : public iterator_interface<indexed_iterator>
	{
	  public:
		/// Value type used by the iterator.
		using value_type = codepoint;

		/// Difference type used by the iterator.
		using difference_type = ssize;

		/// @name Constructors
		/// @{

		/// Default-constructs an indexed codepoint iterator.
		constexpr indexed_iterator() = default;

		/// Constructs an indexed codepoint iterator.
		/// @param ptr Pointer to the beginning of a UTF-8 character sequence.
		/// @param index Index of the iterator.
		constexpr indexed_iterator(const char* ptr, ssize index);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Converts the operator to an unindexed codepoint iterator.
		/// @return Unindexed equivalent of `*this`.
		constexpr operator iterator() const;

		/// @}

		/// Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		constexpr friend auto operator<=>(indexed_iterator lhs, indexed_iterator rhs);

		/// Compares iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return Whether the iterators are equal.
		constexpr friend bool operator==(indexed_iterator lhs, indexed_iterator rhs);

		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		constexpr codepoint operator*() const;

		/// Increments the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator++();

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator--();

		/// @}
		/// @name Unwrapping
		/// @{

		/// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		constexpr const char* base() const;

		/// The index of the iterator.
		/// @return Index of the iterator.
		constexpr ssize index() const;

		/// @}

	  private:
		/// Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};

		/// The index of the iterator.
		ssize m_index{0};
	};

	/// Unicode codepoint const reverse iterator for UTF-8 string views.
	using reverse_indexed_iterator = std::reverse_iterator<indexed_iterator>;

	/// @name UTF-8 iterators
	/// @{

	/// Creates a beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Beginning codepoint iterator for `str`.
	constexpr iterator begin(std::string_view str);

	/// Creates an indexed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed beginning codepoint iterator for `str`.
	constexpr indexed_iterator ibegin(std::string_view str);

	/// Creates an ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Ending codepoint iterator for `str`.
	constexpr iterator end(std::string_view str);

	/// Creates an indexed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed ending codepoint iterator for `str`.
	constexpr indexed_iterator iend(std::string_view str);

	/// Creates a reversed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Reversed beginning codepoint iterator for `str`.
	constexpr reverse_iterator rbegin(std::string_view str);

	/// Creates an indexed reversed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed reversed beginning codepoint iterator for `str`.
	constexpr reverse_indexed_iterator ribegin(std::string_view str);

	/// Creates a reversed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Reversed ending codepoint iterator for `str`.
	constexpr reverse_iterator rend(std::string_view str);

	/// Creates an indexed reversed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed reversed ending codepoint iterator for `str`.
	constexpr reverse_indexed_iterator riend(std::string_view str);

	/// Creates a codepoint iterator range pair for a UTF-8 string view.
	/// @param str String to get the range for.
	/// @return Codepoint iterator range over `str`.
	constexpr std::ranges::subrange<iterator> range(std::string_view str);

	/// Counts the number of unicode characters in a UTF-8 string view.
	/// @param str String to get the length for.
	/// @return Number of unicode characters in `str`.
	constexpr usize length(std::string_view str);

	/// @}
} // namespace tr::utf8

#include "impl/utf8.hpp" // IWYU pragma: export