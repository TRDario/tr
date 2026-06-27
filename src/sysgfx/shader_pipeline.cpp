///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements shader_pipeline.hpp.                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/utility/hash_map.hpp"
#include "tr/sysgfx/shader.hpp"

///////////////////////////////////////////////////////////// SHADER PIPELINE /////////////////////////////////////////////////////////////

tr::shader_pipeline::shader_pipeline(graphics_context& context)
	: m_ppo{{context}}
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.create_program_pipelines(1, out_handle(m_ppo));
}

tr::shader_pipeline::shader_pipeline(graphics_context& context, const vertex_shader& vshader, const fragment_shader& fshader)
	: shader_pipeline{context}
{
#ifdef TR_ENABLE_GL_CHECKS
	TR_ASSERT(vshader.m_outputs.size() == fshader.m_inputs.size(),
			  "Mismatched shader inputs/outputs (vertex shader '{}' has {} outputs, fragment shader '{}' has {} inputs).", vshader.label(),
			  vshader.m_outputs.size(), fshader.label(), fshader.m_inputs.size());
	for (const auto& [location, info] : vshader.m_outputs) {
		TR_ASSERT(fshader.m_inputs.contains(location),
				  "Mismatched shader inputs/outputs (vertex shader '{}' has output '{}' at location {} that was not found in fragment "
				  "shader '{}''s inputs).",
				  vshader.label(), info, location, fshader.label());
		const glsl_variable& frag_info{get(fshader.m_inputs, location)};
		TR_ASSERT(frag_info.type == info.type && frag_info.array_size == info.array_size,
				  "Mismatched shader inputs/outputs (vertex shader '{}' has output '{}' at location {}, but the input '{}' at the same "
				  "location in fragment shader '{}' is not compatible with it).",
				  vshader.label(), info, location, get(fshader.m_inputs, location), fshader.label());
	}
#endif

	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.use_program_stages(m_ppo.get(), GL_VERTEX_SHADER_BIT, vshader.m_program.get());
	gl.use_program_stages(m_ppo.get(), GL_FRAGMENT_SHADER_BIT, fshader.m_program.get());
}

void tr::shader_pipeline::deleter::operator()(unsigned int id) const
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.delete_program_pipelines(1, &id);
}

//

tr::graphics_context& tr::shader_pipeline::context() const
{
	return m_ppo.get_deleter().context;
}

//

#ifdef TR_ENABLE_ASSERTS
void tr::shader_pipeline::set_label(std::string_view label)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.set_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), label.size(), label.data());
}

std::string tr::shader_pipeline::label() const
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	int label_length;
	gl.get_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}
#endif

////////////////////////////////////////////////////////// OWNING SHADER PIPELINE /////////////////////////////////////////////////////////

tr::owning_shader_pipeline::owning_shader_pipeline(graphics_context& context, tr::vertex_shader&& vshader, tr::fragment_shader&& fshader)
	: m_vshader{std::move(vshader)}
	, m_fshader{std::move(fshader)}
	, m_base{context, m_vshader, m_fshader}
{
}

tr::owning_shader_pipeline::operator const tr::shader_pipeline&() const
{
	return m_base;
}

tr::graphics_context& tr::owning_shader_pipeline::context() const
{
	return m_base.context();
}

tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader()
{
	return m_vshader;
}

const tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() const
{
	return m_vshader;
}

tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader()
{
	return m_fshader;
}

const tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() const
{
	return m_fshader;
}

#ifdef TR_ENABLE_ASSERTS
void tr::owning_shader_pipeline::set_label(std::string_view label)
{
	m_base.set_label(label);
}

std::string tr::owning_shader_pipeline::label() const
{
	return m_base.label();
}
#endif