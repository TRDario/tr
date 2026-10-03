/// @file
/// @brief Implements internal/image_unit.hpp.

#include "internal/opengl_definitions.hpp"
#include "internal/opengl_texture_format.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/internal/image_unit.hpp>
#include <tr/sysgfx/mutable_texture_view.hpp>

//

tr::internal::image_unit::image_unit(graphics_context& context) noexcept
	: m_handle{context.allocate_image_unit(), deleter{context}}
{
}

void tr::internal::image_unit::deleter::operator()(unsigned int id) const noexcept
{
	context->free_texture_unit(id);
}

//

unsigned int tr::internal::image_unit::id() const noexcept
{
	return m_handle.get();
}

//

void tr::internal::image_unit::set(mutable_texture_view texture, access access) noexcept
{
	const unsigned int gl_access{std::to_underlying(access) + GL_READ_ONLY};
	m_handle.get_deleter().context->gl().bind_image_texture(m_handle.get(), texture.unwrap(), 0, false, 0, gl_access,
															internal::opengl_image_format(texture.format()));
}