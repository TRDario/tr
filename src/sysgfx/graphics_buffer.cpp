/// @file
/// @brief Implements graphics_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::graphics_buffer::graphics_buffer(graphics_context& context) noexcept
	: m_handle{deleter{context}}
{
	context.gl().create_buffers(1, out_handle(m_handle));
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context.registry().buffers.emplace(id());
#endif
}

void tr::graphics_buffer::deleter::operator()(unsigned int bo) const noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context->registry().buffers.erase(id);
#endif
	context->gl().delete_buffers(1, &bo);
}

//

tr::graphics_context& tr::graphics_buffer::context() const noexcept
{
	return m_handle.get_deleter().context;
}

//

bool tr::graphics_buffer::valid() const noexcept
{
	return m_handle.has_value();
}

//

std::string tr::graphics_buffer::label() const
{
	const internal::opengl& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_BUFFER, unwrap(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_BUFFER, unwrap(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

void tr::graphics_buffer::set_label(std::string_view label) noexcept
{
	context().gl().set_object_label(GL_BUFFER, unwrap(), label.size(), label.data());
}

//

unsigned int tr::graphics_buffer::unwrap() const noexcept
{
	return m_handle.get();
}

//

void tr::graphics_buffer::reallocate()
{
	const graphics_buffer old_buffer{std::exchange(*this, graphics_buffer{context()})};
	context().move_label(GL_BUFFER, old_buffer.unwrap(), unwrap());
}

#ifdef TR_ENABLE_CHECKED_GRAPHICS
tr::internal::graphics_object_id tr::graphics_buffer::id() const noexcept
{
	return m_handle.get_deleter().id;
}
#endif