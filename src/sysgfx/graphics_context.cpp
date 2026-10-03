/// @file
/// @brief Implements graphics_context.hpp.

#include "internal/opengl_debug_callback.hpp"
#include "internal/opengl_definitions.hpp"
#include <SDL3/SDL.h>
#include <tr/sysgfx/blending.hpp>
#include <tr/sysgfx/dynamic_index_buffer.hpp>
#include <tr/sysgfx/exception.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/shader.hpp>
#include <tr/sysgfx/shader_pipeline.hpp>
#include <tr/sysgfx/static_index_buffer.hpp>
#include <tr/sysgfx/texture.hpp>
#include <tr/sysgfx/window_view.hpp>

//

namespace tr::internal
{
	namespace
	{
		/// Pointer to the current graphics context.
		thread_local SDL_GLContextState* current_graphics_context{nullptr};
	} // namespace
} // namespace tr::internal

//

tr::graphics_context::graphics_context(window_view window)
	: m_window{window.unwrap()}
	, m_ptr{SDL_GL_CreateContext(m_window)}
	, m_viewport{{}, window.size()}
	, m_scissor_box{{}, window.size()}
{
	if (m_ptr == nullptr) {
		throw graphics_context_init_error{};
	}
	internal::current_graphics_context = m_ptr.get();

	m_gl.enable(GL_BLEND);
	m_gl.enable(GL_SCISSOR_TEST);

	int context_flags;
	m_gl.get_integer_v(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		m_gl.enable(GL_DEBUG_OUTPUT);
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		m_gl.enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
		m_gl.set_debug_message_callback(internal::opengl_debug_callback, nullptr);
		m_gl.set_debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		m_gl.set_debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	}
}

void tr::graphics_context::deleter::operator()(SDL_GLContextState* context) const noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	TR_ASSERT(registered_framebuffers.empty(),
			  "Tried to destroy a graphics context while one or more framebuffers were still alive on it.");
	TR_ASSERT(registered_shaders.empty(), "Tried to destroy a graphics context while one or more shaders were still alive on it.");
	TR_ASSERT(registered_shader_pipelines.empty(),
			  "Tried to destroy a graphics context while one or more shader pipelines were still alive on it.");
	TR_ASSERT(registered_vertex_formats.empty(),
			  "Tried to destroy a graphics context while one or more vertex formats were still alive on it.");
	TR_ASSERT(registered_buffers.empty(), "Tried to destroy a graphics context while one or more buffers were still alive on it.");
#endif

	if (internal::current_graphics_context == context) {
		internal::current_graphics_context = nullptr;
	}
	SDL_GL_DestroyContext(context);
}

//

struct tr::graphics_context::info tr::graphics_context::info() const noexcept
{
	using get_string_t = const unsigned char* (*)(unsigned int);
	const get_string_t get_string{gl().get_string};
	return {
		reinterpret_cast<const char*>(get_string(GL_VENDOR)),
		reinterpret_cast<const char*>(get_string(GL_RENDERER)),
		reinterpret_cast<const char*>(get_string(GL_VERSION)),
	};
}

//

tr::window_view tr::graphics_context::window() const noexcept
{
	return window_view{m_window};
}

tr::render_target tr::graphics_context::backbuffer() const noexcept
{
	return render_target{*this};
}

//

bool tr::graphics_context::wireframe_mode_enabled() const noexcept
{
	return m_wireframe_mode_enabled;
}

void tr::graphics_context::set_wireframe_mode(bool enable) noexcept
{
	if (m_wireframe_mode_enabled != enable) {
		gl().set_polygon_mode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
		m_wireframe_mode_enabled = enable;
	}
}

//

bool tr::graphics_context::face_culling_enabled() const noexcept
{
	return m_face_culling_enabled;
}

void tr::graphics_context::set_face_culling(bool enable) noexcept
{
	if (m_face_culling_enabled != enable) {
		const internal::opengl& gl{this->gl()};
		enable ? gl.enable(GL_CULL_FACE) : gl.disable(GL_CULL_FACE);
	}
}

//

bool tr::graphics_context::depth_testing_enabled() const noexcept
{
	return m_depth_testing_enabled;
}

void tr::graphics_context::set_depth_testing(bool enable) noexcept
{
	if (m_depth_testing_enabled != enable) {
		const internal::opengl& gl{this->gl()};
		enable ? gl.enable(GL_DEPTH_TEST) : gl.disable(GL_DEPTH_TEST);
	}
}

//

