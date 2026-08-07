/// @file
/// @brief Implements texture_view.hpp.

#include "../../include/tr/sysgfx/texture_view.hpp"

//

tr::texture_view::texture_view(unsigned int id)
	: m_id{id}
{
}

//

bool tr::texture_view::empty() const
{
	return m_id == 0;
}