#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

tr::gfx::shader_pipeline::shader_pipeline(const vertex_shader& vshader, const fragment_shader& fshader)
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
		const glsl_variable& frag_info{fshader.m_inputs.at(location)};
		TR_ASSERT(frag_info.type == info.type && frag_info.array_size == info.array_size,
				  "Mismatched shader inputs/outputs (vertex shader '{}' has output '{}' at location {}, but the input '{}' at the same "
				  "location in fragment shader '{}' is not compatible with it).",
				  vshader.label(), info, location, fshader.m_inputs.at(location), fshader.label());
	}
#endif

	GLuint ppo;
	TR_GL_CALL(glCreateProgramPipelines, 1, &ppo);
	m_ppo.reset(ppo);

	TR_GL_CALL(glUseProgramStages, m_ppo.get(), GL_VERTEX_SHADER_BIT, vshader.m_program.get());
	TR_GL_CALL(glUseProgramStages, m_ppo.get(), GL_FRAGMENT_SHADER_BIT, fshader.m_program.get());
}

void tr::gfx::shader_pipeline::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteProgramPipelines, 1, &id);
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::shader_pipeline::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM_PIPELINE, m_ppo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::shader_pipeline::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_PROGRAM_PIPELINE, m_ppo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_PROGRAM_PIPELINE, m_ppo.get(), length + 1, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif

//

tr::gfx::owning_shader_pipeline::owning_shader_pipeline(gfx::vertex_shader&& vshader, gfx::fragment_shader&& fshader)
	: m_vshader{std::move(vshader)}, m_fshader{std::move(fshader)}, m_base{m_vshader, m_fshader}
{
}

tr::gfx::owning_shader_pipeline::operator const tr::gfx::shader_pipeline&() const
{
	return m_base;
}

tr::gfx::vertex_shader& tr::gfx::owning_shader_pipeline::vertex_shader()
{
	return m_vshader;
}

const tr::gfx::vertex_shader& tr::gfx::owning_shader_pipeline::vertex_shader() const
{
	return m_vshader;
}

tr::gfx::fragment_shader& tr::gfx::owning_shader_pipeline::fragment_shader()
{
	return m_fshader;
}

const tr::gfx::fragment_shader& tr::gfx::owning_shader_pipeline::fragment_shader() const
{
	return m_fshader;
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::owning_shader_pipeline::set_label(std::string_view label)
{
	m_base.set_label(label);
}

std::string tr::gfx::owning_shader_pipeline::label() const
{
	return m_base.label();
}
#endif