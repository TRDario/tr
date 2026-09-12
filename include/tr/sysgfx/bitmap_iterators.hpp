/// @file
/// @brief Provides bitmap iterators.

#pragma once
#include "../utility/iterator_interface.hpp"
#include "../utility/opt_ref.hpp"
#include "bitmap.hpp"

//

namespace tr
{
	/// Read-only reference to a bitmap pixel.
	class sub_bitmap::reference
	{
	  public:
		/// Gets the pixel's color value.
		/// @return Color of the pixel.
		[[nodiscard]] operator rgba8() const noexcept;

	  private:
		/// Pointer to the pixel data.
		const std::byte* m_ptr;

		/// Format of the pixel.
		pixel_format m_format;

		//

		/// Undefined, required to default-construct iterators.
		[[nodiscard]] reference() noexcept = default;

		/// Wraps a pointer to the pixel data.
		/// @param ptr Pointer to the pixel data.
		/// @param pixel_format Format of the pixel.
		[[nodiscard]] reference(const std::byte* ptr, pixel_format format) noexcept;

		//

		// Uses the private constructor.
		friend class sub_bitmap;

		// Uses the private constructor.
		friend class iterator;
	};

	/// Immutable bitmap pixel iterator.
	class sub_bitmap::iterator : public iterator_interface<iterator, glm::ivec2>
	{
	  public:
		/// Value type used by the iterator.
		using value_type = reference;

		/// Pointer type used by the iterator.
		using pointer = const value_type*;

		/// Difference type used by the iterator.
		using difference_type = int;

		/// @name Constructors
		/// @{

		/// Default-constructs an iterator.
		[[nodiscard]] iterator() noexcept = default;

		/// Constructs an iterator to a bitmap pixel.
		/// @param bitmap Pointed-to bitmap.
		/// @param pos Position of the iterator within the bitmap.
		[[nodiscard]] iterator(sub_bitmap bitmap, glm::ivec2 pos) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param rhs Iterator to compare with.
		/// @return Ordering of the iterators.
		[[nodiscard]] std::partial_ordering operator<=>(const iterator& rhs) const noexcept;

		/// Compares iterators for equality.
		/// @param rhs Iterator to compare with.
		/// @return Whether the iterators are equal.
		[[nodiscard]] bool operator==(const iterator& rhs) const noexcept;

		/// @}
		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Pixel reference.
		[[nodiscard]] value_type operator*() const noexcept;

		/// Dereferences the iterator through a pointer.
		/// @return Pointer to a pixel reference.
		[[nodiscard]] pointer operator->() const noexcept;

		/// Increments the iterator.
		/// @return Reference to `*this`.
		iterator& operator++() noexcept;

		/// Advances the iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		iterator& operator+=(difference_type diff) noexcept;

		/// Advances an iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		iterator& operator+=(glm::ivec2 diff) noexcept;

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		iterator& operator--() noexcept;

		/// Gets the difference between two iterators.
		/// @param lhs, rhs Iterators to get the difference of.
		/// @return Distance between `lhs` and `rhs`.
		friend difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept;

		/// @}
		/// @name Position
		/// @{

		/// Gets the 2D position of the iterator within the bitmap.
		/// @return Psosition of the iterator within the bitmap.
		[[nodiscard]] glm::ivec2 pos() const noexcept;

		/// @}

	  private:
		/// Reference to a pixel, needed for the pointer dereference.
		reference m_pixel;

		/// Size of the pointed-to bitmap.
		glm::ivec2 m_bitmap_size;

		/// Pitch of the pointed-to-bitmap.
		int m_bitmap_pitch;

		/// Position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};

	/// Mutable reference to a bitmap pixel.
	class bitmap::reference
	{
	  public:
		/// Gets the pixel's color value.
		/// @return Color of the pixel.
		[[nodiscard]] operator rgba8() const noexcept;

		/// Sets the pixel's color value.
		/// @param color Color to set.
		/// @return Reference to the pixel.
		reference& operator=(rgba8 color) noexcept;

	  private:
		/// Pointer to the pixel data.
		std::byte* m_ptr;

		/// Format of the pixel.
		pixel_format m_format;

		//

		/// Undefined, required to default-construct iterators.
		[[nodiscard]] reference() noexcept = default;

		/// Wraps a pointer to the pixel data.
		/// @param ptr Pointer to the pixel data.
		/// @param format Format of the pixel.
		[[nodiscard]] reference(std::byte* ptr, pixel_format format) noexcept;

		//

		// Uses the private constructor.
		friend class iterator;
	};

	/// Mutable bitmap pixel iterator.
	class bitmap::iterator : public iterator_interface<iterator, glm::ivec2>
	{
	  public:
		/// Value type used by the iterator.
		using value_type = reference;

		/// Reference type used by the iterator.
		using reference = reference;

		/// Constant reference type used by the iterator.
		using const_reference = const reference;

		/// Pointer type used by the iterator.
		using pointer = const value_type*;

		/// Difference type used by the iterator.
		using difference_type = int;

		/// @name Constructors
		/// @{

		/// Default-constructs an iterator.
		[[nodiscard]] iterator() noexcept = default;

		/// Constructs an iterator to a bitmap pixel.
		/// @param bitmap Pointed-to bitmap.
		/// @param pos Position of the iterator within the bitmap.
		[[nodiscard]] iterator(bitmap& bitmap, glm::ivec2 pos) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param rhs Iterator to compare with.
		/// @return Ordering of the iterators.
		[[nodiscard]] std::partial_ordering operator<=>(const iterator& rhs) const noexcept;

		/// Compares iterators for equality.
		/// @param rhs Iterator to compare with.
		/// @return Whether the iterators are equal.
		[[nodiscard]] bool operator==(const iterator& rhs) const noexcept;

		/// @}
		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		/// @return Pixel reference.
		[[nodiscard]] value_type operator*() const noexcept;

		/// Dereferences the iterator through a pointer.
		/// @return Pointer to a pixel reference.
		[[nodiscard]] pointer operator->() const noexcept;

		/// Increments the iterator.
		/// @return Reference to `*this`.
		iterator& operator++() noexcept;

		/// Advances the iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		iterator& operator+=(difference_type diff) noexcept;

		/// Advances an iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		iterator& operator+=(glm::ivec2 diff) noexcept;

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		iterator& operator--() noexcept;

		/// Gets the difference between two iterators.
		/// @param lhs, rhs Iterators to get the difference of.
		/// @return Distance between `lhs` and `rhs`.
		friend difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept;

		/// @}
		/// @name Position
		/// @{

		/// Gets the 2D position of the iterator within the bitmap.
		/// @return Psosition of the iterator within the bitmap.
		[[nodiscard]] glm::ivec2 pos() const noexcept;

		/// @}

	  private:
		/// Reference to a pixel, needed for the pointer dereference.
		reference m_pixel;

		/// Pointed-to-bitmap.
		opt_ref<bitmap> m_bitmap;

		/// Position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};
} // namespace tr