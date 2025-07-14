#pragma once
#include "binary_io.hpp"

/** @ingroup utility
 *  @defgroup static_string Static String
 *  Static string template class and related functions.
 *  @{
 */

namespace tr {
	/**************************************************************************************************************************************
	 * Inplace-allocated fixed-capacity string.
	 *
	 * @em static_string is @em not NULL-terminated.
	 *************************************************************************************************************************************/
	template <std::size_t S> class static_string {
	  public:
		using value_type = char;
		using size_type = size_type_t<S>;
		using difference_type = std::make_signed_t<size_type>;
		using reference = char&;
		using const_reference = const char&;
		using pointer = char*;
		using const_pointer = const char*;
		using iterator = pointer;
		using const_iterator = const_pointer;

		//

		/**********************************************************************************************************************************
		 * Creates an empty string.
		 *********************************************************************************************************************************/
		constexpr static_string() noexcept = default;

		/**********************************************************************************************************************************
		 * Creates a string by copying from a string literal.
		 *********************************************************************************************************************************/
		template <std::size_t S1> constexpr static_string(const char (&literal)[S1]) noexcept;

		/**********************************************************************************************************************************
		 * Creates a string by copying from another string.
		 *********************************************************************************************************************************/
		constexpr static_string(std::string_view str) noexcept;

		//

		/**********************************************************************************************************************************
		 * Gets a string view over the string.
		 *********************************************************************************************************************************/
		constexpr operator std::string_view() const noexcept;

		/**********************************************************************************************************************************
		 * Gets a standard string copy of the string.
		 *********************************************************************************************************************************/
		constexpr explicit operator std::string() const noexcept;

		//

		/**********************************************************************************************************************************
		 * Gets a reference to a character in the string.
		 *
		 * @pre @em offset must be less than the size of the string.
		 *
		 * @return A reference to a character in the string at the given offset.
		 *********************************************************************************************************************************/
		constexpr reference operator[](size_type offset) noexcept;

		/**********************************************************************************************************************************
		 * Gets a reference to a character in the string.
		 *
		 * @pre @em offset must be less than the size of the string.
		 *
		 * @return A reference to a character in the string at the given offset.
		 *********************************************************************************************************************************/
		constexpr const_reference operator[](size_type offset) const noexcept;

		/**********************************************************************************************************************************
		 * Gets a reference to the first character in the string.
		 *
		 * @pre The string must not be empty.
		 *
		 * @return A reference to the first character in the string.
		 *********************************************************************************************************************************/
		constexpr reference front() noexcept;

		/**********************************************************************************************************************************
		 * Gets a reference to the first character in the string.
		 *
		 * @pre The string must not be empty.
		 *
		 * @return A reference to the first character in the string.
		 *********************************************************************************************************************************/
		constexpr const_reference front() const noexcept;

		/**********************************************************************************************************************************
		 * Gets a reference to the last character in the string.
		 *
		 * @pre The string must not be empty.
		 *
		 * @return A reference to the last character in the string.
		 *********************************************************************************************************************************/
		constexpr reference back() noexcept;

		/**********************************************************************************************************************************
		 * Gets a reference to the last character in the string.
		 *
		 * @pre The string must not be empty.
		 *
		 * @return A reference to the last character in the string.
		 *********************************************************************************************************************************/
		constexpr const_reference back() const noexcept;

		/**********************************************************************************************************************************
		 * Gets a pointer to the data of the string.
		 *
		 * @return A pointer to the data of the string.
		 *********************************************************************************************************************************/
		constexpr pointer data() noexcept;

		/**********************************************************************************************************************************
		 * Gets a pointer to the data of the string.
		 *
		 * @return A pointer to the data of the string.
		 *********************************************************************************************************************************/
		constexpr const_pointer data() const noexcept;

		//

		/**********************************************************************************************************************************
		 * Gets an iterator to the beginning of the string.
		 *
		 * @return An iterator to the beginning of the string.
		 *********************************************************************************************************************************/
		constexpr iterator begin() noexcept;

		/**********************************************************************************************************************************
		 * Gets an iterator to the beginning of the string.
		 *
		 * @return An iterator to the beginning of the string.
		 *********************************************************************************************************************************/
		constexpr const_iterator begin() const noexcept;

		/**********************************************************************************************************************************
		 * Gets an iterator to the beginning of the string.
		 *
		 * @return An iterator to the beginning of the string.
		 *********************************************************************************************************************************/
		constexpr const_iterator cbegin() const noexcept;

		/**********************************************************************************************************************************
		 * Gets an iterator to the end of the string.
		 *
		 * @return An iterator to the end of the string.
		 *********************************************************************************************************************************/
		constexpr iterator end() noexcept;

		/**********************************************************************************************************************************
		 * Gets an iterator to the end of the string.
		 *
		 * @return An iterator to the end of the string.
		 *********************************************************************************************************************************/
		constexpr const_iterator end() const noexcept;

		/**********************************************************************************************************************************
		 * Gets an iterator to the end of the string.
		 *
		 * @return An iterator to the end of the string.
		 *********************************************************************************************************************************/
		constexpr const_iterator cend() const noexcept;

		//

		/**********************************************************************************************************************************
		 * Gets whether the string is empty.
		 *
		 * @return True if the string is empty, false otherwise.
		 *********************************************************************************************************************************/
		constexpr bool empty() const noexcept;

		/**********************************************************************************************************************************
		 * Gets the current size of the string.
		 *
		 * @return The current size of the string.
		 *********************************************************************************************************************************/
		constexpr size_type size() const noexcept;

		/**********************************************************************************************************************************
		 * Gets the maximum allowed size of the string.
		 *
		 * @return The maximum allowed size of the string.
		 *********************************************************************************************************************************/
		static constexpr size_type max_size() noexcept;

		//

		/**********************************************************************************************************************************
		 * Erases all characters from the string.
		 *********************************************************************************************************************************/
		constexpr void clear() noexcept;

		/**********************************************************************************************************************************
		 * Appends a character to the string.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] chr The character to append.
		 *********************************************************************************************************************************/
		constexpr void push_back(char chr) noexcept;

		/**********************************************************************************************************************************
		 * Appends a character to the string.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] chr The character to append.
		 *********************************************************************************************************************************/
		constexpr void append(char chr) noexcept;

		/**********************************************************************************************************************************
		 * Appends a character to the string.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] chr The character to append.
		 *********************************************************************************************************************************/
		constexpr static_string& operator+=(char chr) noexcept;

		/**********************************************************************************************************************************
		 * Appends a range to the string.
		 *
		 * @pre size() + str.size() <= max_size().
		 *
		 * @param[in] begin The beginning of the range to append.
		 * @param[in] end The end of the range to append.
		 *********************************************************************************************************************************/
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, char>)
		constexpr void append(It begin, It end) noexcept;

