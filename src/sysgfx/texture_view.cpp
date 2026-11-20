/// @file
/// @brief Implements texture_view.hpp.

#include <tr/sysgfx/texture_view.hpp>

//

tr::texture_view::texture_view(unsigned int handle) noexcept
	: m_handle{handle}
{
}

//

tr::texture_view::operator bool() const noexcept
{
	return m_handle != 0;
}

bool tr::texture_view::empty() const noexcept
{
	return m_handle == 0;
}

unsigned int tr::texture_view::handle() const noexcept
{
	return m_handle;
}