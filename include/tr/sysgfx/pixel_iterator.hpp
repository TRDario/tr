/// @file
/// @brief Provides `tr::const_pixel_iterator` and `tr::pixel_iterator`.

#pragma once
#include <tr/sysgfx/pixel_proxy.hpp>
#include <tr/utility/iterator_interface.hpp>
#include <tr/utility/opt_ref.hpp>

namespace tr
{
	class sub_bitmap;
	class bitmap;
} // namespace tr

//

namespace tr
{
	/// Immutable bitmap pixel iterator.
	class const_pixel_iterator : public iterator_interface<const_pixel_iterator, glm::ivec2>
	{
	  public:
		/// Value type used by the iterator.
		using value_type = const_pixel_proxy;

		/// Pointer type used by the iterator.
		using pointer = const value_type*;

		/// Difference type used by the iterator.
		using difference_type = int;

		/// @name Constructors
		/// @{

		/// Default-constructs an iterator.
		[[nodiscard]] const_pixel_iterator() noexcept = default;

		/// Constructs an iterator to a bitmap pixel.
		/// @param bitmap Pointed-to bitmap.
		/// @param pos Position of the iterator within the bitmap.
		[[nodiscard]] const_pixel_iterator(sub_bitmap bitmap, glm::ivec2 pos) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param rhs Iterator to compare with.
		/// @return Ordering of the iterators.
		[[nodiscard]] std::partial_ordering operator<=>(const const_pixel_iterator& rhs) const noexcept;

		/// Compares iterators for equality.
		/// @param rhs Iterator to compare with.
		/// @return Whether the iterators are equal.
		[[nodiscard]] bool operator==(const const_pixel_iterator& rhs) const noexcept;

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
		const_pixel_iterator& operator++() noexcept;

		/// Advances the iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		const_pixel_iterator& operator+=(difference_type diff) noexcept;

		/// Advances an iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		const_pixel_iterator& operator+=(glm::ivec2 diff) noexcept;

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		const_pixel_iterator& operator--() noexcept;

		/// Gets the difference between two iterators.
		/// @param lhs, rhs Iterators to get the difference of.
		/// @return Distance between `lhs` and `rhs`.
		friend difference_type operator-(const const_pixel_iterator& lhs, const const_pixel_iterator& rhs) noexcept;

		/// @}
		/// @name Position
		/// @{

		/// Gets the 2D position of the iterator within the bitmap.
		/// @return Psosition of the iterator within the bitmap.
		[[nodiscard]] glm::ivec2 pos() const noexcept;

		/// @}

	  private:
		/// Reference to a pixel, needed for the pointer dereference.
		const_pixel_proxy m_pixel;

		/// Size of the pointed-to bitmap.
		glm::ivec2 m_bitmap_size;

		/// Pitch of the pointed-to-bitmap.
		int m_bitmap_pitch;

		/// Position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};

	/// Mutable bitmap pixel iterator.
	class pixel_iterator : public iterator_interface<pixel_iterator, glm::ivec2>
	{
	  public:
		/// Value type used by the iterator.
		using value_type = pixel_proxy;

		/// Reference type used by the iterator.
		using reference = pixel_proxy;

		/// Constant reference type used by the iterator.
		using const_reference = const pixel_proxy;

		/// Pointer type used by the iterator.
		using pointer = const value_type*;

		/// Difference type used by the iterator.
		using difference_type = int;

		/// @name Constructors
		/// @{

		/// Default-constructs an iterator.
		[[nodiscard]] pixel_iterator() noexcept = default;

		/// Constructs an iterator to a bitmap pixel.
		/// @param bitmap Pointed-to bitmap.
		/// @param pos Position of the iterator within the bitmap.
		[[nodiscard]] pixel_iterator(bitmap& bitmap, glm::ivec2 pos) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param rhs Iterator to compare with.
		/// @return Ordering of the iterators.
		[[nodiscard]] std::partial_ordering operator<=>(const pixel_iterator& rhs) const noexcept;

		/// Compares iterators for equality.
		/// @param rhs Iterator to compare with.
		/// @return Whether the iterators are equal.
		[[nodiscard]] bool operator==(const pixel_iterator& rhs) const noexcept;

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
		pixel_iterator& operator++() noexcept;

		/// Advances the iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		pixel_iterator& operator+=(difference_type diff) noexcept;

		/// Advances an iterator.
		/// @param diff Amount to advance the iterator by.
		/// @return Reference to `*this`.
		pixel_iterator& operator+=(glm::ivec2 diff) noexcept;

		/// Decrements the iterator.
		/// @return Reference to `*this`.
		pixel_iterator& operator--() noexcept;

		/// Gets the difference between two iterators.
		/// @param lhs, rhs Iterators to get the difference of.
		/// @return Distance between `lhs` and `rhs`.
		friend difference_type operator-(const pixel_iterator& lhs, const pixel_iterator& rhs) noexcept;

		/// @}
		/// @name Position
		/// @{

		/// Gets the 2D position of the iterator within the bitmap.
		/// @return Psosition of the iterator within the bitmap.
		[[nodiscard]] glm::ivec2 pos() const noexcept;

		/// @}

	  private:
		/// Reference to a pixel, needed for the pointer dereference.
		pixel_proxy m_pixel;

		/// Pointed-to-bitmap.
		opt_ref<bitmap> m_bitmap;

		/// Position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};
} // namespace tr