void tr::graphics_context::set_render_target(const render_target& target) noexcept
{
	const internal::graphics_object_id framebuffer_id{target.framebuffer_id()};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	TR_ASSERT(&target.context() == this, "Tried to set render target to a context it is not associated with.");
	TR_ASSERT(framebuffer_id == internal::graphics_object_id::invalid || registered_framebuffers().contains(framebuffer_id),
			  "Tried to set render target on a framebuffer in an invalid state to a context.");
#endif

	const internal::opengl& gl{this->gl()};
	const rectangle<u16> viewport{target.viewport()};
	const int bottom{target.framebuffer_height() - viewport.tl.y - viewport.size.y};
	if (m_bound_framebuffer != framebuffer_id) {
#ifdef TR_ENABLE_LOG_TRACE
		int label_length;
		std::string framebuffer_label;
		if (target.framebuffer_fbo() != 0) {
			gl.get_object_label(GL_FRAMEBUFFER, target.framebuffer_fbo(), 0, &label_length, nullptr);
			if (label_length > 0) {
				framebuffer_label.resize(label_length, '\0');
				gl.get_object_label(GL_FRAMEBUFFER, target.framebuffer_fbo(), label_length + 1, nullptr, framebuffer_label.data());
			}
			else {
				framebuffer_label = "<unnamed>";
			}
		}
		else {
			framebuffer_label = std::format("<backbuffer of '{}'>", SDL_GetWindowTitle(m_window));
		}
		TR_LOG_TRACE("gfx", "Setting framebuffer \"{}\" (ID: {}, FBO: {}) on context at {}", framebuffer_label,
					 std::to_underlying(framebuffer_id), target.framebuffer_fbo(), static_cast<void*>(this));
#endif
		gl.bind_framebuffer(GL_DRAW_FRAMEBUFFER, target.framebuffer_fbo());
		m_bound_framebuffer = framebuffer_id;
	}
	if (m_viewport != viewport) {
		gl.set_viewport(viewport.tl.x, bottom, viewport.size.x, viewport.size.y);
		m_viewport = viewport;
	}
	if (const rectangle<u16> scissor_box{target.scissor_box()}; m_scissor_box != scissor_box) {
		gl.set_scissor(scissor_box.tl.x, bottom, scissor_box.size.x, scissor_box.size.y);
		m_scissor_box = scissor_box;
	}
}

void tr::graphics_context::set_shader_pipeline(const shader_pipeline& pipeline) noexcept
{
	TR_ASSERT(pipeline.valid(), "Tried to set a shader pipeline in an invalid state to a context.");
	TR_ASSERT(&pipeline.context() == this, "Tried to set shader pipeline {} to a context it is not associated with.", pipeline);
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	TR_ASSERT(registered_shaders().contains(pipeline.vertex_shader_id()), "Tried to set shader pipeline {} with invalid set vertex shader.",
			  pipeline);
	TR_ASSERT(registered_shaders().contains(pipeline.fragment_shader_id()),
			  "Tried to set shader pipeline {} with invalid set fragment shader.", pipeline);
#endif

	const internal::opengl& gl{this->gl()};
	if (m_bound_shader != internal::graphics_object_id::invalid) {
		gl.use_program(0);
	}
	if (internal::graphics_object_id id{pipeline.id()}; m_bound_shader_pipeline != id) {
		TR_LOG_TRACE("gfx", "Setting shader pipeline {} on context.", pipeline);
		gl.bind_program_pipeline(pipeline.unwrap());
		m_bound_shader_pipeline = id;
	}
}

void tr::graphics_context::dispatch_compute_shader(const compute_shader& shader, glm::uvec3 groups) noexcept
{
	TR_ASSERT(shader.valid(), "Tried to dispatch a compute shader in an invalid state to a context.");
	TR_ASSERT(&shader.context() == this, "Tried to dispatch shader {} om a context it is not associated with.", shader);

	const internal::opengl& gl{this->gl()};
	if (internal::graphics_object_id id{shader.id()}; m_bound_shader != id) {
		TR_LOG_TRACE("gfx", "Setting shader program {} on context.", shader);
		gl.use_program(shader.unwrap());
		m_bound_shader = id;
	}
	gl.dispatch_compute_shader(groups.x, groups.y, groups.z);
}

//

const tr::blend_mode& tr::graphics_context::blend_mode() const noexcept
{
	return m_blend_mode;
}

