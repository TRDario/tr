#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

tr::shader_pipeline::shader_pipeline(const vertex_shader& vshader, const fragment_shader& fshader)
{
	GLuint id;
	TR_GL_CALL(glCreateProgramPipelines, 1, &id);
	ppo.reset(id);

	TR_GL_CALL(glUseProgramStages, ppo.get(), GL_VERTEX_SHADER_BIT, vshader.program.get());
	TR_GL_CALL(glUseProgramStages, ppo.get(), GL_FRAGMENT_SHADER_BIT, fshader.program.get());
}

void tr::shader_pipeline::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteProgramPipelines, 1, &id);
}

void tr::shader_pipeline::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM_PIPELINE, ppo.get(), static_cast<GLsizei>(label.size()), label.data());
}

tr::owning_shader_pipeline::owning_shader_pipeline(tr::vertex_shader&& vshader, tr::fragment_shader&& fshader)
	: vshader{std::move(vshader)}, fshader{std::move(fshader)}, base{vshader, fshader}
{
}

tr::owning_shader_pipeline::operator const tr::shader_pipeline&() const
{
	return base;
}

tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader()
{
	return vshader;
}

const tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() const
{
	return vshader;
}

tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader()
{
	return fshader;
}

const tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() const
{
	return fshader;
}

void tr::owning_shader_pipeline::set_label(std::string_view label)
{
	base.set_label(label);
}