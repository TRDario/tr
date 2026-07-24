///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements bitmap_iterators.hpp.                                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include "../../include/tr/utility/macro.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

/////////////////////////////////////////////////////////////// PIXEL COLOR ///////////////////////////////////////////////////////////////

namespace tr {
	namespace {
		using u24 = tr::u8[3];

		// Extracts an RGBA8 color value from a pixel.
		rgba8 pixel_color(const std::byte* data, pixel_format format)
		{
			u32 value{};
			switch (pixel_bytes(format)) {
			case 1:
				value = *reinterpret_cast<const u8*>(data);
				break;
			case 2:
				value = *reinterpret_cast<const u16*>(data);
				break;
			case 3: {
				const u24& arr{*reinterpret_cast<const u24*>(data)};
				value = arr[0] << 16 | arr[1] << 8 | arr[2];
				break;
			}
			case 4:
				value = *reinterpret_cast<const u32*>(data);
				break;
			}

			rgba8 color;
			SDL_GetRGBA(value, SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(format)), nullptr, &color.r, &color.g, &color.b,
						&color.a);
			return color;
		}
	} // namespace
} // namespace tr

//////////////////////////////////////////////////////// CONST/SUB-BITMAP ITERATOR ////////////////////////////////////////////////////////

tr::sub_bitmap::reference::reference(const std::byte* ptr, pixel_format format)
	: m_ptr{ptr}
	, m_format{format}
{
}

tr::sub_bitmap::reference::operator tr::rgba8() const
{
	return pixel_color(m_ptr, m_format);
}

tr::sub_bitmap::iterator::iterator(const sub_bitmap& bitmap, glm::ivec2 pos)
	: m_pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, m_bitmap_size{bitmap.size()}
	, m_bitmap_pitch{bitmap.pitch()}
	, m_bitmap_pos{pos}
{
}