void tr::graphics_context::set_blend_mode(const tr::blend_mode& blend_mode) noexcept
{
	if (m_blend_mode != blend_mode) {
		const internal::opengl& gl{this->gl()};
		gl.set_separate_blend_equations(std::to_underlying(blend_mode.rgb_fn), std::to_underlying(blend_mode.alpha_fn));
		gl.set_separate_blend_function(std::to_underlying(blend_mode.rgb_src), std::to_underlying(blend_mode.rgb_dst),
									   std::to_underlying(blend_mode.alpha_src), std::to_underlying(blend_mode.alpha_dst));
	}
}

//

void tr::graphics_context::set_vertex_format(const vertex_format& format) noexcept
{
	TR_ASSERT(format.valid(), "Tried to set vertex format in an invalid state to a graphics context.");
	TR_ASSERT(&format.context() == this, "Tried to set vertex format {} to a context it is not associated with.", format);

	if (internal::graphics_object_id id{format.id()}; m_bound_vertex_format != id) {
		TR_LOG_TRACE("gfx", "Binding vertex format {} to context.", format);
		gl().bind_vertex_array(format.unwrap());
		m_bound_vertex_format = id;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		m_bound_vertex_format_bindings = format.bindings();
#endif
	}
}

#ifdef TR_ENABLE_CHECKED_GRAPHICS
void tr::graphics_context::check_typed_vertex_buffer(const std::string& label, int slot,
													 std::span<const vertex_attribute> vertex_attributes) noexcept
{
	TR_ASSERT(usize(slot) < m_bound_vertex_format_bindings.size(),
			  "Tried to set vertex buffer '{}' to invalid slot {} (max in set vertex format: {}).", label, slot,
			  m_bound_vertex_format_bindings.size());

	const std::span<const vertex_attribute> ref{m_bound_vertex_format_bindings.begin()[slot].attrs};
	TR_ASSERT(
		vertex_attributes.size() == ref.size(),
		"Tried to set vertex buffer '{}' of a different type from the one in the set vertex format (has {} attributes instead of {}).",
		label, vertex_attributes.size(), ref.size());
	for (usize i = 0; i < vertex_attributes.size(); ++i) {
		const vertex_attribute& lhs{vertex_attributes.begin()[i]};
		const vertex_attribute& rhs{ref.begin()[i]};
		TR_ASSERT(lhs.type == rhs.type && lhs.elements == rhs.elements,
				  "Tried to set vertex buffer '{}' of a type different from than the one in the set vertex format (expected '{}' in "
				  "attribute {}, got '{}').",
				  label, rhs, i, lhs);
	}
}
#endif

void tr::graphics_context::set_vertex_buffer(const untyped_static_vertex_buffer& buffer, int slot, ssize offset, int stride) noexcept
{
	TR_ASSERT(buffer.valid(), "Tried to set a vertex buffer in an invalid state to a graphics context.");
	TR_ASSERT(&buffer.context() == this, "Tried to set vertex buffer {} to a context it is not associated with.", buffer);

	if (m_bound_vertex_buffers.size() <= static_cast<usize>(slot)) {
		m_bound_vertex_buffers.resize(slot + 1);
	}

	bound_vertex_buffer_info& bound_vertex_buffer{m_bound_vertex_buffers[slot]};
	if (internal::graphics_object_id id{buffer.id()};
		bound_vertex_buffer.id != id || bound_vertex_buffer.offset != offset || bound_vertex_buffer.stride != stride) {
		TR_LOG_TRACE("gfx", "Binding vertex buffer {} to context.", buffer);
		gl().bind_vertex_buffer(slot, buffer.unwrap(), offset, stride);
		bound_vertex_buffer.id = id;
		bound_vertex_buffer.offset = offset;
		bound_vertex_buffer.stride = stride;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		bound_vertex_buffer.vertex_attributes = {};
#endif
	}
}

void tr::graphics_context::set_vertex_buffer(const untyped_dynamic_vertex_buffer& buffer, int slot, ssize offset, int stride) noexcept
{
	TR_ASSERT(buffer.valid(), "Tried to set a vertex buffer in an invalid state to a graphics context.");
	TR_ASSERT(&buffer.context() == this, "Tried to set vertex buffer {} to a context it is not associated with.", buffer);

	if (m_bound_vertex_buffers.size() <= static_cast<usize>(slot)) {
		m_bound_vertex_buffers.resize(slot + 1);
	}

	bound_vertex_buffer_info& bound_vertex_buffer{m_bound_vertex_buffers[slot]};
	if (internal::graphics_object_id id{buffer.id()};
		bound_vertex_buffer.id != id || bound_vertex_buffer.offset != offset || bound_vertex_buffer.stride != stride) {
		TR_LOG_TRACE("gfx", "Binding vertex buffer {} to context.", buffer);
		gl().bind_vertex_buffer(slot, buffer.unwrap(), offset, stride);
		bound_vertex_buffer.id = id;
		bound_vertex_buffer.offset = offset;
		bound_vertex_buffer.stride = stride;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		bound_vertex_buffer.vertex_attributes = {};
#endif
	}
}

