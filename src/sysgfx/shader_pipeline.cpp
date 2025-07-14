#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

tr::shader_pipeline::shader_pipeline(const vertex_shader& vshader, const fragment_shader& fshader) noexcept
{
	GLuint id;
	TR_GL_CALL(glCreateProgramPipelines, 1, &id);
	_id.reset(id);

	TR_GL_CALL(glUseProgramStages, _id.get(), GL_VERTEX_SHADER_BIT, vshader._id.get());
	TR_GL_CALL(glUseProgramStages, _id.get(), GL_FRAGMENT_SHADER_BIT, fshader._id.get());
}

void tr::shader_pipeline::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteProgramPipelines, 1, &id);
}

void tr::shader_pipeline::set_label(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM_PIPELINE, _id.get(), static_cast<GLsizei>(label.size()), label.data());
}

void tr::shader_pipeline::bind() const noexcept
{
	TR_GL_CALL(glBindProgramPipeline, _id.get());
}

tr::owning_shader_pipeline::owning_shader_pipeline(tr::vertex_shader&& vshader, tr::fragment_shader&& fshader) noexcept
	: _vshader{std::move(vshader)}, _fshader{std::move(fshader)}, _pipeline{_vshader, _fshader}
{
}

tr::owning_shader_pipeline::operator const tr::shader_pipeline&() const noexcept
{
	return _pipeline;
}

tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() noexcept
{
	return _vshader;
}

const tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() const noexcept
{
	return _vshader;
}

tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() noexcept
{
	return _fshader;
}

const tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() const noexcept
{
	return _fshader;
}

void tr::owning_shader_pipeline::set_label(std::string_view label) noexcept
{
	_pipeline.set_label(label);
}