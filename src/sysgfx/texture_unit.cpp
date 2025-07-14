#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/atlas.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

tr::texture_unit::texture_unit() noexcept
{
	TR_ASSERT(std::ranges::find(_tex_units, false) != _tex_units.end(), "Tried to create more than 80 simultaneous texture units.");

	auto it{std::ranges::find(_tex_units, false)};
	*it = true;
	_tex_unit_textures[it - _tex_units.begin()] = NO_TEXTURE;
	_id.reset(static_cast<unsigned int>(it - _tex_units.begin()));
}

void tr::texture_unit::deleter::operator()(unsigned int id) noexcept
{
	_tex_units[id] = false;
}

void tr::texture_unit::set_texture(texture_ref texture) noexcept
{
	if (texture.valid()) {
		if (_tex_unit_textures[_id.get()] != texture) {
			TR_GL_CALL(glBindTextures, _id.get(), 1, &texture._id);
			_tex_unit_textures[_id.get()] = texture;
		}
	}
	else {
		_tex_unit_textures[_id.get()] = NO_TEXTURE;
	}
}

void tr::texture_unit::set_texture(const dyn_atlas& atlas) noexcept
{
	set_texture(static_cast<const texture&>(atlas));
}