void tr::graphics_context::set_index_buffer(const static_index_buffer& buffer) noexcept
{
	TR_ASSERT(buffer.valid(), "Tried to set invalid index buffer to a graphics context.");
	TR_ASSERT(&buffer.context() == this, "Tried to set index buffer {} to a context it is not associated with.", buffer);

	if (internal::graphics_object_id id{buffer.id()}; m_bound_index_buffer != id) {
		TR_LOG_TRACE("gfx", "Binding index buffer {} to context.", buffer);
		gl().bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.unwrap());
		m_bound_index_buffer = id;
	}
}

void tr::graphics_context::set_index_buffer(const dynamic_index_buffer& buffer) noexcept
{
	TR_ASSERT(buffer.valid(), "Tried to set invalid index buffer to a graphics context.");
	TR_ASSERT(&buffer.context() == this, "Tried to set index buffer {} to a context it is not associated with.", buffer);

	if (internal::graphics_object_id id{buffer.id()}; m_bound_index_buffer != id) {
		TR_LOG_TRACE("gfx", "Binding index buffer {} to context.", buffer);
		gl().bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.unwrap());
		m_bound_index_buffer = id;
	}
}

//

void tr::graphics_context::clear_backbuffer(tr::rgbaf color) noexcept
{
	set_render_target(backbuffer());

	const internal::opengl& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer(tr::rgbaf color, double depth, int stencil) noexcept
{
	set_render_target(backbuffer());

	const internal::opengl& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(rectangle<int> region, tr::rgbaf color) noexcept
{
	set_render_target(backbuffer().cropped(region));

	const internal::opengl& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(rectangle<int> region, tr::rgbaf color, double depth, int stencil) noexcept
{
	set_render_target(backbuffer().cropped(region));

	const internal::opengl& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//

void tr::graphics_context::draw(primitive type, usize offset, usize vertices) noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::no);
#endif

	gl().draw_arrays(std::to_underlying(type), offset, vertices);
}

void tr::graphics_context::draw_instances(primitive type, usize offset, usize vertices, int instances) noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::no);
#endif

	gl().draw_arrays_instanced(std::to_underlying(type), offset, vertices, instances);
}

void tr::graphics_context::draw_indexed(primitive type, usize offset, usize indices) noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::yes);
#endif

	const void* const byte_offset{reinterpret_cast<const void*>(offset * sizeof(u16))};
	gl().draw_elements(std::to_underlying(type), indices, GL_UNSIGNED_SHORT, byte_offset);
}

void tr::graphics_context::draw_indexed_instances(primitive type, usize offset, usize indices, int instances) noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::yes);
#endif

	const void* const byte_offset{reinterpret_cast<const void*>(offset * sizeof(u16))};
	gl().draw_elements_instanced(std::to_underlying(type), indices, GL_UNSIGNED_SHORT, byte_offset, instances);
}

//

SDL_GLContextState* tr::graphics_context::unwrap() const noexcept
{
	return m_ptr.get();
}

//

const tr::internal::opengl& tr::graphics_context::gl() const noexcept
{
	if (SDL_GLContextState* context_pointer{m_ptr.get()}; internal::current_graphics_context != context_pointer) {
		TR_LOG_TRACE("gfx", "Setting graphics context at {} as current.", static_cast<void*>(context_pointer));
		SDL_GL_MakeCurrent(m_window, context_pointer);
	}
	return m_gl;
}

#ifdef TR_ENABLE_CHECKED_GRAPHICS
boost::unordered_flat_set<tr::internal::graphics_object_id>& tr::graphics_context::registered_framebuffers() noexcept
{
	return m_ptr.get_deleter().registered_framebuffers;
}

boost::unordered_flat_set<tr::internal::graphics_object_id>& tr::graphics_context::registered_shaders() noexcept
{
	return m_ptr.get_deleter().registered_shaders;
}

boost::unordered_flat_set<tr::internal::graphics_object_id>& tr::graphics_context::registered_shader_pipelines() noexcept
{
	return m_ptr.get_deleter().registered_shader_pipelines;
}

boost::unordered_flat_set<tr::internal::graphics_object_id>& tr::graphics_context::registered_vertex_formats() noexcept
{
	return m_ptr.get_deleter().registered_vertex_formats;
}

