/// @file
/// @brief Provides `tr::mapped_graphics_buffer_span`.

#pragma once
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/utility/ranges.hpp>

//

namespace tr
{
	/// @brief Object managing a map of a span of a graphics buffer.
	/// @details
	/// `tr::mapped_graphics_buffer_span` represents the CPU mapping of a typed graphics buffer span and models access to it as such. The
	/// span is automatically unmapped when the map is destroyed.
	///
	/// Every instance of `tr::mapped_graphics_buffer_span` is associated with a graphics context, as well as with a buffer, and
	/// cannot outlive either.
	///
	/// Moved-from instances of `tr::mapped_graphics_buffer_span` are left in a special 'invalid' state. Invalid
	/// `tr::mapped_graphics_buffer_span` instances may not be interacted with besides moving a new value into them and checking for
	/// validity using `valid()` or `operator bool()`.
	/// @tparam Element Type of the elements of the span.
	template <typename Element>
	class mapped_graphics_buffer_span
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Element type used by the span.
		using element_type = Element;

		/// @brief Value type used by the span.
		using value_type = Element;

		/// @brief Pointer type used by the span.
		using pointer = Element*;

		/// @brief Constant pointer type used by the span.
		using const_pointer = const Element*;

		/// @brief Reference type used by the span.
		using reference = Element&;

		/// @brief Constant reference type used by the span.
		using const_reference = const Element&;

		/// @brief Size type used by the span.
		using size_type = std::span<Element>::size_type;

		/// @brief Difference type used by the span.
		using difference_type = std::span<Element>::difference_type;

		/// @brief Constant iterator type used by the span.
		class const_iterator : public pointer_iterator<const_iterator, const element_type*>
		{
		  public:
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<const_iterator, const element_type*>::pointer_iterator;

			/// @}
		};

		/// @brief Iterator type used by the span.
		class iterator : public pointer_iterator<iterator, element_type*>
		{
		  public:
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<iterator, element_type*>::pointer_iterator;

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

		/// @cond implementation_details
		/// @brief Constructs a graphcis buffer span map.
		/// @param span Base graphics buffer span map.
		[[nodiscard]] explicit mapped_graphics_buffer_span(mapped_untyped_graphics_buffer_span&& span) noexcept
			: m_span{std::move(span)}
		{
		}
		/// @endcond

		/// @brief Graphics buffer span maps are not copyable.
		mapped_graphics_buffer_span(const mapped_graphics_buffer_span&) = delete;

		/// @brief Graphics buffer span maps are not copyable.
		mapped_graphics_buffer_span& operator=(const mapped_graphics_buffer_span&) = delete;

		/// @brief Constructs a graphics buffer span map by transferring ownership from another `tr::mapped_graphics_buffer_span`.
		/// @param rhs Graphics buffer span map to move from.
		/// @par Invalidation
		/// All spans and references to elements of the map previously associated with `rhs` remain valid, but are now associated with the
		/// equivalent map in `*this`.
		/// @post `rhs` is left empty after the move.
		[[nodiscard]] mapped_graphics_buffer_span(mapped_graphics_buffer_span&& rhs) = default;

		/// @brief Destroys the currently held graphics buffer span map and replaces it with one moved from another
		/// `tr::mapped_graphics_buffer_span`.
		/// @param rhs Graphics buffer span map to move from.
		/// @par Invalidation
		/// All spans and references to elements of the map previously associated with `*this` are invalidated.
		///
		/// All spans and references to elements of the map previously associated with `rhs` remain valid, but are now associated with the
		/// equivalent map in `*this`.
		/// @post `rhs` is left empty after the move.
		/// @return Reference to `*this`.
		mapped_graphics_buffer_span& operator=(mapped_graphics_buffer_span&& rhs) = default;

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] iterator begin() const noexcept
		{
			return iterator{data()};
		}

		/// @brief Returns a const-qualified iterator to the beginning of the span.
		/// @return Iterator to the beginning of the span.
		[[nodiscard]] const_iterator cbegin() const noexcept
		{
			return const_iterator{data()};
		}

		/// @brief Returns an iterator to the end of the span.
		/// @return Iterator to the end of the span.
		[[nodiscard]] iterator end() const noexcept
		{
			return iterator{data() + size()};
		}

		/// @brief Returns a const-qualified iterator to the end of the span.
		/// @return Iterator to the end of the span.
		[[nodiscard]] const_iterator cend() const noexcept
		{
			return const_iterator{data() + size()};
		}

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a reference to the first element of the mapped span.
		/// @pre The span must not be empty.
		/// @return Reference to the first element of the mapped span.
		[[nodiscard]] reference front() const noexcept
		{
			TR_ASSERT(!empty(), "Tried to get front element of an empty mapped graphics buffer span.");

			return data()[0];
		}

