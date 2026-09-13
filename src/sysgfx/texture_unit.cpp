/// @file
/// @brief Implements internal/texture_unit.hpp.

#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/internal/texture_unit.hpp>
#include <tr/sysgfx/texture_view.hpp>

//

tr::internal::texture_unit::texture_unit(graphics_context& context) noexcept
	: m_handle{context.allocate_texture_unit(), deleter{context}}
{
}

void tr::internal::texture_unit::deleter::operator()(unsigned int id) const noexcept
{
	context->free_texture_unit(id);
}

//

unsigned int tr::internal::texture_unit::id() const noexcept
{
	return m_handle.get();
}

//

void tr::internal::texture_unit::set(texture_view texture) noexcept
{
	const unsigned int texture_id{texture.unwrap()};
	m_handle.get_deleter().context->gl().bind_textures(m_handle.get(), 1, &texture_id);
}