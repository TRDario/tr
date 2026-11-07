/// @file
/// @brief Provides UTF-8-handling utilities.

/// @ingroup utility
/// @defgroup utf8 UTF-8
/// @brief Contains UTF-8-handling utilities.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/iterator_interface.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/utf8.hpp>

//

namespace tr
{
	/// @ingroup utf8
	/// @brief Datatype used for Unicode codepoints.
	using codepoint = u32;
} // namespace tr

/// @brief Namespace containing UTF-8-related functionality.
namespace tr::utf8
{
	/// @ingroup utf8
	/// @brief Concept checking for a valid UTF-8 input iterator type.
	/// @tparam Target Type to check for being a valid UTF-8 input iterator type.
	template <typename Target>
	concept input_iterator = std::input_iterator<Target> && std::same_as<std::iter_value_t<Target>, char>;

	/// @ingroup utf8
	/// @brief Concept checking for a valid UTF-8 bidirectional input iterator type.
	/// @tparam Target Type to check for being a valid UTF-8 bidirectional input iterator type.
	template <typename Target>
	concept bidirectional_iterator = std::bidirectional_iterator<Target> && std::same_as<std::iter_value_t<Target>, char>;

	/// @ingroup utf8
	/// @brief Concept checking for a valid UTF-8 output string type.
	/// @tparam Target Type to check for being a valid UTF-8 output string type.
	template <typename Target>
	concept output_string = requires(Target& str, Target::iterator it, const std::array<char, 4>& chars) {
		{ str.empty() } -> std::same_as<bool>;
		str.insert(it, '\0');
		str.insert(it, chars.begin(), chars.end());
		str.erase(it);
		str.erase(it, it + 4);
		str.pop_back();
	};

	//

	/// @ingroup utf8
	/// @brief Converts a UTF-8 sequence to a Unicode codepoint.
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

	/// @ingroup utf8
	/// @brief Returns an iterator to the next character.
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

	/// @ingroup utf8
	/// @brief Returns an iterator to the `n`th next character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move forward by.
	/// @par Complexity
	/// Linear time with respect to `n`.
	/// @return Iterator to the beginning of the `n`th next character.
	template <input_iterator Iterator>
	[[nodiscard]] constexpr Iterator next(Iterator it, usize n) noexcept(noexcept(*++it))
	{
		for (usize i = 0; i < n; ++i) {
			it = tr::utf8::next(it);
		}
		return it;
	}

	/// @ingroup utf8
	/// @brief Returns an iterator to the previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @return Iterator to the beginning of the previous character.
	template <bidirectional_iterator Iterator>
	[[nodiscard]] constexpr Iterator prev(Iterator it) noexcept(noexcept(*--it))
	{
		while ((*--it & 0xC0) == 0x80) {}
		return it;
	}

	/// @ingroup utf8
	/// @brief Returns an iterator to the `n`th previous character.
	/// @tparam Iterator Input iterator type.
	/// @param it Iterator to the current character.
	/// @param n Number of steps to move backward by.
	/// @par Complexity
	/// Linear time with respect to `n`.
	/// @return Iterator to the beginning of the `n`th previous character.
	template <bidirectional_iterator Iterator>
	[[nodiscard]] constexpr Iterator prev(Iterator it, usize n) noexcept(noexcept(*--it))
	{
		for (usize i = 0; i < n; ++i) {
			it = tr::utf8::prev(it);
		}
		return it;
	}

	/// @ingroup utf8
	/// @brief Inserts a character into a UTF-8-encoded string.
	/// @tparam String Output string type.
	/// @param str String to insert the UTF-8 sequence into.
	/// @param where Location to insert the UTF-8 sequence.
	/// @param cp Codepoint to insert as a UTF-8 sequence.
	/// @par Exception safety
	/// Directly inherited from `str.insert()`.
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

	/// @ingroup utf8
	/// @brief Erases a character from a UTF-8-encoded string.
	/// @tparam String Output string type.
	/// @param str String to erase a character from.
	/// @param where Location to erase the character.
	/// @return Iterator to the location where the character was erased.
	template <output_string String>
	constexpr String::iterator erase(String& str, typename String::iterator where) noexcept
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

