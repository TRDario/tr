/// @file
/// @brief Provides UTF-8-handling utilities

#pragma once
#include <tr/utility/iterator_interface.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/utf8.hpp>

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
	[[nodiscard]] constexpr codepoint to_cp(Iterator it) noexcept(noexcept(*++it))
	{
		const u8 leading_byte{static_cast<u8>(*it++)};
		if (leading_byte < 0x80) {
			return leading_byte;
		}
		else if (leading_byte < 0xE0) {
			const u8 second_byte{static_cast<u8>(static_cast<u8>(*it) & 0x3F)};
			return ((leading_byte & 0x1F) << 6) | (second_byte & 0x3F);
		}
		else if (leading_byte < 0xF0) {
			const u8 second_byte{static_cast<u8>(static_cast<u8>(*it++) & 0x3F)};
			const u8 third_byte{static_cast<u8>(static_cast<u8>(*it) & 0x3F)};
			return ((leading_byte & 0xF) << 12) | ((second_byte & 0x3F) << 6) | (third_byte & 0x3F);
		}
		else {
			const u8 second_byte{static_cast<u8>(static_cast<u8>(*it++) & 0x3F)};
			const u8 third_byte{static_cast<u8>(static_cast<u8>(*it++) & 0x3F)};
			const u8 fourth_byte{static_cast<u8>(static_cast<u8>(*it) & 0x3F)};
			return ((leading_byte & 0x7) << 18) | ((second_byte & 0x3F) << 12) | ((third_byte & 0x3F) << 6) | (fourth_byte & 0x3F);
		}
	}

	/// Gets an iterator to the next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the next character.
	template <input_iterator Iterator>
	[[nodiscard]] constexpr Iterator next(Iterator it) noexcept(noexcept(*++it))
	{
		if (static_cast<u8>(*it) < 0x80) {
			return ++it;
		}
		else if (static_cast<u8>(*it) < 0xE0) {
			return std::ranges::next(it, 2);
		}
		else if (static_cast<u8>(*it) < 0xF0) {
			return std::ranges::next(it, 3);
		}
		else {
			return std::ranges::next(it, 4);
		}
	}

	/// Gets an iterator to the nth next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move forward by.
	/// @return Iterator to the beginning of the nth next character.
	template <input_iterator Iterator>
	[[nodiscard]] constexpr Iterator next(Iterator it, usize n) noexcept(noexcept(*++it))
	{
		for (usize i = 0; i < n; ++i) {
			it = tr::utf8::next(it);
		}
		return it;
	}

	/// Gets an iterator to the previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the previous character.
	template <bidirectional_iterator Iterator>
	[[nodiscard]] constexpr Iterator prev(Iterator it) noexcept(noexcept(*--it))
	{
		while ((*--it & 0xC0) == 0x80) {}
		return it;
	}

	/// Gets an iterator to the nth previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move backward by.
	/// @return Iterator to the beginning of the nth previous character.
	template <bidirectional_iterator Iterator>
	[[nodiscard]] constexpr Iterator prev(Iterator it, usize n) noexcept(noexcept(*--it))
	{
		for (usize i = 0; i < n; ++i) {
			it = tr::utf8::prev(it);
		}
		return it;
	}

	/// Inserts a character into a UTF-8-encoded string.
	/// @tparam String Output string type.
	/// @param str String to insert the UTF-8 sequence into.
	/// @param where Location to insert the UTF-8 sequence.
	/// @param cp Codepoint to insert as a UTF-8 sequence.
	/// @return Iterator to the location where the sequence was inserted.
	template <output_string String>
	constexpr String::iterator insert(String& str, typename String::iterator where, codepoint cp)
	{
		if (cp < 0x80) {
			return str.insert(where, cp);
		}
		else if (cp < 0x800) {
			const std::array<char, 2> chars{
				static_cast<char>(cp >> 6 | 0xC0),
				static_cast<char>((cp & 0x3F) | 0x80),
			};
			return str.insert(where, chars.begin(), chars.end());
		}
		else if (cp < 0x10000) {
			const std::array<char, 3> chars{
				static_cast<char>(cp >> 12 | 0xE0),
				static_cast<char>(((cp >> 6) & 0x3F) | 0x80),
				static_cast<char>((cp & 0x3F) | 0x80),
			};
			return str.insert(where, chars.begin(), chars.end());
		}
		else {
			const std::array<char, 4> chars{
				static_cast<char>(cp >> 18 | 0xF0),
				static_cast<char>(((cp >> 12) & 0x3F) | 0x80),
				static_cast<char>(((cp >> 6) & 0x3F) | 0x80),
				static_cast<char>((cp & 0x3F) | 0x80),
			};
			return str.insert(where, chars.begin(), chars.end());
		}
	}

	/// Erases a character from a UTF-8-encoded string.
	/// @tparam String Output string type.
	/// @param str String to erase a character from.
	/// @param where Location to erase the character.
	/// @return Iterator to the location where the character was erased.
	template <output_string String>
	constexpr String::iterator erase(String& str, typename String::iterator where)
	{
		if (static_cast<u8>(*where) < 0x80) {
			return str.erase(where);
		}
		else if (static_cast<u8>(*where) < 0xE0) {
			return str.erase(where, where + 2);
		}
		else if (static_cast<u8>(*where) < 0xF0) {
			return str.erase(where, where + 3);
		}
		else {
			return str.erase(where, where + 4);
		}
	}

	/// Pops back a character from a UTF-8 encoded string.
	/// @tparam String Output string type.
	/// @param str String to erase a character from.
	template <output_string String>
	constexpr void pop_back(String& str)
	{
		if (!str.empty()) {
			while (!str.empty() && (str.back() & 0xC0) == 0x80) {
				str.pop_back();
			}
			str.pop_back();
		}
	}

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
		[[nodiscard]] constexpr iterator(const char* ptr) noexcept
			: m_ptr{ptr}
		{
			TR_ASSERT(ptr != nullptr, "Tried to create UTF-8 iterator to nullptr.");
		}

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
		[[nodiscard]] constexpr codepoint operator*() const noexcept
		{
			TR_ASSERT(m_ptr != nullptr, "Tried to dereference default-constructed UTF-8 iterator.");

			return to_cp(m_ptr);
		}

		/// Increments the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator++() noexcept
		{
			TR_ASSERT(m_ptr != nullptr, "Tried to increment default-constructed UTF-8 iterator.");

			m_ptr = next(m_ptr);
			return *this;
		}

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator--() noexcept
		{
			TR_ASSERT(m_ptr != nullptr, "Tried to decrement default-constructed UTF-8 iterator.");

			m_ptr = prev(m_ptr);
			return *this;
		}

		/// @}
		/// @name Unwrapping
		/// @{

		/// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		[[nodiscard]] constexpr const char* base() const noexcept
		{
			return m_ptr;
		}

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
		[[nodiscard]] constexpr indexed_iterator(const char* ptr, ssize index) noexcept
			: m_ptr{ptr}
			, m_index{index}
		{
			TR_ASSERT(ptr != nullptr, "Tried to create UTF-8 iterator to nullptr.");
		}

		/// @}
		/// @name Conversion operators
		/// @{

		/// Converts the operator to an unindexed codepoint iterator.
		/// @return Unindexed equivalent of `*this`.
		[[nodiscard]] constexpr operator iterator() const noexcept
		{
			return m_ptr;
		}

		/// @}

		/// Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		[[nodiscard]] constexpr friend auto operator<=>(indexed_iterator lhs, indexed_iterator rhs) noexcept
		{
			return lhs.m_ptr <=> rhs.m_ptr;
		}

		/// Compares iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return Whether the iterators are equal.
		[[nodiscard]] constexpr friend bool operator==(indexed_iterator lhs, indexed_iterator rhs) noexcept
		{
			return lhs.m_ptr == rhs.m_ptr;
		}

		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		[[nodiscard]] constexpr codepoint operator*() const noexcept
		{
			TR_ASSERT(m_ptr != nullptr, "Tried to dereference default-constructed UTF-8 iterator.");

			return to_cp(m_ptr);
		}

		/// Increments the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator++() noexcept
		{
			TR_ASSERT(m_ptr != nullptr, "Tried to increment default-constructed UTF-8 iterator.");

			if (++m_index > 0) {
				m_ptr = next(m_ptr);
			}
			return *this;
		}

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator--() noexcept
		{
			TR_ASSERT(m_ptr != nullptr, "Tried to decrement default-constructed UTF-8 iterator.");

			if (--m_index >= 0) {
				m_ptr = prev(m_ptr);
			}
			return *this;
		}

		/// @}
		/// @name Unwrapping
		/// @{

		/// Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		[[nodiscard]] constexpr const char* base() const noexcept
		{
			return m_ptr;
		}

		/// Gets the index of the iterator.
		/// @return Index of the iterator.
		[[nodiscard]] constexpr ssize index() const noexcept
		{
			return m_index;
		}

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
	[[nodiscard]] constexpr iterator begin(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return str.data();
	}

	/// Creates an indexed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr indexed_iterator ibegin(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return {str.data(), 0};
	}

	/// Creates an ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Ending codepoint iterator for `str`.
	[[nodiscard]] constexpr iterator end(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return str.data() + str.size();
	}

	/// Creates an indexed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr indexed_iterator iend(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return {str.data() + str.size(), ssize(str.size())};
	}

	/// Creates a reversed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Reversed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_iterator rbegin(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return reverse_iterator{tr::utf8::end(str)};
	}

	/// Creates an indexed reversed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed reversed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_indexed_iterator ribegin(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return reverse_indexed_iterator{tr::utf8::iend(str)};
	}

	/// Creates a reversed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Reversed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_iterator rend(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return reverse_iterator{tr::utf8::begin(str)};
	}

	/// Creates an indexed reversed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed reversed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr reverse_indexed_iterator riend(std::string_view str) noexcept
	{
		if (str.empty()) {
			return {};
		}
		return reverse_indexed_iterator{tr::utf8::ibegin(str)};
	}

	/// Creates a codepoint iterator range pair for a UTF-8 string view.
	/// @param str String to get the range for.
	/// @return Codepoint iterator range over `str`.
	[[nodiscard]] constexpr std::ranges::subrange<iterator> range(std::string_view str) noexcept
	{
		return {utf8::begin(str), utf8::end(str)};
	}

	/// Counts the number of unicode characters in a UTF-8 string view.
	/// @param str String to get the length for.
	/// @return Number of unicode characters in `str`.
	[[nodiscard]] constexpr usize length(std::string_view str) noexcept
	{
		return std::distance(utf8::begin(str), utf8::end(str));
	}

	/// @}
} // namespace tr::utf8