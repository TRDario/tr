/// @file
/// @brief Provides `tr::mapped_untyped_graphics_buffer_span`.

#pragma once
#include <tr/utility/pointer_iterator.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// @brief Object managing a map of a span of an untyped graphics buffer.
	/// @details
	/// `tr::mapped_untyped_graphics_buffer_span` represents the CPU mapping of a span of an untyped graphics buffer. Unlike
	/// `tr::mapped_graphics_buffer_span` or `tr::mapped_graphics_buffer_object`, it does not attempt to model the contents of the buffer,
	/// but is instead a minimal wrapper around a span of bytes that unmaps the buffer when destroyed.
	///
	/// Every non-empty instance of `tr::mapped_untyped_graphics_buffer_span` is associated with a graphics context, as well as with a
	/// buffer, and cannot outlive either.
	///
	/// Moved-from instances of `tr::mapped_untyped_graphics_buffer_span` are left in an empty state.
	class mapped_untyped_graphics_buffer_span
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Element type used by the span.
		using element_type = std::byte;

		/// @brief Value type used by the span.
		using value_type = std::byte;

		/// @brief Pointer type used by the span.
		using pointer = std::byte*;

		/// @brief Constant pointer type used by the span.
		using const_pointer = const std::byte*;

		/// @brief Reference type used by the span.
		using reference = value_type&;

		/// @brief Constant reference type used by the span.
		using const_reference = const value_type&;

		/// @brief Size type used by the span.
		using size_type = std::span<std::byte>::size_type;

		/// @brief Difference type used by the span.
		using difference_type = std::span<std::byte>::difference_type;

		/// @brief Constant iterator type used by the span.
		class const_iterator : public pointer_iterator<const_iterator, const std::byte*>
		{
		  public:
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<const_iterator, const std::byte*>::pointer_iterator;

			/// @}
		};

		/// @brief Iterator type used by the span.
		class iterator : public pointer_iterator<iterator, std::byte*>
		{
		  public:
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<iterator, std::byte*>::pointer_iterator;

			/// @}
			/// @name Observers
			/// @{

			/// @brief Converts the iterator into a constant iterator.
			/// @return Equivalent constant iterator.
			[[nodiscard]] operator const_iterator() const noexcept
			{
				return const_iterator{this->base()};
			}

			/// @}
		};

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @cond gl_interop
		/// @brief Constructs an untyped graphics buffer span map.
		/// @param context Reference to the graphics context the mapped graphics buffer is on.
		/// @param buffer Handle to an OpenGL buffer object.
		/// @param span Buffer object map span.
		[[nodiscard]] mapped_untyped_graphics_buffer_span(graphics_context& context, unsigned int buffer,
														  std::span<std::byte> span) noexcept;
		/// @endcond

		/// @brief Untyped graphics buffer span maps are not copyable.
		mapped_untyped_graphics_buffer_span(const mapped_untyped_graphics_buffer_span&) = delete;

		/// @brief Untyped graphics buffer span maps are not copyable.
		mapped_untyped_graphics_buffer_span& operator=(const mapped_untyped_graphics_buffer_span&) = delete;

		/// @brief Constructs an untyped graphics buffer span map by transferring ownership from another
		/// `tr::mapped_untyped_graphics_buffer_span`.
		/// @param rhs Untyped graphics buffer span map to move from.
		/// @par Invalidation
		/// All spans and references to elements of the map previously associated with `rhs` remain valid, but are now associated with the
		/// equivalent map in `*this`.
		/// @post `rhs` is left empty after the move.
		[[nodiscard]] mapped_untyped_graphics_buffer_span(mapped_untyped_graphics_buffer_span&& rhs) noexcept;

		/// @brief Destroys the currently held untyped graphics buffer span map and replaces it with one moved from another
		/// `tr::mapped_untyped_graphics_buffer_span`.
		/// @param rhs Untyped graphics buffer span map to move from.
		/// @par Invalidation
		/// All spans and references to elements of the map previously associated with `*this` are invalidated.
		///
		/// All spans and references to elements of the map previously associated with `rhs` remain valid, but are now associated with the
		/// equivalent map in `*this`.
		/// @post `rhs` is left empty after the move.
		/// @return Reference to `*this`.
		mapped_untyped_graphics_buffer_span& operator=(mapped_untyped_graphics_buffer_span&& rhs) noexcept;

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] iterator begin() const noexcept;

		/// @brief Returns a const-qualified iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] const_iterator cbegin() const noexcept;

		/// @brief Returns an iterator to the end of the span.
		/// @return Iterator to the end of the span.
		[[nodiscard]] iterator end() const noexcept;

		/// @brief Returns a const-qualified iterator to the end of the span.
		/// @return Iterator to the end of the span.
		[[nodiscard]] const_iterator cend() const noexcept;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a reference to the first element of the mapped span.
		/// @pre The span must not be empty.
		/// @return Reference to the first element of the mapped span.
		[[nodiscard]] reference front() const noexcept;

		/// @brief Returns a reference to the last element of the mapped span.
		/// @pre The span must not be empty.
		/// @return Reference to the last element of the mapped span.
		[[nodiscard]] reference back() const noexcept;

		/// @brief Returns a reference to an element of the mapped span.
		/// @pre `offset` must be less than the size of the span.
		/// @return Reference to an element of the mapped span.
		[[nodiscard]] reference at(size_type offset) const noexcept;

		/// @brief Returns a reference to an element of the mapped span.
		/// @pre `offset` must be less than the size of the span.
		/// @return Reference to an element of the mapped span.
		[[nodiscard]] reference operator[](size_type offset) const noexcept;

		/// @brief Returns a pointer to the beginning of the mapped span.
		/// @return Pointer to the beginning of the mapped span.
		[[nodiscard]] pointer data() const noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the number of elements in the span.
		/// @return Number of elements in the span.
		[[nodiscard]] size_type size() const noexcept;

		/// @brief Returns the size of the span in bytes.
		/// @return Size of the span in bytes.
		[[nodiscard]] size_type size_bytes() const noexcept;

		/// @brief Checks whether the span is empty.
		/// @return `true` if the span is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @brief Returns a reference to the graphics context the graphics buffer span map is on.
		/// @pre The span must not be empty.
		/// @return Reference to the graphics context the graphics buffer span map is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name Subviews
		/// @{

		/// @brief Returns a subspan to the first `Count` elements of the span.
		/// @tparam Count Number of elements in the subspan.
		/// @pre `Count` must be less than or equal to the size of the span.
		/// @return Subspan to the first `Count` elements of the span.
		template <size_type Count>
		[[nodiscard]] std::span<element_type, Count> first() const noexcept
		{
			return m_span.first<Count>();
		}

		/// @brief Returns a subspan to the first `count` elements of the span.
		/// @param count Number of elements in the subspan.
		/// @pre `count` must be less than or equal to the size of the span.
		/// @return Subspan to the first `count` elements of the span.
		[[nodiscard]] std::span<element_type> first(size_type count) const noexcept;

		/// @brief Returns a subspan to the last `Count` elements of the span.
		/// @tparam Count Number of elements in the subspan.
		/// @pre `Count` must be less than or equal to the size of the span.
		/// @return Subspan to the last `Count` elements of the span.
		template <size_type Count>
		[[nodiscard]] std::span<element_type, Count> last() const noexcept
		{
			return m_span.last<Count>();
		}

		/// @brief Returns a subspan to the last `count` elements of the span.
		/// @param count Number of elements in the subspan.
		/// @pre `count` must be less than or equal to the size of the span.
		/// @return Subspan to the last `count` elements of the span.
		[[nodiscard]] std::span<element_type> last(size_type count) const noexcept;

		/// @brief Returns a subspan of the span.
		/// @tparam Offset Starting offset of the subspan.
		/// @tparam Count Number of elements in the subspan, or `std::dynamic_extent`.
		/// @pre The beginning and end of the subspan must be within the bounds of the span.
		/// @return Subspan of the span.
		template <size_type Offset, size_type Count = std::dynamic_extent>
		[[nodiscard]] auto subspan() const noexcept
		{
			return m_span.subspan<Offset, Count>();
		}

		/// @brief Returns a subspan of the span.
		/// @param offset Starting offset of the subspan.
		/// @param count Number of elements in the subspan, or `std::dynamic_extent`.
		/// @pre The beginning and end of the subspan must be within the bounds of the span.
		/// @return Subspan of the span.
		[[nodiscard]] std::span<element_type> subspan(size_type offset, size_type count = std::dynamic_extent) const noexcept;

		/// @}

	  private:
		/// @brief OpenGL buffer object unmapper.
		struct gl_buffer_object_unmapper
		{
			/// @brief Reference to the graphics context the mapped graphics buffer is on.
			ref<graphics_context> context;

			//

			/// @brief Unmaps a buffer object.
			/// @param handle OpenGL buffer object handle.
			void operator()(unsigned int id) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL buffer object map.
		unique_handle<unsigned int, 0, gl_buffer_object_unmapper> m_handle;

		/// @brief Buffer object map span.
		std::span<element_type> m_span;
	};
} // namespace tr