		/**********************************************************************************************************************************
		 * Appends a substring to the string.
		 *
		 * @pre size() + str.size() <= max_size().
		 *
		 * @param[in] str The substring to append.
		 *********************************************************************************************************************************/
		constexpr void append(std::string_view str) noexcept;

		/**********************************************************************************************************************************
		 * Appends a substring to the string.
		 *
		 * @pre size() + str.size() <= max_size().
		 *
		 * @param[in] str The substring to append.
		 *********************************************************************************************************************************/
		constexpr static_string& operator+=(std::string_view str) noexcept;

		/**********************************************************************************************************************************
		 * Inserts a character into the string.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] where Iterator to where the character should be inserted.
		 * @param[in] chr The character to insert.
		 *********************************************************************************************************************************/
		constexpr void insert(const_iterator where, char chr) noexcept;

		/**********************************************************************************************************************************
		 * Inserts a range into the string.
		 *
		 * @pre size() + str.size() <= max_size().
		 *
		 * @param[in] where Iterator to where the substring should be inserted.
		 * @param[in] first The beginning of the range to insert.
		 * @param[in] last The end of the range to insert.
		 *********************************************************************************************************************************/
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, char>)
		constexpr void insert(const_iterator where, It first, It last) noexcept;

		/**********************************************************************************************************************************
		 * Inserts a substring into the string.
		 *
		 * @pre size() + str.size() <= max_size().
		 *
		 * @param[in] where Iterator to where the substring should be inserted.
		 * @param[in] str The substring to insert.
		 *********************************************************************************************************************************/
		constexpr void insert(const_iterator where, std::string_view str) noexcept;

		/**********************************************************************************************************************************
		 * Erases a character from the end of the string.
		 *
		 * Calling this function is safe even if the string is empty.
		 *********************************************************************************************************************************/
		constexpr void pop_back() noexcept;

		/**********************************************************************************************************************************
		 * Erases a character from the string.
		 *
		 * @param[in] where Iterator to the deleted character.
		 *********************************************************************************************************************************/
		constexpr void erase(const_iterator where) noexcept;

		/**********************************************************************************************************************************
		 * Erases a portion of the string.
		 *
		 * @param[in] start Iterator to the first deleted character.
		 * @param[in] end Iterator to one past the last deleted character.
		 *********************************************************************************************************************************/
		constexpr void erase(const_iterator start, const_iterator end) noexcept;

		/**********************************************************************************************************************************
		 * Resizes the string, filling it with a character if necessary.
		 *
		 * @param[in] size
		 * @parblock
		 * The new size of the string.
		 *
		 * @pre @em size must be smaller than or equal to max_size().
		 * @endparblock
		 * @param[in] chr The character to fill with.
		 *********************************************************************************************************************************/
		constexpr void resize(size_type size, char chr = '\0') noexcept;

	  private:
		std::array<char, S> _buffer{};
		size_type _size{0};
	};

	template <std::size_t S> constexpr std::strong_ordering operator<=>(const static_string<S>& l, const static_string<S>& r) noexcept;

	template <std::size_t S> constexpr bool operator==(const static_string<S>& l, const static_string<S>& r) noexcept;

	template <std::size_t S> constexpr std::strong_ordering operator<=>(const static_string<S>& l, const std::string_view& r) noexcept;

	template <std::size_t S> constexpr bool operator==(const static_string<S>& l, const std::string_view& r) noexcept;

	template <std::size_t S> constexpr std::strong_ordering operator<=>(const std::string_view& l, const static_string<S>& r) noexcept;

	template <std::size_t S> constexpr bool operator==(const std::string_view& l, const static_string<S>& r) noexcept;

	/**************************************************************************************************************************************
	 * Creates a static string from a string literal.
	 *
	 * @param[in] literal The literal to create the static string with.
	 *
	 * @return A static string with exactly the capacity to fit the literal.
	 *************************************************************************************************************************************/
	template <std::size_t S> constexpr static_string<S - 1> make_static_string(const char (&literal)[S]) noexcept;

	/**************************************************************************************************************************************
	 * Output stream formatter for static strings.
	 *
	 * @param[out] os The stream to output to.
	 * @param[in] str The string to output.
	 *
	 * @return The output stream reference.
	 *************************************************************************************************************************************/
	template <std::size_t S> inline std::ostream& operator<<(std::ostream& os, const static_string<S>& str);

	/**************************************************************************************************************************************
	 * Static string binary reader.
	 *************************************************************************************************************************************/
	template <std::size_t S> struct binary_reader<static_string<S>> {
		static void read_from_stream(std::istream& is, static_string<S>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, static_string<S>& out);
	};

	/**************************************************************************************************************************************
	 * Static string binary writer.
	 *************************************************************************************************************************************/
	template <std::size_t S> struct binary_writer<static_string<S>> {
		static void write_to_stream(std::ostream& os, const static_string<S>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const static_string<S>& in);
	};

} // namespace tr

/******************************************************************************************************************************************
 * Formatter for static strings.
 *****************************************************************************************************************************************/
template <std::size_t S> struct std::formatter<tr::static_string<S>> : std::formatter<std::string_view> {
	/// @cond IMPLEMENTATION
	template <typename FormatContext> constexpr auto format(const tr::static_string<S>& str, FormatContext& ctx) const
	{
		return std::formatter<std::string_view>::format(std::string_view{str}, ctx);
	}
	/// @endcond
};

/// @}

#include "static_string_impl.hpp"
