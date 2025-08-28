#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

tr::gfx::texture_unit::texture_unit()
{
	TR_ASSERT(std::ranges::find(texture_units, false) != texture_units.end(), "Tried to create more than 80 simultaneous texture units.");

	auto it{std::ranges::find(texture_units, false)};
	*it = true;
	texture_unit_textures[it - texture_units.begin()] = std::nullopt;
	m_unit.reset((unsigned int)(it - texture_units.begin()));
}

void tr::gfx::texture_unit::deleter::operator()(unsigned int id)
{
	texture_units[id] = false;
}

void tr::gfx::texture_unit::set_texture(texture_ref ref)
{
	if (!ref.empty()) {
		if (texture_unit_textures[m_unit.get()] != ref && ref.m_ref->m_handle != 0) {
			TR_GL_CALL(glBindTextures, m_unit.get(), 1, &ref.m_ref->m_handle);
		}
	}
	texture_unit_textures[m_unit.get()] = std::move(ref);
}