	/// @ingroup utf8
	/// @brief Pops back a character from a UTF-8 encoded string.
	/// @tparam String Output string type.
	/// @param str String to erase a character from.
	template <output_string String>
	constexpr void pop_back(String& str) noexcept
	{
		if (!str.empty()) {
			while (!str.empty() && (str.back() & 0xC0) == 0x80) {
				str.pop_back();
			}
			str.pop_back();
		}
	}

	//

	/// @ingroup utf8
	/// @brief Unicode codepoint iterator for UTF-8 string views.
	class iterator : public iterator_interface<iterator>
	{
	  public:
		/// @name Iterator traits
		/// @{

		/// @brief Value type used by the iterator.
		using value_type = codepoint;

		/// @brief Difference type used by the iterator.
		using difference_type = ssize;

		/// @brief Iterator category of the iterator.
		using iterator_category = std::bidirectional_iterator_tag;

		/// @brief Iterator concept of the iterator.
		using iterator_concept = std::bidirectional_iterator_tag;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a singular codepoint iterator.
		[[nodiscard]] constexpr iterator() noexcept = default;

		/// @brief Constructs a codepoint iterator.
		/// @param ptr Pointer to the beginning of a UTF-8 character sequence.
		[[nodiscard]] explicit constexpr iterator(const char* ptr) noexcept
			: m_ptr{ptr}
		{
			TR_ASSERT(ptr, "Tried to create UTF-8 iterator to nullptr.");
		}

		/// @}
		/// @name Operators
		/// @{

		/// @brief Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		[[nodiscard]] constexpr friend auto operator<=>(iterator lhs, iterator rhs) noexcept = default;

		/// @brief Checks iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return `true` if the iterators point to the same codepoint, `false` otherwise.
		[[nodiscard]] constexpr friend bool operator==(iterator lhs, iterator rhs) noexcept = default;

		/// @brief Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		[[nodiscard]] constexpr codepoint operator*() const noexcept
		{
			TR_ASSERT(m_ptr, "Tried to dereference default-constructed UTF-8 iterator.");

			return to_cp(m_ptr);
		}

		/// @brief Increments the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator++() noexcept
		{
			TR_ASSERT(m_ptr, "Tried to increment default-constructed UTF-8 iterator.");

			m_ptr = next(m_ptr);
			return *this;
		}

		/// @brief Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr iterator& operator--() noexcept
		{
			TR_ASSERT(m_ptr, "Tried to decrement default-constructed UTF-8 iterator.");

			m_ptr = prev(m_ptr);
			return *this;
		}

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		[[nodiscard]] constexpr const char* base() const noexcept
		{
			return m_ptr;
		}

		/// @}

	  private:
		/// @brief Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};
	};

	//

	/// @ingroup utf8
	/// @brief Indexed Unicode codepoint iterator for UTF-8 string views.
	class indexed_iterator : public iterator_interface<indexed_iterator>
	{
	  public:
		/// @name Iterator traits
		/// @{

		/// @brief Value type used by the iterator.
		using value_type = codepoint;

		/// @brief Difference type used by the iterator.
		using difference_type = ssize;

		/// @brief Iterator category of the iterator.
		using iterator_category = std::bidirectional_iterator_tag;

		/// @brief Iterator concept of the iterator.
		using iterator_concept = std::bidirectional_iterator_tag;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a singular indexed codepoint iterator.
		[[nodiscard]] constexpr indexed_iterator() noexcept = default;

		/// @brief Constructs an indexed codepoint iterator.
		/// @param ptr Pointer to the beginning of a UTF-8 character sequence.
		/// @param index Index of the iterator.
		[[nodiscard]] constexpr indexed_iterator(const char* ptr, ssize index) noexcept
			: m_ptr{ptr}
			, m_index{index}
		{
			TR_ASSERT(ptr, "Tried to create UTF-8 iterator to nullptr.");
		}

		/// @}
		/// @name Operators
		/// @{

		/// @brief Compares iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		[[nodiscard]] constexpr friend auto operator<=>(indexed_iterator lhs, indexed_iterator rhs) noexcept
		{
			return lhs.m_ptr <=> rhs.m_ptr;
		}

		/// @brief Checks iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return `true` if the iterators point to the same codepoint, `false` otherwise.
		[[nodiscard]] constexpr friend bool operator==(indexed_iterator lhs, indexed_iterator rhs) noexcept
		{
			return lhs.m_ptr == rhs.m_ptr;
		}

