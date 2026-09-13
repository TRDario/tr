/// File
/// @brief Implements static_index_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/static_index_buffer.hpp>

//

tr::static_index_buffer::static_index_buffer(graphics_context& context, std::span<const u16> data)
	: graphics_buffer{context}
	, m_size{std::ssize(data)}
{
	const internal::opengl& gl{context.gl()};
	gl.allocate_buffer_storage(unwrap(), m_size * sizeof(u16), data.data(), 0);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"index buffer allocation"};
	}
}