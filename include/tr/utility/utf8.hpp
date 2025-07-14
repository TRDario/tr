#pragma once
#include "static_string.hpp"

/** @ingroup utility
 *  @defgroup utf8 UTF-8
 *  UTF-8 iterators and operations.
 *  @{
 */

namespace tr {
	/******************************************************************************************************************
	 * Datatype used for Unicode codepoints.
	 *****************************************************************************************************************/
	using codepoint = std::uint32_t;
} // namespace tr

/**********************************************************************************************************************
 * Namespace containing UTF-8-related functionality.
 *********************************************************************************************************************/
namespace tr::utf8 {
	/******************************************************************************************************************
	 * Iterator types supported by the UTF-8 functions.
	 *****************************************************************************************************************/
	template <class T>
	concept base_iterator = std::same_as<T, char*> || std::same_as<T, const char*> || std::same_as<T, std::string_view::iterator> ||
							std::same_as<T, std::string::iterator> || std::same_as<T, std::string::const_iterator>;

	/******************************************************************************************************************
	 * String types supported by the UTF-8 functions.
	 *****************************************************************************************************************/
	template <class T>
	concept string = std::same_as<T, std::string> || std::same_as<T, static_string<T::max_size()>>;

	//

	/******************************************************************************************************************
	 * Converts a UTF-8 sequence to a Unicode codepoint.
	 *
	 * @note The function assumes the text is validly encoded.
	 *
	 * @param[in] it Iterator to UTF-8-encoded text.
	 *
	 * @return A Unicode codepoint.
	 ******************************************************************************************************************/
	template <base_iterator T> constexpr codepoint to_cp(T it) noexcept;

	/******************************************************************************************************************
	 * Gets an iterator to the next character.
	 *
	 * @note The function assumes the text is validly encoded.
	 *
	 * @param[in] it Iterator to UTF-8-encoded text.
	 *
	 * @return Iterator to the beginning of the next character.
	 ******************************************************************************************************************/
	template <base_iterator T> constexpr T next(T it) noexcept;

	/******************************************************************************************************************
	 * Gets an iterator to the previous character.
	 *
	 * @note The function assumes the text is validly encoded.
	 *
	 * @param[in] it Iterator to UTF-8-encoded text.
	 *
	 * @return Iterator to the beginning of the previous character.
	 ******************************************************************************************************************/
	template <base_iterator T> constexpr T prev(T it) noexcept;

	/******************************************************************************************************************
	 * Inserts a character into a UTF-8-encoded string.
	 *
	 * @param[out] str The string to edit.
	 * @param[out] where Where to insert the character.
	 * @param[in] cp The character to insert.
	 ******************************************************************************************************************/
	template <string String> constexpr String::iterator insert(String& str, typename String::iterator where, codepoint cp) noexcept;

	/******************************************************************************************************************
	 * Erases a character from a UTF-8-encoded string.
	 *
	 * @param[out] str The string to edit.
	 * @param[out] where Where to character to delete is located.
	 ******************************************************************************************************************/
	template <string String> constexpr String::iterator erase(String& str, typename String::iterator where) noexcept;

	/******************************************************************************************************************
	 * Pops back a character from a UTF-8 encoded string.
	 *
	 * @param[out] str The string to edit.
	 ******************************************************************************************************************/
	template <string String> constexpr void pop_back(String& str) noexcept;

	//

	/******************************************************************************************************************
	 * Unicode codepoint const iterator for UTF-8 string views.
	 *
	 * The iterator fulfills the @em BidirectionalIterator requirements.
	 ******************************************************************************************************************/
	class const_it {
	  public:
		/**************************************************************************************************************
		 * @em BidirectionalIterator typedef requirement.
		 **************************************************************************************************************/
		using value_type = std::uint32_t;

		/**************************************************************************************************************
		 * @em BidirectionalIterator typedef requirement.
		 **************************************************************************************************************/
		using difference_type = std::ptrdiff_t;

		/**************************************************************************************************************
		 * Default-constructs an iterator.
		 *
		 * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to
		 * it.
		 **************************************************************************************************************/
		constexpr const_it() noexcept = default;

