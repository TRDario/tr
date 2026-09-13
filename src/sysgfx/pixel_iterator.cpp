/// @file
/// @brief Implements bitmap_iterators.hpp.

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <tr/sysgfx/bitmap.hpp>
#include <tr/sysgfx/pixel_iterator.hpp>
#include <tr/sysgfx/sub_bitmap.hpp>
#include <tr/utility/macro.hpp>

//

tr::const_pixel_iterator::const_pixel_iterator(sub_bitmap bitmap, glm::ivec2 pos) noexcept
	: m_pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, m_bitmap_size{bitmap.size()}
	, m_bitmap_pitch{bitmap.pitch()}
	, m_bitmap_pos{pos}
{
}

std::partial_ordering tr::const_pixel_iterator::operator<=>(const const_pixel_iterator& rhs) const noexcept
{
	if (m_pixel.data() != nullptr && rhs.m_pixel.data() != nullptr) {
		return m_pixel.data() <=> rhs.m_pixel.data();
	}
	else if (m_pixel.data() == nullptr && rhs.m_pixel.data() == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::const_pixel_iterator::operator==(const const_pixel_iterator& rhs) const noexcept
{
	return *this <=> rhs == std::strong_ordering::equal;
}

tr::const_pixel_iterator::value_type tr::const_pixel_iterator::operator*() const noexcept
{
	TR_ASSERT(m_pixel.data() != nullptr && rectangle<int>{m_bitmap_size}.contains(m_bitmap_pos),
			  "Tried to dereference an invalid bitmap iterator.");
	return m_pixel;
}

tr::const_pixel_iterator::pointer tr::const_pixel_iterator::operator->() const noexcept
{
	return &m_pixel;
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator++() noexcept
{
	return *this += 1;
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator+=(int diff) noexcept
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
	m_pixel = const_pixel_proxy{m_pixel.data() + m_bitmap_pitch * lines + pixel_bytes(m_pixel.format()) * diff, m_pixel.format()};
	return *this;
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * m_bitmap_size.x + diff.x);
}

tr::const_pixel_iterator& tr::const_pixel_iterator::operator--() noexcept
{
	return *this -= 1;
}

int tr::operator-(const const_pixel_iterator& lhs, const const_pixel_iterator& rhs) noexcept
{
	return (lhs.m_bitmap_pos.y * lhs.m_bitmap_size.x + lhs.m_bitmap_pos.x) -
		   (rhs.m_bitmap_pos.y * rhs.m_bitmap_size.x + rhs.m_bitmap_pos.x);
}

glm::ivec2 tr::const_pixel_iterator::pos() const noexcept
{
	return m_bitmap_pos;
}

//

tr::pixel_iterator::pixel_iterator(bitmap& bitmap, glm::ivec2 pos) noexcept
	: m_pixel{bitmap.data() + bitmap.pitch() * pos.y + pixel_bytes(bitmap.format()) * pos.x, bitmap.format()}
	, m_bitmap{bitmap}
	, m_bitmap_pos{pos}
{
}

std::partial_ordering tr::pixel_iterator::operator<=>(const pixel_iterator& rhs) const noexcept
{
	if (m_pixel.data() != nullptr && rhs.m_pixel.data() != nullptr) {
		return m_pixel.data() <=> rhs.m_pixel.data();
	}
	else if (m_pixel.data() == nullptr && rhs.m_pixel.data() == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::pixel_iterator::operator==(const pixel_iterator& rhs) const noexcept
{
	return *this <=> rhs == std::strong_ordering::equal;
}

tr::pixel_iterator::value_type tr::pixel_iterator::operator*() const noexcept
{
	TR_ASSERT(m_pixel.data() != nullptr && rectangle<int>{m_bitmap->size()}.contains(m_bitmap_pos),
			  "Tried to dereference an invalid bitmap iterator.");

	return m_pixel;
}

tr::pixel_iterator::pointer tr::pixel_iterator::operator->() const noexcept
{
	return &m_pixel;
}

tr::pixel_iterator& tr::pixel_iterator::operator++() noexcept
{
	return *this += 1;
}

tr::pixel_iterator& tr::pixel_iterator::operator+=(int diff) noexcept
{
	TR_ASSERT(m_pixel.data() != nullptr, "Tried to add to default-constructed bitmap iterator.");

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
	m_pixel = pixel_proxy{m_pixel.data() + m_bitmap->pitch() * lines + pixel_bytes(m_pixel.format()) * diff, m_pixel.format()};
	m_bitmap_pos += glm::ivec2{diff, lines};
	return *this;
}

tr::pixel_iterator& tr::pixel_iterator::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * m_bitmap->size().x + diff.x);
}

tr::pixel_iterator& tr::pixel_iterator::operator--() noexcept
{
	return *this -= 1;
}

int tr::operator-(const pixel_iterator& lhs, const pixel_iterator& rhs) noexcept
{
	TR_ASSERT(lhs.m_bitmap == rhs.m_bitmap, "Tried to subtract iterators to different bitmaps.");

	return (lhs.m_bitmap_pos.y * lhs.m_bitmap->size().x + lhs.m_bitmap_pos.x) -
		   (rhs.m_bitmap_pos.y * rhs.m_bitmap->size().x + rhs.m_bitmap_pos.x);
}

glm::ivec2 tr::pixel_iterator::pos() const noexcept
{
	return m_bitmap_pos;
}