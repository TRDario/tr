/// @file
/// @brief Implements mutable_texture_view.hpp.

#include <tr/sysgfx/mutable_texture_view.hpp>
#include <tr/sysgfx/texture_view.hpp>

//

tr::mutable_texture_view::mutable_texture_view(unsigned int id, pixel_format format) noexcept
	: m_id{id}
	, m_format{format}
{
}

//

tr::mutable_texture_view::operator texture_view() const noexcept
{
	return texture_view{m_id};
}

//

bool tr::mutable_texture_view::empty() const noexcept
{
	return m_id == 0;
}

tr::pixel_format tr::mutable_texture_view::format() const noexcept
{
	return m_format;
}

//

unsigned int tr::mutable_texture_view::unwrap() const noexcept
{
	return m_id;
}