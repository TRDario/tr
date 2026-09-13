/// @file
/// @brief Implements untyped_static_vertex_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/untyped_static_vertex_buffer.hpp>

//

tr::untyped_static_vertex_buffer::untyped_static_vertex_buffer(graphics_context& context, std::span<const std::byte> data)
	: graphics_buffer{context}
	, m_size{std::ssize(data)}
{
	const internal::opengl& gl{context.gl()};
	gl.allocate_buffer_storage(unwrap(), m_size, data.data(), 0);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"vertex buffer allocation"};
	}
}