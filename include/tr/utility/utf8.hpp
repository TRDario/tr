/// @file
/// @brief Provides UTF-8-handling utilities

#pragma once
#include "iterator_interface.hpp"

//

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

	/// Supported bidirectional input iterator type.
	template <typename T>
	concept bidirectional_iterator = std::bidirectional_iterator<T> && std::same_as<std::iter_value_t<T>, char>;

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
	[[nodiscard]] constexpr codepoint to_cp(Iterator it) noexcept(noexcept(*++it));

	/// Gets an iterator to the next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the next character.
	template <input_iterator Iterator>
	[[nodiscard]] constexpr Iterator next(Iterator it) noexcept(noexcept(*++it));

	/// Gets an iterator to the nth next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move forward by.
	/// @return Iterator to the beginning of the nth next character.
	template <input_iterator Iterator>
	[[nodiscard]] constexpr Iterator next(Iterator it, usize n) noexcept(noexcept(*++it));

	/// Gets an iterator to the previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the previous character.
	template <bidirectional_iterator Iterator>
	[[nodiscard]] constexpr Iterator prev(Iterator it) noexcept(noexcept(*--it));

	/// Gets an iterator to the nth previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move backward by.
	/// @return Iterator to the beginning of the nth previous character.
	template <bidirectional_iterator Iterator>
	[[nodiscard]] constexpr Iterator prev(Iterator it, usize n) noexcept(noexcept(*--it));

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
		[[nodiscard]] constexpr iterator() noexcept = default;

		/// Constructs a codepoint iterator.
		/// @param ptr Pointer to the beginning of a UTF-8 character sequence.
		[[nodiscard]] constexpr iterator(const char* ptr) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		[[nodiscard]] constexpr friend auto operator<=>(iterator lhs, iterator rhs) noexcept = default;

		/// Compares iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return Whether the iterators are equal.
		[[nodiscard]] constexpr friend bool operator==(iterator lhs, iterator rhs) noexcept = default;

		/// @}
		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		[[nodiscard]] constexpr codepoint operator*() const noexcept;

		/// Increments the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator++() noexcept;

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator--() noexcept;

		/// @}
		/// @name Unwrapping
		/// @{

		/// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		[[nodiscard]] constexpr const char* base() const noexcept;

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
		[[nodiscard]] constexpr indexed_iterator() noexcept = default;

		/// Constructs an indexed codepoint iterator.
		/// @param ptr Pointer to the beginning of a UTF-8 character sequence.
		/// @param index Index of the iterator.
		[[nodiscard]] constexpr indexed_iterator(const char* ptr, ssize index) noexcept;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Converts the operator to an unindexed codepoint iterator.
		/// @return Unindexed equivalent of `*this`.
		[[nodiscard]] constexpr operator iterator() const noexcept;

		/// @}

		/// Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		constexpr friend auto operator<=>(indexed_iterator lhs, indexed_iterator rhs) noexcept;

		/// Compares iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return Whether the iterators are equal.
		constexpr friend bool operator==(indexed_iterator lhs, indexed_iterator rhs) noexcept;

		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		[[nodiscard]] constexpr codepoint operator*() const noexcept;

		/// Increments the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator++() noexcept;

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator--() noexcept;

		/// @}
		/// @name Unwrapping
		/// @{

		/// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		[[nodiscard]] constexpr const char* base() const noexcept;

		/// Gets the index of the iterator.
		/// @return Index of the iterator.
		[[nodiscard]] constexpr ssize index() const noexcept;

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
	[[nodiscard]] constexpr iterator begin(std::string_view str) noexcept;

	/// Creates an indexed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr indexed_iterator ibegin(std::string_view str) noexcept;

	/// Creates an ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Ending codepoint iterator for `str`.
	[[nodiscard]] constexpr iterator end(std::string_view str) noexcept;

	/// Creates an indexed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr indexed_iterator iend(std::string_view str) noexcept;

	/// Creates a reversed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Reversed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_iterator rbegin(std::string_view str) noexcept;

	/// Creates an indexed reversed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed reversed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_indexed_iterator ribegin(std::string_view str) noexcept;

	/// Creates a reversed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Reversed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_iterator rend(std::string_view str) noexcept;

	/// Creates an indexed reversed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed reversed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_indexed_iterator riend(std::string_view str) noexcept;

	/// Creates a codepoint iterator range pair for a UTF-8 string view.
	/// @param str String to get the range for.
	/// @return Codepoint iterator range over `str`.
	[[nodiscard]] constexpr std::ranges::subrange<iterator> range(std::string_view str) noexcept;

	/// Counts the number of unicode characters in a UTF-8 string view.
	/// @param str String to get the length for.
	/// @return Number of unicode characters in `str`.
	[[nodiscard]] constexpr usize length(std::string_view str) noexcept;

	/// @}
} // namespace tr::utf8

#include "impl/utf8.hpp" // IWYU pragma: export