std::partial_ordering tr::sub_bitmap::iterator::operator<=>(const iterator& rhs) const
{
	if (m_pixel.m_ptr != nullptr && rhs.m_pixel.m_ptr != nullptr) {
		return m_pixel.m_ptr <=> rhs.m_pixel.m_ptr;
	}
	else if (m_pixel.m_ptr == nullptr && rhs.m_pixel.m_ptr == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::sub_bitmap::iterator::operator==(const iterator& rhs) const
{
	return *this <=> rhs == std::strong_ordering::equal;
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator*() const
{
	TR_ASSERT(m_pixel.m_ptr != nullptr && rectangle<int>{m_bitmap_size}.contains(m_bitmap_pos),
			  "Tried to dereference an invalid bitmap iterator.");
	return m_pixel;
}

tr::sub_bitmap::iterator::pointer tr::sub_bitmap::iterator::operator->() const
{
	return &m_pixel;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator++()
{
	return *this += 1;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator+=(int diff)
{
	int lines{diff / m_bitmap_size.x};
	diff %= m_bitmap_size.x;
	if (diff + m_bitmap_pos.x >= m_bitmap_size.x) {
		++lines;
		diff -= m_bitmap_size.x;
	}
	else if (diff + m_bitmap_pos.x < 0) {
		--lines;
		diff += m_bitmap_size.x;
	}
	m_pixel.m_ptr += m_bitmap_pitch * lines + pixel_bytes(m_pixel.m_format) * diff;
	return *this;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator+=(glm::ivec2 diff)
{
	return *this += (diff.y * m_bitmap_size.x + diff.x);
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator--()
{
	return *this -= 1;
}

int tr::operator-(const tr::sub_bitmap::iterator& lhs, const tr::sub_bitmap::iterator& rhs)
{
	return (lhs.m_bitmap_pos.y * lhs.m_bitmap_size.x + lhs.m_bitmap_pos.x) -
		   (rhs.m_bitmap_pos.y * rhs.m_bitmap_size.x + rhs.m_bitmap_pos.x);
}

glm::ivec2 tr::sub_bitmap::iterator::pos() const
{
	return m_bitmap_pos;
}

///////////////////////////////////////////////////////////// BITMAP ITERATOR /////////////////////////////////////////////////////////////

tr::bitmap::reference::reference(std::byte* ptr, pixel_format format)
	: m_ptr{ptr}
	, m_format{format}
{
}

tr::bitmap::reference::operator tr::rgba8() const
{
	return pixel_color(m_ptr, m_format);
}

tr::bitmap::reference& tr::bitmap::reference::operator=(rgba8 color)
{
	const SDL_PixelFormatDetails* format_details{SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(m_format))};
	const u32 formatted{SDL_MapRGBA(format_details, nullptr, color.r, color.g, color.b, color.a)};
	switch (pixel_bytes(m_format)) {
	case 1:
		*reinterpret_cast<u8*>(m_ptr) = formatted;
		break;
	case 2:
		*reinterpret_cast<u16*>(m_ptr) = formatted;
		break;
	case 3: {
		u24& arr{*reinterpret_cast<u24*>(m_ptr)};
		arr[0] = formatted >> 16;
		arr[1] = formatted >> 8;
		arr[2] = formatted;
		break;
	}
	case 4:
		*reinterpret_cast<u32*>(m_ptr) = formatted;
	}
	return *this;
}

tr::bitmap::iterator::iterator(bitmap& bitmap, glm::ivec2 pos)
	: m_pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, m_bitmap{bitmap}
	, m_bitmap_pos{pos}
{
}

std::partial_ordering tr::bitmap::iterator::operator<=>(const tr::bitmap::iterator& rhs) const
{
	if (m_pixel.m_ptr != nullptr && rhs.m_pixel.m_ptr != nullptr) {
		return m_pixel.m_ptr <=> rhs.m_pixel.m_ptr;
	}
	else if (m_pixel.m_ptr == nullptr && rhs.m_pixel.m_ptr == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::bitmap::iterator::operator==(const iterator& rhs) const
{
	return *this <=> rhs == std::strong_ordering::equal;
}

tr::bitmap::iterator::value_type tr::bitmap::iterator::operator*() const
{
	TR_ASSERT(m_pixel.m_ptr != nullptr && rectangle<int>{m_bitmap->size()}.contains(m_bitmap_pos),
			  "Tried to dereference an invalid bitmap iterator.");

	return m_pixel;
}

tr::bitmap::iterator::pointer tr::bitmap::iterator::operator->() const
{
	return &m_pixel;
}

tr::bitmap::iterator& tr::bitmap::iterator::operator++()
{
	return *this += 1;
}

tr::bitmap::iterator& tr::bitmap::iterator::operator+=(int diff)
{
	TR_ASSERT(m_pixel.m_ptr != nullptr, "Tried to add to default-constructed bitmap iterator.");

	const glm::ivec2 bitmap_size{m_bitmap->size()};
	int lines{diff / bitmap_size.x};
	diff %= bitmap_size.x;
	if (diff + m_bitmap_pos.x >= bitmap_size.x) {
		++lines;
		diff -= bitmap_size.x;
	}
	else if (diff + m_bitmap_pos.x < 0) {
		--lines;
		diff += bitmap_size.x;
	}
	m_pixel.m_ptr += m_bitmap->pitch() * lines + pixel_bytes(m_pixel.m_format) * diff;
	m_bitmap_pos += glm::ivec2{diff, lines};
	return *this;
}

tr::bitmap::iterator& tr::bitmap::iterator::operator+=(glm::ivec2 diff)
{
	return *this += (diff.y * m_bitmap->size().x + diff.x);
}

tr::bitmap::iterator& tr::bitmap::iterator::operator--()
{
	return *this -= 1;
}

int tr::operator-(const tr::bitmap::iterator& lhs, const tr::bitmap::iterator& rhs)
{
	TR_ASSERT(lhs.m_bitmap == rhs.m_bitmap, "Tried to subtract iterators to different bitmaps.");

	return (lhs.m_bitmap_pos.y * lhs.m_bitmap->size().x + lhs.m_bitmap_pos.x) -
		   (rhs.m_bitmap_pos.y * rhs.m_bitmap->size().x + rhs.m_bitmap_pos.x);
}

glm::ivec2 tr::bitmap::iterator::pos() const
{
	return m_bitmap_pos;
}