boost::unordered_flat_set<tr::internal::graphics_object_id>& tr::graphics_context::registered_buffers() noexcept
{
	return m_ptr.get_deleter().registered_buffers;
}
#endif

//

unsigned int tr::graphics_context::allocate_texture_unit() noexcept
{
	for (unsigned int free_index{0}; free_index < m_allocated_texture_units.size(); ++free_index) {
		if (!m_allocated_texture_units[free_index]) {
			m_allocated_texture_units[free_index] = true;
			return free_index;
		}
	}
	TR_ASSERT(false, "Tried to allocate more than 80 texture units simultaneously.");
	TR_UNREACHABLE;
}

void tr::graphics_context::free_texture_unit(unsigned int texture_unit) noexcept
{
	TR_ASSERT(m_allocated_texture_units[texture_unit], "Tried to free already free texture unit.");

	m_allocated_texture_units[texture_unit] = false;
}

//

unsigned int tr::graphics_context::allocate_image_unit() noexcept
{
	for (unsigned int free_index{0}; free_index < m_allocated_image_units.size(); ++free_index) {
		if (!m_allocated_image_units[free_index]) {
			m_allocated_image_units[free_index] = true;
			return free_index;
		}
	}
	TR_ASSERT(false, "Tried to allocate more than 80 image units simultaneously.");
	TR_UNREACHABLE;
}

void tr::graphics_context::free_image_unit(unsigned int image_unit) noexcept
{
	TR_ASSERT(m_allocated_image_units[image_unit], "Tried to free already free image unit.");

	m_allocated_image_units[image_unit] = false;
}

//

void tr::graphics_context::move_label(unsigned int type, unsigned int old_id, unsigned int new_id)
{
	const internal::opengl& gl{this->gl()};

	int label_length;
	gl.get_object_label(type, old_id, 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label(label_length, '\0');
		gl.get_object_label(type, old_id, label_length, &label_length, label.data());
		gl.set_object_label(type, new_id, label.size(), label.data());
		gl.set_object_label(type, old_id, 0, nullptr);
	}
}

//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
void tr::graphics_context::assert_valid_drawing_state(check_index_buffer check_index_buffer) noexcept
{
	TR_ASSERT(m_bound_framebuffer == internal::graphics_object_id::invalid || registered_framebuffers().contains(m_bound_framebuffer),
			  "Tried to perform a drawing operation with an invalid set render target.");
	TR_ASSERT(registered_shader_pipelines().contains(m_bound_shader_pipeline),
			  "Tried to perform a drawing operation with an invalid set shader pipeline.");
	TR_ASSERT(registered_vertex_formats().contains(m_bound_vertex_format),
			  "Tried to perform a drawing operation with an invalid set vertex format.");
	for (usize slot = 0; slot < m_bound_vertex_format_bindings.size(); ++slot) {
		TR_ASSERT(registered_buffers().contains(m_bound_vertex_buffers[slot].id),
				  "Tried to perform a drawing operation with an invalid set vertex buffer in slot {}.", slot);

		if (m_bound_vertex_buffers[slot].vertex_attributes.empty()) {
			continue;
		}

		TR_ASSERT(m_bound_vertex_buffers[slot].vertex_attributes.size() == m_bound_vertex_format_bindings[slot].attrs.size(),
				  "Tried to perform a drawing operation with a vertex buffer in slot {} holding a different type from the one in the set "
				  "vertex format (has {} attributes instead of {}).",
				  slot, m_bound_vertex_buffers[slot].vertex_attributes.size(), m_bound_vertex_format_bindings[slot].attrs.size());
		for (usize i = 0; i < m_bound_vertex_format_bindings[i].attrs.size(); ++i) {
			const vertex_attribute& lhs{m_bound_vertex_buffers[i].vertex_attributes[i]};
			const vertex_attribute& rhs{m_bound_vertex_format_bindings[i].attrs[i]};
			TR_ASSERT(lhs.type == rhs.type && lhs.elements == rhs.elements,
					  "Tried to perform a drawing operation with a vertex buffer in slot {} holding a type different from than the one in "
					  "the set vertex format (expected '{}' in attribute {}, got '{}').",
					  m_bound_vertex_format_bindings[slot].attrs, rhs, i, lhs);
		}
	}
	TR_ASSERT(check_index_buffer == check_index_buffer::no || registered_buffers().contains(m_bound_index_buffer),
			  "Tried to perform a drawing operation with an invalid set index buffer.");
}
#endif