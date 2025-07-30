#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/atlas.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

tr::texture_unit::texture_unit()
{
	TR_ASSERT(std::ranges::find(texture_units, false) != texture_units.end(), "Tried to create more than 80 simultaneous texture units.");

	auto it{std::ranges::find(texture_units, false)};
	*it = true;
	texure_unit_textures[it - texture_units.begin()] = NO_TEXTURE;
	unit.reset(static_cast<unsigned int>(it - texture_units.begin()));
}

void tr::texture_unit::deleter::operator()(unsigned int id)
{
	texture_units[id] = false;
}

void tr::texture_unit::set_texture(texture_ref texture)
{
	if (texture.valid()) {
		if (texure_unit_textures[unit.get()] != texture) {
			TR_GL_CALL(glBindTextures, unit.get(), 1, &texture.id);
			texure_unit_textures[unit.get()] = texture;
		}
	}
	else {
		texure_unit_textures[unit.get()] = NO_TEXTURE;
	}
}

void tr::texture_unit::set_texture(const dyn_atlas& atlas)
{
	set_texture(static_cast<const texture&>(atlas));
}