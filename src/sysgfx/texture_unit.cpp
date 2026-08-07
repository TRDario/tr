/// @file
/// @brief Implements texture_unit.hpp.

#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

//

tr::texture_unit::texture_unit(graphics_context& context)
	: m_handle{{context}}
{
	for (unsigned int free_index{0}; free_index < context.m_allocated_texture_units.size(); ++free_index) {
		if (!context.m_allocated_texture_units[free_index]) {
			context.m_allocated_texture_units[free_index] = true;
			m_handle.reset(free_index);
			return;
		}
	}
	TR_ASSERT(m_handle.has_value(), "Tried to allocate more than 80 texture units simultaneously.");
}

void tr::texture_unit::deleter::operator()(unsigned int id) const
{
	context.m_allocated_texture_units[id] = false;
}

//

unsigned int tr::texture_unit::id() const
{
	return m_handle.get();
}

//

void tr::texture_unit::set(texture_view texture)
{
	const gl_api& gl{m_handle.get_deleter().context.make_current_and_return_gl_api()};
	gl.bind_textures(m_handle.get(), 1, &texture.m_id);
}