		/// @brief Returns a reference to the last element of the mapped span.
		/// @pre The span must not be empty.
		/// @return Reference to the last element of the mapped span.
		[[nodiscard]] reference back() const noexcept
		{
			TR_ASSERT(!empty(), "Tried to get back element of an empty mapped graphics buffer span.");

			return data()[size() - 1];
		}

		/// @brief Returns a reference to an element of the mapped span.
		/// @pre `offset` must be less than the size of the span.
		/// @return Reference to an element of the mapped span.
		[[nodiscard]] reference at(size_type offset) const noexcept
		{
			TR_ASSERT(offset < size(), "Tried to get out-of-bounds element at index {} of a mapped graphics buffer span of size {}.",
					  offset, size());

			return data()[offset];
		}

		/// @brief Returns a reference to an element of the mapped span.
		/// @pre `offset` must be less than the size of the span.
		/// @return Reference to an element of the mapped span.
		[[nodiscard]] reference operator[](size_type offset) const noexcept
		{
			return at(offset);
		}

		/// @brief Returns a pointer to the beginning of the mapped span.
		/// @return Pointer to the beginning of the mapped span.
		[[nodiscard]] pointer data() const noexcept
		{
			return reinterpret_cast<pointer>(m_span.data());
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the number of elements in the span.
		/// @return Number of elements in the span.
		[[nodiscard]] size_type size() const noexcept
		{
			return m_span.size() / sizeof(element_type);
		}

		/// @brief Returns the size of the span in bytes.
		/// @return Size of the span in bytes.
		[[nodiscard]] size_type size_bytes() const noexcept
		{
			return m_span.size_bytes();
		}

		/// @brief Checks whether the span is empty.
		/// @return `true` if the span is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept
		{
			return m_span.empty();
		}

		/// @brief Returns a reference to the graphics context the graphics buffer span map is on.
		/// @pre The span must not be empty.
		/// @return Reference to the graphics context the graphics buffer span map is on.
		[[nodiscard]] graphics_context& context() const noexcept
		{
			return m_span.context();
		}

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
			return reinterpret_span<element_type>(m_span).template first<Count>();
		}

		/// @brief Returns a subspan to the first `count` elements of the span.
		/// @param count Number of elements in the subspan.
		/// @pre `count` must be less than or equal to the size of the span.
		/// @return Subspan to the first `count` elements of the span.
		[[nodiscard]] std::span<element_type> first(size_type count) const noexcept
		{
			return reinterpret_span<element_type>(m_span).first(count);
		}

		/// @brief Returns a subspan to the last `Count` elements of the span.
		/// @tparam Count Number of elements in the subspan.
		/// @pre `Count` must be less than or equal to the size of the span.
		/// @return Subspan to the last `Count` elements of the span.
		template <size_type Count>
		[[nodiscard]] std::span<element_type, Count> last() const noexcept
		{
			return reinterpret_span<element_type>(m_span).template last<Count>();
		}

		/// @brief Returns a subspan to the last `count` elements of the span.
		/// @param count Number of elements in the subspan.
		/// @pre `count` must be less than or equal to the size of the span.
		/// @return Subspan to the last `count` elements of the span.
		[[nodiscard]] std::span<element_type> last(size_type count) const noexcept
		{
			return reinterpret_span<element_type>(m_span).last(count);
		}

		/// @brief Returns a subspan of the span.
		/// @tparam Offset Starting offset of the subspan.
		/// @tparam Count Number of elements in the subspan, or `std::dynamic_extent`.
		/// @pre The beginning and end of the subspan must be within the bounds of the span.
		/// @return Subspan of the span.
		template <size_type Offset, size_type Count = std::dynamic_extent>
		[[nodiscard]] auto subspan() const noexcept
		{
			return reinterpret_span<element_type>(m_span).template subspan<Offset, Count>();
		}

		/// @brief Returns a subspan of the span.
		/// @param offset Starting offset of the subspan.
		/// @param count Number of elements in the subspan, or `std::dynamic_extent`.
		/// @pre The beginning and end of the subspan must be within the bounds of the span.
		/// @return Subspan of the span.
		[[nodiscard]] std::span<element_type> subspan(size_type offset, size_type count = std::dynamic_extent) const noexcept
		{
			return reinterpret_span<element_type>(m_span).subspan(offset, count);
		}

		/// @}

	  private:
		/// @brief Base graphics buffer span map.
		mapped_untyped_graphics_buffer_span m_span;
	};
} // namespace tr