/// @file
/// @brief Implements framebuffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/framebuffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/texture_view.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::framebuffer::framebuffer(graphics_context& context, std::string_view label) noexcept
	: m_handle{gl_framebuffer_object_deleter{context}}
{
	context.gl().create_framebuffers(1, out_handle{m_handle});
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context.registered_framebuffers().emplace(id());
#endif
	if (!label.empty()) {
		set_label(label);
	}
}

void tr::framebuffer::gl_framebuffer_object_deleter::operator()(unsigned int handle) const noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context->registered_framebuffers().erase(id.get());
#endif
	context->gl().delete_framebuffers(1, &handle);
}

//

void tr::framebuffer::attach(attachment attachment, texture_view texture) noexcept
{
	TR_ASSERT(valid(), "Tried to attach to a framebuffer in an invalid state.");

	context().gl().set_framebuffer_texture(m_handle.get(), std::to_underlying(attachment), texture.unwrap(), 0);
}

void tr::framebuffer::detach(attachment attachment) noexcept
{
	TR_ASSERT(valid(), "Tried to detach from a framebuffer in an invalid state.");

	context().gl().set_framebuffer_texture(m_handle.get(), std::to_underlying(attachment), 0, 0);
}

void tr::framebuffer::set_label(std::string_view label) noexcept
{
	TR_ASSERT(valid(), "Tried to set the label of a framebuffer in an invalid state");

	context().gl().set_object_label(GL_FRAMEBUFFER, m_handle.get(), static_cast<int>(label.size()), label.data());
}

//

tr::framebuffer::operator bool() const noexcept
{
	return static_cast<bool>(m_handle);
}

bool tr::framebuffer::valid() const noexcept
{
	return static_cast<bool>(m_handle);
}

tr::graphics_context& tr::framebuffer::context() const noexcept
{
	TR_ASSERT(valid(), "Tried to get context of a framebuffer in an invalid state.");

	return m_handle.get_deleter().context;
}

std::string tr::framebuffer::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a framebuffer in an invalid state");

	const internal::opengl& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_FRAMEBUFFER, m_handle.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_FRAMEBUFFER, m_handle.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

unsigned int tr::framebuffer::handle() const noexcept
{
	return m_handle.get();
}

tr::internal::graphics_object_id tr::framebuffer::id() const noexcept
{
	return m_handle.get_deleter().id.get();
}