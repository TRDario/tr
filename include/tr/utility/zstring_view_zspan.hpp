/// @file
/// @brief Provides `tr::basic_zstring_view_zspan`.

/// @ingroup containers
/// @defgroup zstring_view_zspan NUL-terminated spans of NUL-terminated string views
/// @brief Contains `tr::basic_zstring_view_zspan` and its specializations.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// @ingroup zstring_view_zspan
	/// @brief NUL-terminated span of NUL-terminated string views.
	/// @tparam CharT Character type used by the strings of the span.
	/// @tparam Traits Character traits used by the strings of the span.
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_zstring_view_zspan
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Size type used by the span.
		using size_type = usize;

		/// @brief Difference type used by the span.
		using difference_type = ssize;

		/// @brief Value type used by the span.
		using value_type = basic_zstring_view<CharT, Traits>;

		/// @brief End sentinel type used by the span.
		struct end_sentinel
		{
		};

		/// @brief Iterator type used by the span.
		class iterator : public pointer_iterator<iterator, const CharT* const*>
		{
		  public:
			/// @name Constructors, destructors, and assignment operators.
			/// @{

			using pointer_iterator<iterator, const CharT* const*>::pointer_iterator;

			/// @}
			/// @name Operators
			/// @{

			/// @brief Checks whether the iterator is pointing to the end of a NUL-terminated span.
			/// @return `true` if the iterator is pointing to the end of the span, `false` otherwise.
			[[nodiscard]] constexpr bool operator==(end_sentinel) const noexcept
			{
				return !this->base() || !*(this->base());
			}

			/// @brief Dereferences the iterator.
			/// @return Dereferenced string view.
			[[nodiscard]] constexpr basic_zstring_view<CharT, Traits> operator*() const noexcept
			{
				return basic_zstring_view<CharT, Traits>{*(this->base())};
			}

			/// @brief Dereferences the iterator with a subscript.
			/// @param diff Offset to the target value.
			/// @return Dereferenced string view.
			[[nodiscard]] constexpr basic_zstring_view<CharT, Traits> operator[](difference_type diff) const noexcept
			{
				return *(*this + diff);
			}

			/// @}
		};

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty NUL-terminated string view span.
		[[nodiscard]] constexpr basic_zstring_view_zspan() noexcept = default;

		/// @brief Constructs a NUL-terminated string view span.
		/// @param ptr Pointer to the NUL-terminated string view span.
		[[nodiscard]] constexpr basic_zstring_view_zspan(CharT* const* ptr) noexcept
			: m_ptr{const_cast<const CharT* const*>(ptr)}
		{
		}

		/// @brief Constructs a NUL-terminated string view span.
		/// @param ptr Pointer to the NUL-terminated string view span.
		[[nodiscard]] constexpr basic_zstring_view_zspan(const CharT* const* ptr) noexcept
			: m_ptr{ptr}
		{
		}

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a view to a string in the span.
		/// @param index Index of the string within the span.
		/// @pre `index < length()`.
		/// @return View to a string in the span.
		[[nodiscard]] constexpr value_type operator[](size_type index) const noexcept
		{
			return m_ptr[index];
		}

		/// @brief Returns a view to a string in the span.
		/// @param index Index of the string within the span.
		/// @pre `index < length()`.
		/// @return View to a string in the span.
		[[nodiscard]] constexpr value_type at(size_type index) const noexcept
		{
			return m_ptr[index];
		}

		/// @brief Returns a view to the first string in the span.
		/// @pre `!this->empty()`.
		/// @return View to the first string in the span.
		[[nodiscard]] constexpr value_type front() const noexcept
		{
			return *m_ptr;
		}

		/// @brief Returns a pointer to the raw data of the span.
		/// @return Pointer to the first string pointer in the span.
		[[nodiscard]] constexpr const CharT* const* data() const noexcept
		{
			return m_ptr;
		}

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] constexpr iterator begin() const noexcept
		{
			return iterator{m_ptr};
		}

		/// @brief Returns an iterator sentinel to the end of the span.
		/// @return Iterator sentinel to the end of the span.
		[[nodiscard]] constexpr end_sentinel end() const noexcept
		{
			return end_sentinel{};
		}

		/// @}
		/// @name Capacity
		/// @{

		/// @brief Checks whether the span is empty.
		/// @return `true` if the span doesn't hold any strings, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return begin() == end();
		}

		/// @brief Returns the length of the span.
		/// @par Complexity
		/// Linear time with respect to the length of the span.
		/// @return Length of the span.
		[[nodiscard]] constexpr usize length() const noexcept
		{
			return std::ranges::distance(begin(), end());
		}

		/// @}

	  private:
		/// @brief Pointer to the underlying character pointer array.
		const CharT* const* m_ptr{nullptr};
	};

	/// @ingroup zstring_view_zspan
	/// @brief Span of NUL-terminated string views.
	using zstring_view_zspan = basic_zstring_view_zspan<char>;

	/// @ingroup zstring_view_zspan
	/// @brief Span of NUL-terminated wide string views.
	using wzstring_view_zspan = basic_zstring_view_zspan<wchar_t>;
} // namespace tr

//

#ifndef TR_DOXYGEN
/// @brief Designates basic_zstring_view_zspan as a view.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_view<tr::basic_zstring_view_zspan<CharT, Traits>> = true;

/// @brief Designates basic_zstring_view_zspan as a borrowed range.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_borrowed_range<tr::basic_zstring_view_zspan<CharT, Traits>> = true;
#endif