		/**************************************************************************************************************
		 * Constructs an iterator pointing to a UTF-8 sequence in memory.
		 *
		 * @param[in] ptr A pointer to a valid UTF-8 char sequence.
		 **************************************************************************************************************/
		constexpr const_it(const char* ptr) noexcept;

		/**************************************************************************************************************
		 * Compares two iterators.
		 **************************************************************************************************************/
		constexpr friend auto operator<=>(const const_it&, const const_it&) = default;

		/**************************************************************************************************************
		 * Compares two iterators for equality.
		 **************************************************************************************************************/
		constexpr friend bool operator==(const const_it&, const const_it&) = default;

		/**************************************************************************************************************
		 * Dereferences the iterator.
		 *
		 * @pre The iterator must be in a dereferencable state (i.e., not in a default-constructed state or an iterator
		 * created by utf8_end()).
		 *
		 * @return A Unicode codepoint.
		 **************************************************************************************************************/
		constexpr codepoint operator*() const noexcept;

		/**************************************************************************************************************
		 * Pre-increments the iterator.
		 *
		 * @pre The iterator must be in a dereferencable state (i.e., not in a default-constructed state or an iterator
		 * created by utf8_end()).
		 *
		 * @return A reference to the incremented iterator.
		 **************************************************************************************************************/
		constexpr const_it& operator++() noexcept;

		/**************************************************************************************************************
		 * Post-increments the iterator.
		 *
		 * @pre The iterator must be in a dereferencable state (i.e., not in a default-constructed state or an iterator
		 * created by utf8_end()).
		 *
		 * @return An iterator with the prior state of the incremented iterator.
		 **************************************************************************************************************/
		constexpr const_it operator++(int) noexcept;

		/**************************************************************************************************************
		 * Pre-decrements the iterator.
		 *
		 * @pre The iterator must not be default-constructed or pointing to the beginning of a range.
		 *
		 * @return A reference to the decremented iterator.
		 **************************************************************************************************************/
		constexpr const_it& operator--() noexcept;

		/**************************************************************************************************************
		 * Post-decrements the iterator.
		 *
		 * @pre The iterator must not be default-constructed or pointing to the beginning of a range.
		 *
		 * @return An iterator with the prior state of the incremented iterator.
		 **************************************************************************************************************/
		constexpr const_it operator--(int) noexcept;

		/**************************************************************************************************************
		 * Gets a const char pointer to the beginning of the iterator's UTF-8 character.
		 *
		 * @return The const char pointer to the beginning of the iterator's UTF-8 character.
		 **************************************************************************************************************/
		constexpr const char* base() const noexcept;

	  private:
		const char* _impl{nullptr}; // Pointer to a valid UTF-8 char sequence.
	};

	/******************************************************************************************************************
	 * Typedef for a reverse UTF-8 const iterator.
	 ******************************************************************************************************************/
	using const_reverse_it = std::reverse_iterator<const_it>;

	//

	/******************************************************************************************************************
	 * Creates a beginning codepoint iterator for a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return An iterator to the first Unicode character of the string if @em str is non-empty,
	 *         or end() if @em str is empty.
	 ******************************************************************************************************************/
	constexpr const_it begin(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Creates an ending codepoint iterator for a UTF-8 UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return A sentinel iterator marking the end of the string,
	 ******************************************************************************************************************/
	constexpr const_it end(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Creates a reversed beginning codepoint iterator for a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return An iterator to the last Unicode character of the string if @em str is non-empty,
	 *         or rend() if @em str is empty.
	 ******************************************************************************************************************/
	constexpr const_reverse_it rbegin(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Creates an reserved ending codepoint iterator for a UTF-8 UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return A sentinel iterator marking the reverse end of the string,
	 ******************************************************************************************************************/
	constexpr const_reverse_it rend(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Creates a codepoint iterator range pair for a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return A range as if produced by `{ begin(str), end(str) }`.
	 ******************************************************************************************************************/
	constexpr std::ranges::subrange<const_it> range(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Counts the number of unicode characters in a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return The length as if produced by `std::distance(utf8_begin(str), utf8_end(str))`.
	 ******************************************************************************************************************/
	constexpr std::size_t length(std::string_view str) noexcept;
} // namespace tr::utf8

/// @}

#include "utf8_impl.hpp"