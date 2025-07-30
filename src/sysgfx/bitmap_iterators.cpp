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
	std::uint32_t value{};
	switch (pixel_bytes(format)) {
	case 1:
		value = *reinterpret_cast<const std::uint8_t*>(data);
		break;
	case 2:
		value = *reinterpret_cast<const std::uint16_t*>(data);
		break;
	case 3: {
		const std::uint8_t (&arr)[3]{*reinterpret_cast<const std::uint8_t (*)[3]>(data)};
		value = arr[0] << 16 | arr[1] << 8 | arr[2];
		break;
	}
	case 4:
		value = *reinterpret_cast<const std::uint32_t*>(data);
		break;
	}

	rgba8 color;
	SDL_GetRGBA(value, SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(format)), nullptr, &color.r, &color.g, &color.b, &color.a);
	return color;
}

tr::sub_bitmap::pixel_ref::pixel_ref(const std::byte* ptr, pixel_format format)
	: ptr{ptr}, format{format}
{
}

tr::sub_bitmap::pixel_ref::operator tr::rgba8() const
{
	return pixel_color(ptr, format);
}

tr::sub_bitmap::iterator::iterator(const sub_bitmap& bitmap, glm::ivec2 pos)
	: pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, bitmap_size{bitmap.size()}
	, bitmap_pitch{bitmap.pitch()}
	, bitmap_pos{pos}
{
}

std::partial_ordering tr::sub_bitmap::iterator::operator<=>(const iterator& r) const
{
	if (pixel.ptr != nullptr && r.pixel.ptr != nullptr) {
		return pixel.ptr <=> r.pixel.ptr;
	}
	else if (pixel.ptr == nullptr && r.pixel.ptr == nullptr) {
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
	TR_ASSERT(pixel.ptr != nullptr && irect2{bitmap_size}.contains(bitmap_pos), "Tried to dereference an invalid bitmap iterator.");
	return pixel;
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
	return &pixel;
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
	int lines{diff / bitmap_size.x};
	diff %= bitmap_size.x;
	if (diff + bitmap_pos.x >= bitmap_size.x) {
		++lines;
		diff -= bitmap_size.x;
	}
	else if (diff + bitmap_pos.x < 0) {
		--lines;
		diff += bitmap_size.x;
	}
	pixel.ptr = pixel.ptr + bitmap_pitch * lines + pixel_bytes(pixel.format) * diff;
	return *this;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator+=(glm::ivec2 diff)
{
	return *this += (diff.y * bitmap_size.x + diff.x);
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
	return bitmap_pos;
}

tr::bitmap::pixel_ref::pixel_ref(std::byte* ptr, pixel_format format)
	: ptr{ptr}, format{format}
{
}

tr::bitmap::pixel_ref::operator tr::rgba8() const
{
	return pixel_color(ptr, format);
}

tr::bitmap::pixel_ref& tr::bitmap::pixel_ref::operator=(rgba8 color)
{
	const SDL_PixelFormatDetails* format_details{SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(format))};
	const std::uint32_t formatted{SDL_MapRGBA(format_details, nullptr, color.r, color.g, color.b, color.a)};
	switch (pixel_bytes(format)) {
	case 1:
		*reinterpret_cast<std::uint8_t*>(ptr) = static_cast<std::uint8_t>(formatted);
		break;
	case 2:
		*reinterpret_cast<std::uint16_t*>(ptr) = static_cast<std::uint16_t>(formatted);
		break;
	case 3: {
		std::uint8_t (&arr)[3]{*reinterpret_cast<std::uint8_t (*)[3]>(ptr)};
		arr[0] = static_cast<std::uint8_t>(formatted >> 16);
		arr[1] = static_cast<std::uint8_t>(formatted >> 8);
		arr[2] = static_cast<std::uint8_t>(formatted);
		break;
	}
	case 4:
		*reinterpret_cast<std::uint32_t*>(ptr) = formatted;
	}
	return *this;
}

tr::bitmap::mut_it::mut_it(class bitmap& bitmap, glm::ivec2 pos)
	: pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, bitmap{&bitmap}
	, bitmap_pos{pos}
{
}

std::partial_ordering tr::bitmap::mut_it::operator<=>(const tr::bitmap::mut_it& r) const
{
	if (pixel.ptr != nullptr && r.pixel.ptr != nullptr) {
		return pixel.ptr <=> r.pixel.ptr;
	}
	else if (pixel.ptr == nullptr && r.pixel.ptr == nullptr) {
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
	TR_ASSERT(pixel.ptr != nullptr && irect2{bitmap->size()}.contains(bitmap_pos), "Tried to dereference an invalid bitmap iterator.");

	return pixel;
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
	return &pixel;
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
	TR_ASSERT(pixel.ptr != nullptr, "Tried to add to default-constructed bitmap iterator.");

	const glm::ivec2 bitmap_size{bitmap->size()};
	int lines{diff / bitmap_size.x};
	diff %= bitmap_size.x;
	if (diff + bitmap_pos.x >= bitmap_size.x) {
		++lines;
		diff -= bitmap_size.x;
	}
	else if (diff + bitmap_pos.x < 0) {
		--lines;
		diff += bitmap_size.x;
	}
	pixel.ptr = pixel.ptr + bitmap->pitch() * lines + pixel_bytes(pixel.format) * diff;
	bitmap_pos += glm::ivec2{diff, lines};
	return *this;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator+=(glm::ivec2 diff)
{
	return *this += (diff.y * bitmap->size().x + diff.x);
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
	TR_ASSERT(l.bitmap == r.bitmap && l.bitmap != nullptr, "Tried to subtract iterators to different bitmaps.");

	return (l.bitmap_pos.y * l.bitmap->size().x + l.bitmap_pos.x) - (r.bitmap_pos.y * r.bitmap->size().x + r.bitmap_pos.x);
}

glm::ivec2 tr::bitmap::mut_it::pos() const
{
	return bitmap_pos;
}