#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

tr::gfx::texture_unit::texture_unit()
{
	TR_ASSERT(std::ranges::find(texture_units, false) != texture_units.end(), "Tried to create more than 80 simultaneous texture units.");

	auto it{std::ranges::find(texture_units, false)};
	*it = true;
	texture_unit_textures[it - texture_units.begin()] = NO_TEXTURE;
	m_unit.reset(static_cast<unsigned int>(it - texture_units.begin()));
}

void tr::gfx::texture_unit::deleter::operator()(unsigned int id)
{
	texture_units[id] = false;
}

void tr::gfx::texture_unit::set_texture(texture_ref texture)
{
	if (texture.valid()) {
		if (texture_unit_textures[m_unit.get()] != texture) {
			TR_GL_CALL(glBindTextures, m_unit.get(), 1, &texture.m_id);
			texture_unit_textures[m_unit.get()] = texture;
		}
	}
	else {
		texture_unit_textures[m_unit.get()] = NO_TEXTURE;
	}
}

void tr::gfx::replace_texture_bindings(texture_ref old, texture_ref fresh)
{
	for (auto& unit : texture_unit_textures) {
		if (unit == old) {
			unit = fresh;
		}
	}
}