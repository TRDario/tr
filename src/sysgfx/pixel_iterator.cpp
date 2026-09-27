/// @file
/// @brief Implements bitmap_iterators.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/pixel_iterator.hpp>
#include <tr/sysgfx/pixel_proxy.hpp>
#include <tr/utility/macro.hpp>

//

tr::const_pixel_iterator::const_pixel_iterator(const std::byte* bitmap_begin, int bitmap_pitch, int bitmap_width,
											   pixel_format bitmap_format, int byte_offset) noexcept
	: m_bitmap_begin{bitmap_begin}
	, m_bitmap_pitch{bitmap_pitch}
	, m_bitmap_width{bitmap_width}
	, m_bitmap_format{bitmap_format}
	, m_byte_offset{byte_offset}
{
}

std::strong_ordering tr::const_pixel_iterator::operator<=>(const const_pixel_iterator& rhs) const noexcept
{
	TR_ASSERT(m_bitmap_begin == rhs.m_bitmap_begin && m_bitmap_width == rhs.m_bitmap_width,
			  "Tried to compare pixel iterators from different bitmaps.");

	return m_byte_offset <=> rhs.m_byte_offset;
}

bool tr::const_pixel_iterator::operator==(const const_pixel_iterator& rhs) const noexcept
{
	TR_ASSERT(m_bitmap_begin == rhs.m_bitmap_begin && m_bitmap_width == rhs.m_bitmap_width,
			  "Tried to compare pixel iterators from different bitmaps.");

	return m_byte_offset == rhs.m_byte_offset;
}

tr::const_pixel_iterator::value_type tr::const_pixel_iterator::operator*() const noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to dereference a singular pixel iterator.");

	return const_pixel_proxy{m_bitmap_begin + m_byte_offset, m_bitmap_format};
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator++() noexcept
{
	return *this += 1;
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator+=(difference_type diff) noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to perform arithmetic on a singular pixel iterator.");

	const difference_type new_pixel_offset{pixel_offset() + diff};
	const difference_type new_pixel_y_offset{new_pixel_offset / m_bitmap_width};
	const difference_type new_pixel_x_offset{new_pixel_offset % m_bitmap_width};
	m_byte_offset = new_pixel_y_offset * m_bitmap_pitch + new_pixel_x_offset * pixel_bytes(m_bitmap_format);
	return *this;
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * m_bitmap_width + diff.x);
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator--() noexcept
{
	return *this -= 1;
}

tr::const_pixel_iterator::difference_type tr::operator-(const const_pixel_iterator& lhs, const const_pixel_iterator& rhs) noexcept
{
	TR_ASSERT(lhs.m_bitmap_begin == rhs.m_bitmap_begin && lhs.m_bitmap_width == rhs.m_bitmap_width,
			  "Tried to subtract pixel iterators from different bitmaps.");

	return lhs.pixel_offset() - rhs.pixel_offset();
}

glm::ivec2 tr::const_pixel_iterator::pos() const noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to get position of a singular pixel iterator.");

	const difference_type pixel_offset{this->pixel_offset()};
	return glm::ivec2{pixel_offset / m_bitmap_width, pixel_offset % m_bitmap_width};
}

tr::const_pixel_iterator::difference_type tr::const_pixel_iterator::pixel_offset() const noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to get pixel offset of a singular pixel iterator.");

	const difference_type current_pixel_y_offset{m_byte_offset / m_bitmap_pitch};
	const difference_type current_pixel_x_offset{m_byte_offset % m_bitmap_pitch / pixel_bytes(m_bitmap_format)};
	return current_pixel_y_offset * m_bitmap_width + current_pixel_x_offset;
}

//

tr::pixel_iterator::pixel_iterator(std::byte* bitmap_begin, int bitmap_pitch, int bitmap_width, pixel_format bitmap_format,
								   int byte_offset) noexcept
	: m_bitmap_begin{bitmap_begin}
	, m_bitmap_pitch{bitmap_pitch}
	, m_bitmap_width{bitmap_width}
	, m_bitmap_format{bitmap_format}
	, m_byte_offset{byte_offset}
{
}

std::strong_ordering tr::pixel_iterator::operator<=>(const pixel_iterator& rhs) const noexcept
{
	TR_ASSERT(m_bitmap_begin == rhs.m_bitmap_begin && m_bitmap_width == rhs.m_bitmap_width,
			  "Tried to compare pixel iterators from different bitmaps.");

	return m_byte_offset <=> rhs.m_byte_offset;
}

bool tr::pixel_iterator::operator==(const pixel_iterator& rhs) const noexcept
{
	TR_ASSERT(m_bitmap_begin == rhs.m_bitmap_begin && m_bitmap_width == rhs.m_bitmap_width,
			  "Tried to compare pixel iterators from different bitmaps.");

	return m_byte_offset == rhs.m_byte_offset;
}

tr::pixel_iterator::value_type tr::pixel_iterator::operator*() const noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to dereference a singular pixel iterator.");

	return pixel_proxy{m_bitmap_begin + m_byte_offset, m_bitmap_format};
}

tr::pixel_iterator& tr::pixel_iterator::operator++() noexcept
{
	return *this += 1;
}

tr::pixel_iterator& tr::pixel_iterator::operator+=(difference_type diff) noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to perform arithmetic on a singular pixel iterator.");

	const difference_type new_pixel_offset{pixel_offset() + diff};
	const difference_type new_pixel_y_offset{new_pixel_offset / m_bitmap_width};
	const difference_type new_pixel_x_offset{new_pixel_offset % m_bitmap_width};
	m_byte_offset = new_pixel_y_offset * m_bitmap_pitch + new_pixel_x_offset * pixel_bytes(m_bitmap_format);
	return *this;
}

tr::pixel_iterator& tr::pixel_iterator::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * m_bitmap_width + diff.x);
}

tr::pixel_iterator& tr::pixel_iterator::operator--() noexcept
{
	return *this -= 1;
}

tr::pixel_iterator::difference_type tr::operator-(const pixel_iterator& lhs, const pixel_iterator& rhs) noexcept
{
	TR_ASSERT(lhs.m_bitmap_begin == rhs.m_bitmap_begin && lhs.m_bitmap_width == rhs.m_bitmap_width,
			  "Tried to subtract pixel iterators from different bitmaps.");

	return lhs.pixel_offset() - rhs.pixel_offset();
}

glm::ivec2 tr::pixel_iterator::pos() const noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to get position of a singular pixel iterator.");

	const difference_type pixel_offset{this->pixel_offset()};
	return glm::ivec2{pixel_offset / m_bitmap_width, pixel_offset % m_bitmap_width};
}

tr::pixel_iterator::difference_type tr::pixel_iterator::pixel_offset() const noexcept
{
	TR_ASSERT(m_bitmap_begin != nullptr, "Tried to get pixel offset of a singular pixel iterator.");

	const difference_type current_pixel_y_offset{m_byte_offset / m_bitmap_pitch};
	const difference_type current_pixel_x_offset{m_byte_offset % m_bitmap_pitch / pixel_bytes(m_bitmap_format)};
	return current_pixel_y_offset * m_bitmap_width + current_pixel_x_offset;
}