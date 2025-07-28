#include "../../include/tr/utility/atlas_packer.hpp"

tr::atlas_packer::atlas_packer(glm::u16vec2 size);

glm::u16vec2 tr::atlas_packer::size() const
{
	return _size;
}

void tr::atlas_packer::clear();

void tr::atlas_packer::resize(glm::u16vec2 size);

std::optional<glm::u16vec2> tr::atlas_packer::try_insert(glm::u16vec2 size);