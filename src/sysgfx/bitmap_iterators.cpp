#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include "../../include/tr/utility/macro.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace tr {
	// Extracts an RGBA8 color value from a pixel.
	rgba8 pixel_color(const std::byte* data, pixel_format format);
} // namespace tr

tr::rgba8 tr::pixel_color(const std::byte* data, pixel_format format)
{
	u32 value{};
	switch (pixel_bytes(format)) {
	case 1:
		value = *(const u8*)data;
		break;
	case 2:
		value = *(const u16*)data;
		break;
	case 3: {
		const u8(&arr)[3]{*(const u8(*)[3])data};
		value = arr[0] << 16 | arr[1] << 8 | arr[2];
		break;
	}
	case 4:
		value = *(const u32*)data;
		break;
	}

	rgba8 color;
	SDL_GetRGBA(value, SDL_GetPixelFormatDetails(SDL_PixelFormat(format)), nullptr, &color.r, &color.g, &color.b, &color.a);
	return color;
}

tr::sub_bitmap::pixel_ref::pixel_ref(const std::byte* ptr, pixel_format format)
	: m_ptr{ptr}, m_format{format}
{
}

tr::sub_bitmap::pixel_ref::operator tr::rgba8() const
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

std::partial_ordering tr::sub_bitmap::iterator::operator<=>(const iterator& r) const
{
	if (m_pixel.m_ptr != nullptr && r.m_pixel.m_ptr != nullptr) {
		return m_pixel.m_ptr <=> r.m_pixel.m_ptr;
	}
	else if (m_pixel.m_ptr == nullptr && r.m_pixel.m_ptr == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::sub_bitmap::iterator::operator==(const iterator& r) const
{
	return *this <=> r == std::strong_ordering::equal;
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator*() const
{
	TR_ASSERT(m_pixel.m_ptr != nullptr && irect2{m_bitmap_size}.contains(m_bitmap_pos), "Tried to dereference an invalid bitmap iterator.");
	return m_pixel;
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator[](int diff) const
{
	return *(*this + diff);
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator[](glm::ivec2 diff) const
{
	return *(*this + diff);
}

tr::sub_bitmap::iterator::pointer tr::sub_bitmap::iterator::operator->() const
{
	return &m_pixel;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator++()
{
	return *this += 1;
}

tr::sub_bitmap::iterator tr::sub_bitmap::iterator::operator++(int)
{
	iterator copy{*this};
	++*this;
	return copy;
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
	m_pixel.m_ptr = m_pixel.m_ptr + m_bitmap_pitch * lines + pixel_bytes(m_pixel.m_format) * diff;
	return *this;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator+=(glm::ivec2 diff)
{
	return *this += (diff.y * m_bitmap_size.x + diff.x);
}

tr::sub_bitmap::iterator tr::operator+(const tr::sub_bitmap::iterator& it, int diff)
{
	tr::sub_bitmap::iterator copy{it};
	copy += diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator+(int diff, const tr::sub_bitmap::iterator& it)
{
	return it + diff;
}

tr::sub_bitmap::iterator tr::operator+(const tr::sub_bitmap::iterator& it, glm::ivec2 diff)
{
	tr::sub_bitmap::iterator copy{it};
	copy += diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator+(glm::ivec2 diff, const tr::sub_bitmap::iterator& it)
{
	return it + diff;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator--()
{
	return *this -= 1;
}

tr::sub_bitmap::iterator tr::sub_bitmap::iterator::operator--(int)
{
	iterator copy{*this};
	--*this;
	return copy;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator-=(int diff)
{
	return *this += -diff;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator-=(glm::ivec2 diff)
{
	return *this += -diff;
}

tr::sub_bitmap::iterator tr::operator-(const tr::sub_bitmap::iterator& it, int diff)
{
	tr::sub_bitmap::iterator copy{it};
	copy -= diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator-(int diff, const tr::sub_bitmap::iterator& it)
{
	return it - diff;
}

tr::sub_bitmap::iterator tr::operator-(const tr::sub_bitmap::iterator& it, glm::ivec2 diff)
{
	tr::sub_bitmap::iterator copy{it};
	copy -= diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator-(glm::ivec2 diff, const tr::sub_bitmap::iterator& it)
{
	return it - diff;
}

glm::ivec2 tr::sub_bitmap::iterator::pos() const
{
	return m_bitmap_pos;
}

tr::bitmap::pixel_ref::pixel_ref(std::byte* ptr, pixel_format format)
	: m_ptr{ptr}, m_format{format}
{
}

tr::bitmap::pixel_ref::operator tr::rgba8() const
{
	return pixel_color(m_ptr, m_format);
}

tr::bitmap::pixel_ref& tr::bitmap::pixel_ref::operator=(rgba8 color)
{
	const SDL_PixelFormatDetails* format_details{SDL_GetPixelFormatDetails(SDL_PixelFormat(m_format))};
	const u32 formatted{SDL_MapRGBA(format_details, nullptr, color.r, color.g, color.b, color.a)};
	switch (pixel_bytes(m_format)) {
	case 1:
		*(u8*)m_ptr = u8(formatted);
		break;
	case 2:
		*(u16*)m_ptr = u16(formatted);
		break;
	case 3: {
		u8(&arr)[3]{*(u8(*)[3])m_ptr};
		arr[0] = u8(formatted >> 16);
		arr[1] = u8(formatted >> 8);
		arr[2] = u8(formatted);
		break;
	}
	case 4:
		*(u32*)m_ptr = formatted;
	}
	return *this;
}

tr::bitmap::mut_it::mut_it(class bitmap& bitmap, glm::ivec2 pos)
	: m_pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, m_bitmap{&bitmap}
	, m_bitmap_pos{pos}
{
}

std::partial_ordering tr::bitmap::mut_it::operator<=>(const tr::bitmap::mut_it& r) const
{
	if (m_pixel.m_ptr != nullptr && r.m_pixel.m_ptr != nullptr) {
		return m_pixel.m_ptr <=> r.m_pixel.m_ptr;
	}
	else if (m_pixel.m_ptr == nullptr && r.m_pixel.m_ptr == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::bitmap::mut_it::operator==(const mut_it& r) const
{
	return *this <=> r == std::strong_ordering::equal;
}

tr::bitmap::mut_it::value_type tr::bitmap::mut_it::operator*() const
{
	TR_ASSERT(m_pixel.m_ptr != nullptr && irect2{m_bitmap->size()}.contains(m_bitmap_pos),
			  "Tried to dereference an invalid bitmap iterator.");

	return m_pixel;
}

tr::bitmap::mut_it::value_type tr::bitmap::mut_it::operator[](int diff) const
{
	return *(*this + diff);
}

tr::bitmap::mut_it::value_type tr::bitmap::mut_it::operator[](glm::ivec2 diff) const
{
	return *(*this + diff);
}

tr::bitmap::mut_it::pointer tr::bitmap::mut_it::operator->() const
{
	return &m_pixel;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator++()
{
	return *this += 1;
}

tr::bitmap::mut_it tr::bitmap::mut_it::operator++(int)
{
	mut_it copy{*this};
	++*this;
	return copy;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator+=(int diff)
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
	m_pixel.m_ptr = m_pixel.m_ptr + m_bitmap->pitch() * lines + pixel_bytes(m_pixel.m_format) * diff;
	m_bitmap_pos += glm::ivec2{diff, lines};
	return *this;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator+=(glm::ivec2 diff)
{
	return *this += (diff.y * m_bitmap->size().x + diff.x);
}

tr::bitmap::mut_it tr::operator+(const tr::bitmap::mut_it& it, int diff)
{
	tr::bitmap::mut_it copy{it};
	copy += diff;
	return copy;
}

tr::bitmap::mut_it tr::operator+(int diff, const tr::bitmap::mut_it& it)
{
	return it + diff;
}

tr::bitmap::mut_it tr::operator+(const tr::bitmap::mut_it& it, glm::ivec2 diff)
{
	tr::bitmap::mut_it copy{it};
	copy += diff;
	return copy;
}

tr::bitmap::mut_it tr::operator+(glm::ivec2 diff, const tr::bitmap::mut_it& it)
{
	return it + diff;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator--()
{
	return *this -= 1;
}

tr::bitmap::mut_it tr::bitmap::mut_it::operator--(int)
{
	mut_it copy{*this};
	--*this;
	return copy;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator-=(int diff)
{
	return *this += -diff;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator-=(glm::ivec2 diff)
{
	return *this += -diff;
}

tr::bitmap::mut_it tr::operator-(const tr::bitmap::mut_it& it, int diff)
{
	tr::bitmap::mut_it copy{it};
	copy -= diff;
	return copy;
}

tr::bitmap::mut_it tr::operator-(int diff, const tr::bitmap::mut_it& it)
{
	return it - diff;
}

tr::bitmap::mut_it tr::operator-(const tr::bitmap::mut_it& it, glm::ivec2 diff)
{
	tr::bitmap::mut_it copy{it};
	copy -= diff;
	return copy;
}

tr::bitmap::mut_it tr::operator-(glm::ivec2 diff, const tr::bitmap::mut_it& it)
{
	return it - diff;
}

int tr::operator-(const tr::bitmap::mut_it& l, const tr::bitmap::mut_it& r)
{
	TR_ASSERT(l.m_bitmap == r.m_bitmap && l.m_bitmap != nullptr, "Tried to subtract iterators to different bitmaps.");

	return (l.m_bitmap_pos.y * l.m_bitmap->size().x + l.m_bitmap_pos.x) - (r.m_bitmap_pos.y * r.m_bitmap->size().x + r.m_bitmap_pos.x);
}

glm::ivec2 tr::bitmap::mut_it::pos() const
{
	return m_bitmap_pos;
}