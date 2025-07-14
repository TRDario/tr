#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace tr {
	// Extracts an RGBA8 color value from a pixel.
	rgba8 _pixel_color(const std::byte* data, pixel_format format);
} // namespace tr

tr::rgba8 tr::_pixel_color(const std::byte* data, pixel_format format)
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

tr::sub_bitmap::pixel_ref::pixel_ref(const std::byte* ptr, pixel_format format) noexcept
	: _impl{ptr}, _format{format}
{
}

tr::sub_bitmap::pixel_ref::operator tr::rgba8() const noexcept
{
	return _pixel_color(_impl, _format);
}

tr::sub_bitmap::iterator::iterator(const sub_bitmap& bitmap, glm::ivec2 pos) noexcept
	: _pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, _bitmap_size{bitmap.size()}
	, _pitch{bitmap.pitch()}
	, _pos{pos}
{
}

std::partial_ordering tr::sub_bitmap::iterator::operator<=>(const iterator& r) const noexcept
{
	if (_pixel._impl != nullptr && r._pixel._impl != nullptr) {
		return _pixel._impl <=> r._pixel._impl;
	}
	else if (_pixel._impl == nullptr && r._pixel._impl == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::sub_bitmap::iterator::operator==(const iterator& r) const noexcept
{
	return *this <=> r == std::strong_ordering::equal;
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator*() const noexcept
{
	TR_ASSERT(_pixel._impl != nullptr && irect2{_bitmap_size}.contains(_pos), "Tried to dereference an invalid bitmap iterator.");
	return _pixel;
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator[](int diff) const noexcept
{
	return *(*this + diff);
}

tr::sub_bitmap::iterator::value_type tr::sub_bitmap::iterator::operator[](glm::ivec2 diff) const noexcept
{
	return *(*this + diff);
}

tr::sub_bitmap::iterator::pointer tr::sub_bitmap::iterator::operator->() const noexcept
{
	return &_pixel;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator++() noexcept
{
	return *this += 1;
}

tr::sub_bitmap::iterator tr::sub_bitmap::iterator::operator++(int) noexcept
{
	iterator copy{*this};
	++*this;
	return copy;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator+=(int diff) noexcept
{
	int lines{diff / _bitmap_size.x};
	diff %= _bitmap_size.x;
	if (diff + _pos.x >= _bitmap_size.x) {
		++lines;
		diff -= _bitmap_size.x;
	}
	else if (diff + _pos.x < 0) {
		--lines;
		diff += _bitmap_size.x;
	}
	_pixel._impl = _pixel._impl + _pitch * lines + pixel_bytes(_pixel._format) * diff;
	return *this;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * _bitmap_size.x + diff.x);
}

tr::sub_bitmap::iterator tr::operator+(const tr::sub_bitmap::iterator& it, int diff) noexcept
{
	tr::sub_bitmap::iterator copy{it};
	copy += diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator+(int diff, const tr::sub_bitmap::iterator& it) noexcept
{
	return it + diff;
}

tr::sub_bitmap::iterator tr::operator+(const tr::sub_bitmap::iterator& it, glm::ivec2 diff) noexcept
{
	tr::sub_bitmap::iterator copy{it};
	copy += diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator+(glm::ivec2 diff, const tr::sub_bitmap::iterator& it) noexcept
{
	return it + diff;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator--() noexcept
{
	return *this -= 1;
}

tr::sub_bitmap::iterator tr::sub_bitmap::iterator::operator--(int) noexcept
{
	iterator copy{*this};
	--*this;
	return copy;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator-=(int diff) noexcept
{
	return *this += -diff;
}

tr::sub_bitmap::iterator& tr::sub_bitmap::iterator::operator-=(glm::ivec2 diff) noexcept
{
	return *this += -diff;
}

tr::sub_bitmap::iterator tr::operator-(const tr::sub_bitmap::iterator& it, int diff) noexcept
{
	tr::sub_bitmap::iterator copy{it};
	copy -= diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator-(int diff, const tr::sub_bitmap::iterator& it) noexcept
{
	return it - diff;
}

tr::sub_bitmap::iterator tr::operator-(const tr::sub_bitmap::iterator& it, glm::ivec2 diff) noexcept
{
	tr::sub_bitmap::iterator copy{it};
	copy -= diff;
	return copy;
}

tr::sub_bitmap::iterator tr::operator-(glm::ivec2 diff, const tr::sub_bitmap::iterator& it) noexcept
{
	return it - diff;
}

glm::ivec2 tr::sub_bitmap::iterator::pos() const noexcept
{
	return _pos;
}

tr::bitmap::pixel_ref::pixel_ref(std::byte* ptr, pixel_format format) noexcept
	: _impl{ptr}, _format{format}
{
}

tr::bitmap::pixel_ref::operator tr::rgba8() const noexcept
{
	return _pixel_color(_impl, _format);
}

tr::bitmap::pixel_ref& tr::bitmap::pixel_ref::operator=(rgba8 color) noexcept
{
	const std::uint32_t formatted{
		SDL_MapRGBA(SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(_format)), nullptr, color.r, color.g, color.b, color.a)};
	switch (pixel_bytes(_format)) {
	case 1:
		*reinterpret_cast<std::uint8_t*>(_impl) = static_cast<std::uint8_t>(formatted);
		break;
	case 2:
		*reinterpret_cast<std::uint16_t*>(_impl) = static_cast<std::uint16_t>(formatted);
		break;
	case 3: {
		std::uint8_t (&arr)[3]{*reinterpret_cast<std::uint8_t (*)[3]>(_impl)};
		arr[0] = static_cast<std::uint8_t>(formatted >> 16);
		arr[1] = static_cast<std::uint8_t>(formatted >> 8);
		arr[2] = static_cast<std::uint8_t>(formatted);
		break;
	}
	case 4:
		*reinterpret_cast<std::uint32_t*>(_impl) = formatted;
	}
	return *this;
}

tr::bitmap::mut_it::mut_it(bitmap& bitmap, glm::ivec2 pos) noexcept
	: _pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}, _bitmap{&bitmap}, _pos{pos}
{
}

std::partial_ordering tr::bitmap::mut_it::operator<=>(const tr::bitmap::mut_it& r) const noexcept
{
	if (_pixel._impl != nullptr && r._pixel._impl != nullptr) {
		return _pixel._impl <=> r._pixel._impl;
	}
	else if (_pixel._impl == nullptr && r._pixel._impl == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::bitmap::mut_it::operator==(const mut_it& r) const noexcept
{
	return *this <=> r == std::strong_ordering::equal;
}

tr::bitmap::mut_it::value_type tr::bitmap::mut_it::operator*() const noexcept
{
	TR_ASSERT(_pixel._impl != nullptr && irect2{_bitmap->size()}.contains(_pos), "Tried to dereference an invalid bitmap iterator.");

	return _pixel;
}

tr::bitmap::mut_it::value_type tr::bitmap::mut_it::operator[](int diff) const noexcept
{
	return *(*this + diff);
}

tr::bitmap::mut_it::value_type tr::bitmap::mut_it::operator[](glm::ivec2 diff) const noexcept
{
	return *(*this + diff);
}

tr::bitmap::mut_it::pointer tr::bitmap::mut_it::operator->() const noexcept
{
	return &_pixel;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator++() noexcept
{
	return *this += 1;
}

tr::bitmap::mut_it tr::bitmap::mut_it::operator++(int) noexcept
{
	mut_it copy{*this};
	++*this;
	return copy;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator+=(int diff) noexcept
{
	TR_ASSERT(_pixel._impl != nullptr, "Tried to add to default-constructed bitmap iterator.");

	const glm::ivec2 bitmap_size{_bitmap->size()};
	int lines{diff / bitmap_size.x};
	diff %= bitmap_size.x;
	if (diff + _pos.x >= bitmap_size.x) {
		++lines;
		diff -= bitmap_size.x;
	}
	else if (diff + _pos.x < 0) {
		--lines;
		diff += bitmap_size.x;
	}
	_pixel._impl = _pixel._impl + _bitmap->pitch() * lines + pixel_bytes(_pixel._format) * diff;
	_pos += glm::ivec2{diff, lines};
	return *this;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * _bitmap->size().x + diff.x);
}

tr::bitmap::mut_it tr::operator+(const tr::bitmap::mut_it& it, int diff) noexcept
{
	tr::bitmap::mut_it copy{it};
	copy += diff;
	return copy;
}

tr::bitmap::mut_it tr::operator+(int diff, const tr::bitmap::mut_it& it) noexcept
{
	return it + diff;
}

tr::bitmap::mut_it tr::operator+(const tr::bitmap::mut_it& it, glm::ivec2 diff) noexcept
{
	tr::bitmap::mut_it copy{it};
	copy += diff;
	return copy;
}

tr::bitmap::mut_it tr::operator+(glm::ivec2 diff, const tr::bitmap::mut_it& it) noexcept
{
	return it + diff;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator--() noexcept
{
	return *this -= 1;
}

tr::bitmap::mut_it tr::bitmap::mut_it::operator--(int) noexcept
{
	mut_it copy{*this};
	--*this;
	return copy;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator-=(int diff) noexcept
{
	return *this += -diff;
}

tr::bitmap::mut_it& tr::bitmap::mut_it::operator-=(glm::ivec2 diff) noexcept
{
	return *this += -diff;
}

tr::bitmap::mut_it tr::operator-(const tr::bitmap::mut_it& it, int diff) noexcept
{
	tr::bitmap::mut_it copy{it};
	copy -= diff;
	return copy;
}

tr::bitmap::mut_it tr::operator-(int diff, const tr::bitmap::mut_it& it) noexcept
{
	return it - diff;
}

tr::bitmap::mut_it tr::operator-(const tr::bitmap::mut_it& it, glm::ivec2 diff) noexcept
{
	tr::bitmap::mut_it copy{it};
	copy -= diff;
	return copy;
}

tr::bitmap::mut_it tr::operator-(glm::ivec2 diff, const tr::bitmap::mut_it& it) noexcept
{
	return it - diff;
}

int tr::operator-(const tr::bitmap::mut_it& l, const tr::bitmap::mut_it& r) noexcept
{
	TR_ASSERT(l._bitmap == r._bitmap && l._bitmap != nullptr, "Tried to subtract iterators to different bitmaps.");

	return (l._pos.y * l._bitmap->size().x + l._pos.x) - (r._pos.y * r._bitmap->size().x + r._pos.x);
}

glm::ivec2 tr::bitmap::mut_it::pos() const noexcept
{
	return _pos;
}