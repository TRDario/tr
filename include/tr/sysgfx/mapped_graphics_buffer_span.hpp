/// @file
/// @brief Provides `tr::mapped_graphics_buffer_span`.

#pragma once
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Mapped graphics buffer span.
	/// @details
	/// `tr::mapped_graphics_buffer_span` represents the CPU mapping of a typed graphics buffer span and models access to it as such. The
	/// span is automatically unmapped when the map is destroyed.
	///
	/// Every instance of `tr::mapped_graphics_buffer_span` is associated with a graphics context, as well as with a buffer, and
	/// cannot outlive either.
	///
	/// Moved-from instances of `tr::mapped_graphics_buffer_span` are left in a special 'invalid' state. Invalid
	/// `tr::mapped_graphics_buffer_span` instances may not be interacted with besides moving a new value into them.
	/// @tparam Element Type of the elements of the span.
	template <typename Element>
	class mapped_graphics_buffer_span
	{
	  public:
		/// Element type used by the span.
		using element_type = Element;

		/// Value type used by the span.
		using value_type = Element;

		/// Pointer type used by the span.
		using pointer = Element*;

		/// Constant pointer type used by the span.
		using const_pointer = const Element*;

		/// Reference type used by the span.
		using reference = Element&;

		/// Constant reference type used by the span.
		using const_reference = const Element&;

		/// Size type used by the span.
		using size_type = std::span<Element>::size_type;

		/// Difference type used by the span.
		using difference_type = std::span<Element>::difference_type;

		/// Iterator type used by the span.
		using iterator = std::span<Element>::iterator;

		/// @name Constructors and destructors
		/// @{

		/// @cond implementation_details

		/// Wraps an untyped buffer map.
		/// @param map Untyped buffer map.
		[[nodiscard]] explicit mapped_graphics_buffer_span(mapped_untyped_graphics_buffer_span&& map) noexcept
			: m_raw_map{std::move(map)}
		{
		}

		/// @endcond

		/// Graphics buffer span maps are not copyable.
		mapped_graphics_buffer_span(const mapped_graphics_buffer_span&) = delete;

		/// Moves a graphics buffer span map.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Graphics buffer span map to move.
		[[nodiscard]] mapped_graphics_buffer_span(mapped_graphics_buffer_span&& rhs) = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Graphics buffer span maps are not copyable.
		mapped_graphics_buffer_span& operator=(const mapped_graphics_buffer_span&) = delete;

		/// Moves a graphics buffer span map.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Graphics buffer span map to move.
		/// @return Reference to `*this`.
		mapped_graphics_buffer_span& operator=(mapped_graphics_buffer_span&& rhs) = default;

		/// @}
		/// @name Span conversion
		/// @{

		/// Casts the map into a regular span.
		/// @return Base span of the map.
		[[nodiscard]] operator std::span<Element>() const noexcept
		{
			return span();
		}

		/// Casts the map into a regular span.
		/// @return Span of bytes covering the buffer map.
		[[nodiscard]] std::span<Element> span() const noexcept
		{
			return as_objects<Element>(m_raw_map.span());
		}

		/// @}
		/// @name Access
		/// @{

		/// Indexes into the map.
		/// @param index Index of the element within the map to access.
		/// @return Reference to an element of the map.
		[[nodiscard]] reference operator[](usize index) const noexcept
		{
			return span()[index];
		}

		/// Gets a pointer to the data of the map.
		/// @return Pointer to the data of the map.
		[[nodiscard]] pointer data() const noexcept
		{
			return span().data();
		}

		/// Gets the size of the map.
		/// @return Number of elements in the map.
		[[nodiscard]] size_type size() const noexcept
		{
			return span().size();
		}

		/// Gets an iterator to the beginning of the map.
		/// @return Iterator to the beginning of the map.
		[[nodiscard]] iterator begin() const noexcept
		{
			return span().begin();
		}

		/// Gets an iterator to the end of the map.
		/// @return Iterator to the end of the map.
		[[nodiscard]] iterator end() const noexcept
		{
			return span().end();
		}

		/// @}

	  private:
		/// Wrapped untyped buffer map.
		mapped_untyped_graphics_buffer_span m_raw_map;
	};
} // namespace tr