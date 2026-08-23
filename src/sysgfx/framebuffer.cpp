/// @file
/// @brief Implements framebuffer.hpp.

#include "../../include/tr/sysgfx/framebuffer.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

//

tr::framebuffer::framebuffer(graphics_context& context)
	: m_handle{deleter{context}}
{
	context.gl().create_framebuffers(1, out_handle(m_handle));
}

void tr::framebuffer::deleter::operator()(unsigned int fbo) const
{
	context->gl().delete_framebuffers(1, &fbo);
}

//

tr::graphics_context& tr::framebuffer::context() const
{
	TR_ASSERT(valid(), "Tried to get context of a framebuffer in an invalid state.");

	return m_handle.get_deleter().context;
}

//

tr::render_target tr::framebuffer::render_target(glm::ivec2 size) const
{
	TR_ASSERT(valid(), "Tried to get render target of a framebuffer in an invalid state.");

	return tr::render_target{gid(), size};
}

//

void tr::framebuffer::attach(attachment attachment, texture_view texture)
{
	TR_ASSERT(valid(), "Tried to attach to a framebuffer in an invalid state.");

	context().gl().set_framebuffer_texture(gid(), std::to_underlying(attachment), texture.m_id, 0);
}

void tr::framebuffer::detach(attachment attachment)
{
	TR_ASSERT(valid(), "Tried to detach from a framebuffer in an invalid state.");

	context().gl().set_framebuffer_texture(gid(), std::to_underlying(attachment), 0, 0);
}

//

bool tr::framebuffer::valid() const
{
	return m_handle.has_value();
}

//

void tr::framebuffer::set_label(std::string_view label)
{
	TR_ASSERT(valid(), "Tried to set the label of a framebuffer in an invalid state");

	context().gl().set_object_label(GL_FRAMEBUFFER, gid(), label.size(), label.data());
}

std::string tr::framebuffer::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a framebuffer in an invalid state");

	const gl_api& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_FRAMEBUFFER, gid(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_FRAMEBUFFER, gid(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

//

unsigned int tr::framebuffer::gid() const
{
	return m_handle.get();
}