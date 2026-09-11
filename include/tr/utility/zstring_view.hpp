/// @file
/// @brief Provides `tr::basic_zstring_view` and related functions.

#pragma once
#include "pointer_iterator.hpp"

//

namespace tr
{
	/// View to a NUL-terminated string.
	/// @tparam CharT Character type used by the string.
	/// @tparam Traits Character traits used by the string.
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_zstring_view
	{
	  public:
		/// Character traits used by the string.
		using traits_type = Traits;

		/// Size type used by the string.
		using size_type = usize;

		/// Difference type used by the string.
		using difference_type = ssize;

		/// Character type used by the string.
		using value_type = CharT;

		/// Pointer type used by the string.
		using pointer = CharT*;

		/// Constant pointer type used by the string.
		using const_pointer = const CharT*;

		/// Reference type used by the string.
		using reference = CharT&;

		/// Constant reference type used by the string.
		using const_reference = const CharT&;

		/// End sentinel type used by the string.
		struct end_sentinel
		{
		};

		/// Iterator type used by the string.
		struct iterator : pointer_iterator<iterator, const_pointer>
		{
			using pointer_iterator<iterator, const_pointer>::pointer_iterator;

			/// Compares the iterator to an end sentinel.
			/// @param rhs End sentinel.
			/// @return `true` if the iterator is pointing to the end of the string, `false` otherwise.
			[[nodiscard]] constexpr bool operator==(end_sentinel rhs) const noexcept;
		};

		/// Constant iterator type used by the string.
		using const_iterator = iterator;

		/// @name Constructors
		/// @{

		/// Constructs an empty string view.
		[[nodiscard]] constexpr basic_zstring_view() noexcept = default;

		/// Constructs a string view from a NUL-terminated string pointer.
		/// @param ptr Pointer to a NUL-terminated string.
		[[nodiscard]] constexpr basic_zstring_view(const CharT* ptr) noexcept;

		/// Constructs a string view from a standard string.
		/// @tparam Allocator String allocator.
		/// @param str Standard string.
		template <typename Allocator>
		[[nodiscard]] constexpr basic_zstring_view(const std::basic_string<CharT, Traits, Allocator>& str) noexcept;

		/// Trivially copies a string view.
		/// @param rhs String view to copy.
		[[nodiscard]] constexpr basic_zstring_view(const basic_zstring_view& rhs) noexcept = default;

		/// Trivially moves a string view.
		/// @param rhs String view to move.
		[[nodiscard]] constexpr basic_zstring_view(basic_zstring_view&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Trivially copies a string view.
		/// @param rhs String view to copy.
		/// @return Reference to `*this`.
		constexpr basic_zstring_view& operator=(const basic_zstring_view& rhs) noexcept = default;

		/// Trivially moves a string view.
		/// @param rhs String view to move.
		/// @return Reference to `*this`.
		constexpr basic_zstring_view& operator=(basic_zstring_view&& rhs) noexcept = default;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Converts the view into a standard string view.
		/// @return Standard string view equivalent to `*this`.
		[[nodiscard]] constexpr operator std::basic_string_view<CharT, Traits>() const noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares NUL-terminated string views.
		/// @param rhs Second string view.
		/// @return Ordering of the string views.
		[[nodiscard]] constexpr std::strong_ordering operator<=>(this basic_zstring_view lhs,
																 std::type_identity_t<basic_zstring_view> rhs) noexcept;

		/// Compares NUL-terminated string views for equality.
		/// @param rhs Second string view.
		/// @return Whether the string views are equal.
		[[nodiscard]] constexpr bool operator==(this basic_zstring_view lhs, std::type_identity_t<basic_zstring_view> rhs) noexcept;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr iterator begin() const noexcept;

		/// Gets an iterator to the beginning of the string.
		/// @return Iterator to the beginning of the string.
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept;

		/// Gets an iterator sentinel to the end of the string.
		/// @return Iterator sentinel to the end of the string.
		[[nodiscard]] constexpr end_sentinel end() const noexcept;

		/// Gets an iterator sentinel to the end of the string.
		/// @return Iterator sentinel to the end of the string.
		[[nodiscard]] constexpr end_sentinel cend() const noexcept;

		/// @}
		/// @name Access
		/// @{

		/// Accesses a specific character.
		/// @param index Index of the character within the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr const_reference operator[](size_type index) const noexcept;

		/// Accesses a specific character.
		/// @param index Index of the character within the string.
		/// @return Reference to a character within the string.
		[[nodiscard]] constexpr const_reference at(size_type index) const noexcept;

		/// Accesses the first character in the string.
		/// @return Reference to the first character in the string.
		[[nodiscard]] constexpr const_reference front() const noexcept;

		/// Gets a pointer to the raw data of the string.
		/// @return Pointer to the first character in the string.
		[[nodiscard]] constexpr const_pointer data() const noexcept;

		/// Gets a pointer to the raw C-string.
		/// @return Pointer to the first character in the string.
		[[nodiscard]] constexpr const_pointer c_str() const noexcept;

		/// @}
		/// @name Length
		/// @{

		/// Gets the length of the string as if by strlen.
		/// @return Length of the string.
		[[nodiscard]] constexpr usize length() const noexcept;

		/// Gets whether the string view is empty.
		/// @return `true` if the view points to `nullptr` or an empty string, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept;

		/// @}
		/// @name Sub-strings
		/// @{

		/// Creates a sub-string view.
		/// @param pos Starting position of the sub-string within the string.
		/// @return Sub-string starting at `pos`.
		[[nodiscard]] constexpr basic_zstring_view substr(size_type pos = 0) const noexcept;

		/// Moves the start of the view forward.
		/// @param n Number of characters to move the start of the view forward by.
		constexpr void remove_prefix(size_type n) noexcept;

		/// @}

	  private:
		/// Pointer to a NUL-terminated string.
		const_pointer m_ptr{nullptr};
	};

	/// NUL-terminated 8-bit character string view.
	using zstring_view = basic_zstring_view<char>;

	inline namespace literals
	{
		/// Namespace containing NUL-terminated string view literals.
		inline namespace zstring_view_literals
		{
			/// @name Literals
			/// @{

			/// Constructs a null-terminated string view literal.
			/// @param str Base C-string literal.
			/// @param len Length of the string literal.
			/// @return String view literal wrapped in `tr::zstring_view`.
			[[nodiscard]] constexpr zstring_view operator""_zsv(const char* str, std::size_t len) noexcept;

			/// @}
		} // namespace zstring_view_literals
	} // namespace literals

	/// @name Overloaded operators
	/// @{

	/// Output stream formatter for NUL-terminated string views.
	/// @tparam CharT Character type used by the string.
	/// @tparam Traits Character traits used by the string.
	/// @param os Output stream.
	/// @param zstr String view to output.
	template <typename CharT, typename Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, basic_zstring_view<CharT, Traits> zstr);

	/// Path construction operator for NUL-terminated string views.
	/// @tparam CharT Character type used by the string.
	/// @tparam Traits Character traits used by the string.
	/// @param lhs Path to concatename.
	/// @param rhs String to concatenate onto the path.
	/// @return Concatenated path.
	template <typename CharT, typename Traits>
	[[nodiscard]] std::filesystem::path operator/(const std::filesystem::path& lhs, basic_zstring_view<CharT, Traits> rhs);

	/// @}
} // namespace tr

#include "impl/zstring_view.hpp" // IWYU pragma: export