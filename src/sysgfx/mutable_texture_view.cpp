/// @file
/// @brief Implements mutable_texture_view.hpp.

#include <tr/sysgfx/mutable_texture_view.hpp>
#include <tr/sysgfx/texture_view.hpp>
#include <tr/utility/macro.hpp>

//

tr::mutable_texture_view::mutable_texture_view(unsigned int handle, pixel_format format) noexcept
	: m_handle{handle}
	, m_format{format}
{
}

//

tr::texture_view::operator bool() const noexcept
{
	return m_handle != 0;
}

bool tr::mutable_texture_view::empty() const noexcept
{
	return m_handle == 0;
}

tr::pixel_format tr::mutable_texture_view::format() const noexcept
{
	TR_ASSERT(!empty(), "Tried to get format of an empty texture view.");

	return m_format;
}

tr::mutable_texture_view::operator tr::texture_view() const noexcept
{
	return texture_view{m_handle};
}

unsigned int tr::mutable_texture_view::handle() const noexcept
{
	return m_handle;
}