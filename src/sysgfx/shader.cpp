#include "../../include/tr/sysgfx/shader.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/texture_unit.hpp"

tr::shader_load_error::shader_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load bitmap from '{}'", path)}, m_details{std::move(details)}
{
}

std::string_view tr::shader_load_error::name() const
{
	return "Bitmap loading error";
}

std::string_view tr::shader_load_error::description() const
{
	return m_description;
}

std::string_view tr::shader_load_error::details() const
{
	return m_details;
}

tr::shader_base::shader_base(const char* source, unsigned int type)
{
	program.reset(TR_RETURNING_GL_CALL(glCreateShaderProgramv, type, 1, &source));
	int linked;
	TR_GL_CALL(glGetProgramiv, program.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		int buffer_size;
		TR_GL_CALL(glGetProgramiv, program.get(), GL_INFO_LOG_LENGTH, &buffer_size);
		std::string buffer(buffer_size, '\0');
		TR_GL_CALL(glGetProgramInfoLog, program.get(), buffer_size, nullptr, buffer.data());
		throw shader_load_error{"(Embedded)", std::format("Failed to compile/link a shader\n{}", buffer)};
	}
}

void tr::shader_base::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteProgram, id);
}

void tr::shader_base::set_uniform(int index, bool value)
{
	set_uniform(index, static_cast<int>(value));
}

void tr::shader_base::set_uniform(int index, glm::bvec2 value)
{
	set_uniform(index, static_cast<glm::ivec2>(value));
}

void tr::shader_base::set_uniform(int index, glm::bvec3 value)
{
	set_uniform(index, static_cast<glm::ivec3>(value));
}

void tr::shader_base::set_uniform(int index, glm::bvec4 value)
{
	set_uniform(index, static_cast<glm::ivec4>(value));
}

void tr::shader_base::set_uniform(int index, int value)
{
	TR_GL_CALL(glProgramUniform1i, program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const int> value)
{
	TR_GL_CALL(glProgramUniform1iv, program.get(), index, static_cast<GLsizei>(value.size()), value.data());
}

void tr::shader_base::set_uniform(int index, glm::ivec2 value)
{
	TR_GL_CALL(glProgramUniform2i, program.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec2> value)
{
	TR_GL_CALL(glProgramUniform2iv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::ivec3 value)
{
	TR_GL_CALL(glProgramUniform3i, program.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec3> value)
{
	TR_GL_CALL(glProgramUniform3iv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::ivec4 value)
{
	TR_GL_CALL(glProgramUniform4i, program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec4> value)
{
	TR_GL_CALL(glProgramUniform4iv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, unsigned int value)
{
	TR_GL_CALL(glProgramUniform1ui, program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const unsigned int> value)
{
	TR_GL_CALL(glProgramUniform1uiv, program.get(), index, static_cast<GLsizei>(value.size()), value.data());
}

void tr::shader_base::set_uniform(int index, glm::uvec2 value)
{
	TR_GL_CALL(glProgramUniform2ui, program.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec2> value)
{
	TR_GL_CALL(glProgramUniform2uiv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::uvec3 value)
{
	TR_GL_CALL(glProgramUniform3ui, program.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec3> value)
{
	TR_GL_CALL(glProgramUniform3uiv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::uvec4 value)
{
	TR_GL_CALL(glProgramUniform4ui, program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec4> value)
{
	TR_GL_CALL(glProgramUniform4uiv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, float value)
{
	TR_GL_CALL(glProgramUniform1f, program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const float> value)
{
	TR_GL_CALL(glProgramUniform1fv, program.get(), index, static_cast<GLsizei>(value.size()), value.data());
}

void tr::shader_base::set_uniform(int index, glm::vec2 value)
{
	TR_GL_CALL(glProgramUniform2f, program.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec2> value)
{
	TR_GL_CALL(glProgramUniform2fv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::vec3 value)
{
	TR_GL_CALL(glProgramUniform3f, program.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec3> value)
{
	TR_GL_CALL(glProgramUniform3fv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::vec4 value)
{
	TR_GL_CALL(glProgramUniform4f, program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec4> value)
{
	TR_GL_CALL(glProgramUniform4fv, program.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2& value)
{
	TR_GL_CALL(glProgramUniformMatrix2fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2> value)
{
	TR_GL_CALL(glProgramUniformMatrix2fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3& value)
{
	TR_GL_CALL(glProgramUniformMatrix3fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3> value)
{
	TR_GL_CALL(glProgramUniformMatrix3fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4& value)
{
	TR_GL_CALL(glProgramUniformMatrix4fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4> value)
{
	TR_GL_CALL(glProgramUniformMatrix4fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2x3& value)
{
	TR_GL_CALL(glProgramUniformMatrix2x3fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2x3> value)
{
	TR_GL_CALL(glProgramUniformMatrix2x3fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2x4& value)
{
	TR_GL_CALL(glProgramUniformMatrix2x4fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2x4> value)
{
	TR_GL_CALL(glProgramUniformMatrix2x4fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3x2& value)
{
	TR_GL_CALL(glProgramUniformMatrix3x2fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3x2> value)
{
	TR_GL_CALL(glProgramUniformMatrix3x2fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3x4& value)
{
	TR_GL_CALL(glProgramUniformMatrix3x4fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3x4> value)
{
	TR_GL_CALL(glProgramUniformMatrix3x4fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4x2& value)
{
	TR_GL_CALL(glProgramUniformMatrix4x2fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4x2> value)
{
	TR_GL_CALL(glProgramUniformMatrix4x2fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4x3& value)
{
	TR_GL_CALL(glProgramUniformMatrix4x3fv, program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4x3> value)
{
	TR_GL_CALL(glProgramUniformMatrix4x3fv, program.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const texture_unit& value)
{
	set_uniform(index, static_cast<int>(value.unit.get()));
}

void tr::shader_base::set_storage_buffer(unsigned int index, shader_buffer& buffer)
{
	TR_GL_CALL(glBindBufferRange, GL_SHADER_STORAGE_BUFFER, index, buffer.sbo.get(), 0, buffer.header_size() + buffer.array_size());
}

void tr::shader_base::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM, program.get(), static_cast<GLsizei>(label.size()), label.data());
}

tr::vertex_shader::vertex_shader(const char* source)
	: shader_base{source, GL_VERTEX_SHADER}
{
}

tr::vertex_shader tr::load_vertex_shader(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		const std::string source{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
		return vertex_shader{source.c_str()};
	}
	catch (shader_load_error& err) {
		throw shader_load_error{path.string(), std::string{err.details()}};
	}
	catch (file_not_found&) {
		throw shader_load_error{path.string(), "File not found."};
	}
	catch (file_open_error&) {
		throw shader_load_error{path.string(), "An error occurred when trying to open the file."};
	}
}

tr::fragment_shader::fragment_shader(const char* source)
	: shader_base{source, GL_FRAGMENT_SHADER}
{
}

tr::fragment_shader tr::load_fragment_shader(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		const std::string source{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
		return fragment_shader{source.c_str()};
	}
	catch (shader_load_error& err) {
		throw shader_load_error{path.string(), std::string{err.details()}};
	}
	catch (file_not_found&) {
		throw shader_load_error{path.string(), "File not found."};
	}
	catch (file_open_error&) {
		throw shader_load_error{path.string(), "An error occurred when trying to open the file."};
	}
}