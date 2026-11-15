/// @file
/// @brief Implements graphics_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::graphics_buffer::graphics_buffer(graphics_context& context, std::string_view label) noexcept
	: m_handle{gl_buffer_object_deleter{context}}
{
	context.gl().create_buffers(1, out_handle{m_handle});
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context.registered_buffers().emplace(id());
#endif
	if (!label.empty()) {
		set_label(label);
	}
}

void tr::graphics_buffer::gl_buffer_object_deleter::operator()(unsigned int handle) const noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context->registered_buffers().erase(id.get());
#endif
	context->gl().delete_buffers(1, &handle);
}

//

tr::graphics_buffer::operator bool() const noexcept
{
	return static_cast<bool>(m_handle);
}

bool tr::graphics_buffer::valid() const noexcept
{
	return static_cast<bool>(m_handle);
}

tr::graphics_context& tr::graphics_buffer::context() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the context of an invalid graphics buffer.");

	return m_handle.get_deleter().context;
}

bool tr::graphics_buffer::mapped() const noexcept
{
	TR_ASSERT(valid(), "Tried to check whether an invalid graphics buffer is mapped.");

	int mapped;
	context().gl().get_buffer_parameter_iv(m_handle.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

std::string tr::graphics_buffer::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of an invalid graphics buffer.");

	const internal::opengl& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_BUFFER, m_handle.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_BUFFER, m_handle.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

unsigned int tr::graphics_buffer::handle() const noexcept
{
	return m_handle.get();
}

tr::internal::graphics_object_id tr::graphics_buffer::id() const noexcept
{
	return m_handle.get_deleter().id.get();
}

//

void tr::graphics_buffer::set_label(std::string_view label) noexcept
{
	TR_ASSERT(valid(), "Tried to set the label of an invalid graphics buffer.");

	context().gl().set_object_label(GL_BUFFER, m_handle.get(), static_cast<int>(label.size()), label.data());
}

void tr::graphics_buffer::replace_with(graphics_buffer&& buffer)
{
	TR_ASSERT(buffer, "Tried to replace a graphics buffer with an invalid graphics buffer.");

	if (valid()) {
		const internal::opengl& gl{context().gl()};
		int label_length;
		gl.get_object_label(GL_BUFFER, m_handle.get(), 0, &label_length, nullptr);
		if (label_length > 0) {
			std::string label(label_length, '\0');
			gl.get_object_label(GL_BUFFER, m_handle.get(), label_length, &label_length, label.data());
			gl.set_object_label(GL_BUFFER, buffer.m_handle.get(), static_cast<int>(label.size()), label.data());
			gl.set_object_label(GL_BUFFER, m_handle.get(), 0, nullptr);
		}
	}
	*this = std::move(buffer);
}