		/// @brief Dereferences the iterator.
		/// @return Codepoint representation of the sequence the iterator is pointing at.
		[[nodiscard]] constexpr codepoint operator*() const noexcept
		{
			TR_ASSERT(m_ptr, "Tried to dereference default-constructed UTF-8 iterator.");

			return to_cp(m_ptr);
		}

		/// @brief Increments the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator++() noexcept
		{
			TR_ASSERT(m_ptr, "Tried to increment default-constructed UTF-8 iterator.");

			if (++m_index > 0) {
				m_ptr = next(m_ptr);
			}
			return *this;
		}

		/// @brief Decrements the iterator.
		/// @return Reference to `*this`.
		constexpr indexed_iterator& operator--() noexcept
		{
			TR_ASSERT(m_ptr, "Tried to decrement default-constructed UTF-8 iterator.");

			if (--m_index >= 0) {
				m_ptr = prev(m_ptr);
			}
			return *this;
		}

		/// @}
		/// @name Element access
		/// @{

		/// @brief Converts the operator to an unindexed codepoint iterator.
		/// @return Unindexed equivalent of `*this`.
		[[nodiscard]] constexpr operator iterator() const noexcept
		{
			return iterator{m_ptr};
		}

		/// @brief Returns a const char pointer to the beginning of the iterator's UTF-8 character.
		/// @return Const char pointer to the beginning of the iterator's UTF-8 character.
		[[nodiscard]] constexpr const char* base() const noexcept
		{
			return m_ptr;
		}

		/// @brief Returns the index of the iterator.
		/// @return Index of the iterator.
		[[nodiscard]] constexpr ssize index() const noexcept
		{
			return m_index;
		}

		/// @}

	  private:
		/// @brief Pointer to a valid UTF-8 char sequence.
		const char* m_ptr{nullptr};

		/// @brief The index of the iterator.
		ssize m_index{0};
	};

	//

	/// @ingroup utf8
	/// @brief Returns a beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr iterator begin(std::string_view str) noexcept
	{
		if (str.empty()) {
			return iterator{};
		}
		return iterator{str.data()};
	}

	/// @ingroup utf8
	/// @brief Returns an indexed beginning codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Indexed beginning codepoint iterator for `str`.
	[[nodiscard]] constexpr indexed_iterator ibegin(std::string_view str) noexcept
	{
		if (str.empty()) {
			return indexed_iterator{};
		}
		return indexed_iterator{str.data(), 0};
	}

	/// @ingroup utf8
	/// @brief Returns an ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @return Ending codepoint iterator for `str`.
	[[nodiscard]] constexpr iterator end(std::string_view str) noexcept
	{
		if (str.empty()) {
			return indexed_iterator{};
		}
		return iterator{str.data() + str.size()};
	}

	/// @ingroup utf8
	/// @brief Returns an indexed ending codepoint iterator for a UTF-8 string view.
	/// @param str String to get the iterator for.
	/// @par Complexity
	/// Linear time with respect to `tr::utf8::length(str)`.
	/// @return Indexed ending codepoint iterator for `str`.
	[[nodiscard]] constexpr indexed_iterator iend(std::string_view str) noexcept
	{
		if (str.empty()) {
			return indexed_iterator{};
		}
		return indexed_iterator{str.data() + str.size(), std::ranges::distance(utf8::begin(str), utf8::end(str))};
	}

	/// @ingroup utf8
	/// @brief Returns a codepoint iterator range pair for a UTF-8 string view.
	/// @param str String to get the range for.
	/// @return Codepoint iterator range over `str`.
	[[nodiscard]] constexpr std::ranges::subrange<iterator> range(std::string_view str) noexcept
	{
		return std::ranges::subrange<iterator>{utf8::begin(str), utf8::end(str)};
	}

	/// @ingroup utf8
	/// @brief Returns the number of unicode characters in a UTF-8 string view.
	/// @param str String to get the length for.
	/// @par Complexity
	/// Linear time with respect to `str.size()`.
	/// @return Number of unicode characters in `str`.
	[[nodiscard]] constexpr usize length(std::string_view str) noexcept
	{
		return std::ranges::distance(utf8::begin(str), utf8::end(str));
	}
} // namespace tr::utf8