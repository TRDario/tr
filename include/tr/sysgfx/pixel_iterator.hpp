/// @file
/// @brief Provides `tr::const_pixel_iterator` and `tr::pixel_iterator`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/utility/iterator_interface.hpp>
#include <tr/utility/opt_ref.hpp>

namespace tr
{
	class const_pixel_proxy;
	class pixel_proxy;
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
		using difference_type = ssize;

		/// @cond implementation_details
		/// @name Constructors and destructors
		/// @{

		/// Constructs a singular iterator.
		[[nodiscard]] const_pixel_iterator() noexcept = default;

		/// Constructs an iterator to a bitmap pixel.
		/// @param bitmap_begin Pointer to the beginning of the bitmap pixel data.
		/// @param bitmap_pitch Pitch of the bitmap in bytes.
		/// @param bitmap_width Width of the bitmap in pixels.
		/// @param bitmap_format Pixel format of the bitmap.
		/// @param byte_offset Byte offset within the bitmap pixel data.
		[[nodiscard]] const_pixel_iterator(const std::byte* bitmap_begin, int bitmap_pitch, int bitmap_width, pixel_format bitmap_format,
										   int byte_offset) noexcept;

		/// @}
		/// @endcond
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param rhs Iterator to compare with.
		/// @return Ordering of the iterators.
		[[nodiscard]] std::strong_ordering operator<=>(const const_pixel_iterator& rhs) const noexcept;

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
		/// Pointer to the beginning of the bitmap's pixel data.
		const std::byte* m_bitmap_begin;

		/// Pitch of the bitmap in bytes.
		int m_bitmap_pitch;

		/// Width of the bitmap in pixels.
		int m_bitmap_width;

		/// Pixel format of the bitmap.
		pixel_format m_bitmap_format;

		/// Byte offset within the bitmap.
		int m_byte_offset;

		//

		/// Computes the pixel offset from the beginning of the bitmap.
		[[nodiscard]] difference_type pixel_offset() const noexcept;
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
		using difference_type = ssize;

		/// @cond implementation_details
		/// @name Constructors and destructors
		/// @{

		/// Constructs a singular iterator.
		[[nodiscard]] pixel_iterator() noexcept = default;

		/// Constructs an iterator to a bitmap pixel.
		/// @param bitmap_begin Pointer to the beginning of the bitmap pixel data.
		/// @param bitmap_pitch Pitch of the bitmap in bytes.
		/// @param bitmap_width Width of the bitmap in pixels.
		/// @param bitmap_format Pixel format of the bitmap.
		/// @param byte_offset Byte offset within the bitmap pixel data.
		[[nodiscard]] pixel_iterator(std::byte* bitmap_begin, int bitmap_pitch, int bitmap_width, pixel_format bitmap_format,
									 int byte_offset) noexcept;

		/// @}
		/// @endcond
		/// @name Comparison operators
		/// @{

		/// Compares iterators.
		/// @param rhs Iterator to compare with.
		/// @return Ordering of the iterators.
		[[nodiscard]] std::strong_ordering operator<=>(const pixel_iterator& rhs) const noexcept;

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
		/// Pointer to the beginning of the bitmap's pixel data.
		std::byte* m_bitmap_begin;

		/// Pitch of the bitmap in bytes.
		int m_bitmap_pitch;

		/// Width of the bitmap in pixels.
		int m_bitmap_width;

		/// Pixel format of the bitmap.
		pixel_format m_bitmap_format;

		/// Byte offset within the bitmap.
		int m_byte_offset;

		//

		/// Computes the pixel offset from the beginning of the bitmap.
		[[nodiscard]] difference_type pixel_offset() const noexcept;
	};
} // namespace tr