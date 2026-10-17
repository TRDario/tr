/// @file
/// @brief Provides `tr::basic_zstring_view_zspan`.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// NUL-terminated span of NUL-terminated string views.
	/// @tparam CharT Character type used by the strings of the span.
	/// @tparam Traits Character traits used by the strings of the span.
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_zstring_view_zspan
	{
	  public:
		/// Size type used by the span.
		using size_type = usize;

		/// Difference type used by the span.
		using difference_type = ssize;

		/// Value type used by the span.
		using value_type = zstring_view;

		/// End sentinel type used by the span.
		struct end_sentinel
		{
		};

		/// Iterator type used by the span.
		class iterator : public pointer_iterator<iterator, const CharT* const*>
		{
		  public:
			using pointer_iterator<iterator, const CharT**>::pointer_iterator;

			/// @name Comparison operators
			/// @{

			/// Compares the iterator to an end sentinel.
			/// @return `true` if the iterator is pointing to the end of the span, `false` otherwise.
			[[nodiscard]] constexpr bool operator==(end_sentinel) const noexcept
			{
				return this->base() == nullptr || *(this->base()) == nullptr;
			}

			/// @}
			/// @name Dereferencing operators
			/// @{

			/// Dereferences the iterator.
			/// @return Dereferenced string view.
			[[nodiscard]] constexpr basic_zstring_view<CharT, Traits> operator*() const noexcept
			{
				return basic_zstring_view<CharT, Traits>{*(this->base())};
			}

			/// Dereferences the iterator with a subscript.
			/// @param diff Offset to the target value.
			/// @return Dereferenced string view.
			[[nodiscard]] constexpr basic_zstring_view<CharT, Traits> operator[](difference_type diff) const noexcept
			{
				return *(*this + diff);
			}

			/// @}

		  private:
			using pointer_iterator<iterator, const CharT* const*>::operator*;

			using pointer_iterator<iterator, const CharT* const*>::operator[];
		};

		/// Constant iterator type used by the span.
		using const_iterator = iterator;

		/// @name Constructors and destructors
		/// @{

		/// Constructs an empty NUL-terminated string view span.
		[[nodiscard]] constexpr basic_zstring_view_zspan() noexcept = default;

		/// Constructs a NUL-terminated string view span.
		/// @param ptr Pointer to the NUL-terminated string view span.
		[[nodiscard]] constexpr basic_zstring_view_zspan(CharT* const* ptr) noexcept
			: m_ptr{const_cast<const CharT* const*>(ptr)}
		{
		}

		/// Constructs a NUL-terminated string view span.
		/// @param ptr Pointer to the NUL-terminated string view span.
		[[nodiscard]] constexpr basic_zstring_view_zspan(const CharT* const* ptr) noexcept
			: m_ptr{ptr}
		{
		}

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] constexpr iterator begin() const noexcept
		{
			return iterator{m_ptr};
		}

		/// Gets an iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return const_iterator{m_ptr};
		}

		/// Gets an iterator sentinel to the end of the span.
		/// @return Iterator sentinel to the end of the span.
		[[nodiscard]] constexpr end_sentinel end() const noexcept
		{
			return end_sentinel{};
		}

		/// Gets an iterator sentinel to the end of the span.
		/// @return Iterator sentinel to the end of the span.
		[[nodiscard]] constexpr end_sentinel cend() const noexcept
		{
			return end_sentinel{};
		}

		/// @}
		/// @name Access
		/// @{

		/// Accesses a specific string.
		/// @param index Index of the string within the span.
		/// @return View to a string in the span.
		[[nodiscard]] constexpr value_type operator[](size_type index) const noexcept
		{
			return m_ptr[index];
		}

		/// Accesses a specific string.
		/// @param index Index of the string within the span.
		/// @return View to a string in the span.
		[[nodiscard]] constexpr value_type at(size_type index) const noexcept
		{
			return m_ptr[index];
		}

		/// Accesses the first string in the span.
		/// @return View to the first string in the span.
		[[nodiscard]] constexpr value_type front() const noexcept
		{
			return *m_ptr;
		}

		/// Gets a pointer to the raw data of the span.
		/// @return Pointer to the first string pointer in the span.
		[[nodiscard]] constexpr const CharT* const* data() const noexcept
		{
			return m_ptr;
		}

		/// @}
		/// @name Length
		/// @{

		/// Gets whether the span is empty.
		/// @return `true` if the span doesn't hold any strings, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return begin() == end();
		}

		/// @}

	  private:
		/// Pointer to the underlying character pointer array.
		const CharT* const* m_ptr{nullptr};
	};

	/// Span of NUL-terminated string views.
	using zstring_view_zspan = basic_zstring_view_zspan<char>;
} // namespace tr

//

#ifndef TR_DOXYGEN
/// Designates basic_zstring_view_zspan as a view.
/// @tparam CharT Character type used by the strings.
/// @tparam Traits Character traits used by the strings.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_view<tr::basic_zstring_view_zspan<CharT, Traits>> = true;

/// Designates basic_zstring_view_zspan as a borrowed range.
/// @tparam CharT Character type used by the strings.
/// @tparam Traits Character traits used by the strings.
template <typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_borrowed_range<tr::basic_zstring_view_zspan<CharT, Traits>> = true;
#endif