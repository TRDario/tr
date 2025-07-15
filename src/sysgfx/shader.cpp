#include "../../include/tr/sysgfx/shader.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/texture_unit.hpp"

tr::shader_load_error::shader_load_error(std::string_view path, std::string&& details) noexcept
	: _description{std::format("Failed to load bitmap from '{}'", path)}, _details{std::move(details)}
{
}

std::string_view tr::shader_load_error::name() const noexcept
{
	return "Bitmap loading error";
}

std::string_view tr::shader_load_error::description() const noexcept
{
	return _description;
}

std::string_view tr::shader_load_error::details() const noexcept
{
	return _details;
}

tr::shader_base::shader_base(const char* source, unsigned int type)
{
	_id.reset(TR_RETURNING_GL_CALL(glCreateShaderProgramv, type, 1, &source));
	int linked;
	TR_GL_CALL(glGetProgramiv, _id.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		int buffer_size;
		TR_GL_CALL(glGetProgramiv, _id.get(), GL_INFO_LOG_LENGTH, &buffer_size);
		std::string buffer(buffer_size, '\0');
		TR_GL_CALL(glGetProgramInfoLog, _id.get(), buffer_size, nullptr, buffer.data());
		throw shader_load_error{"(Embedded)", std::format("Failed to compile/link a shader\n{}", buffer)};
	}
}

void tr::shader_base::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteProgram, id);
}

void tr::shader_base::set_uniform(int index, bool value) noexcept
{
	set_uniform(index, static_cast<int>(value));
}

void tr::shader_base::set_uniform(int index, glm::bvec2 value) noexcept
{
	set_uniform(index, static_cast<glm::ivec2>(value));
}

void tr::shader_base::set_uniform(int index, glm::bvec3 value) noexcept
{
	set_uniform(index, static_cast<glm::ivec3>(value));
}

void tr::shader_base::set_uniform(int index, glm::bvec4 value) noexcept
{
	set_uniform(index, static_cast<glm::ivec4>(value));
}

void tr::shader_base::set_uniform(int index, int value) noexcept
{
	TR_GL_CALL(glProgramUniform1i, _id.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const int> value) noexcept
{
	TR_GL_CALL(glProgramUniform1iv, _id.get(), index, static_cast<GLsizei>(value.size()), value.data());
}

void tr::shader_base::set_uniform(int index, glm::ivec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2i, _id.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2iv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::ivec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3i, _id.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3iv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::ivec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4i, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4iv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, unsigned int value) noexcept
{
	TR_GL_CALL(glProgramUniform1ui, _id.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const unsigned int> value) noexcept
{
	TR_GL_CALL(glProgramUniform1uiv, _id.get(), index, static_cast<GLsizei>(value.size()), value.data());
}

void tr::shader_base::set_uniform(int index, glm::uvec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2ui, _id.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2uiv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::uvec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3ui, _id.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3uiv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::uvec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4ui, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4uiv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, float value) noexcept
{
	TR_GL_CALL(glProgramUniform1f, _id.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const float> value) noexcept
{
	TR_GL_CALL(glProgramUniform1fv, _id.get(), index, static_cast<GLsizei>(value.size()), value.data());
}

void tr::shader_base::set_uniform(int index, glm::vec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2f, _id.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2fv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::vec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3f, _id.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3fv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::vec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4f, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4fv, _id.get(), index, static_cast<GLsizei>(value.size()), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2x3& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x3fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2x3> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x3fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2x4& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x4fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2x4> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x4fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3x2& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x2fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3x2> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x2fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3x4& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x4fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3x4> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x4fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4x2& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x2fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4x2> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x2fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4x3& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x3fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4x3> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x3fv, _id.get(), index, static_cast<GLsizei>(value.size()), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const texture_unit& value) noexcept
{
	set_uniform(index, static_cast<int>(value._id.get()));
}

void tr::shader_base::set_storage_buffer(unsigned int index, shader_buffer& buffer) noexcept
{
	TR_GL_CALL(glBindBufferRange, GL_SHADER_STORAGE_BUFFER, index, buffer._id.get(), 0, buffer.header_size() + buffer.array_size());
}

void tr::shader_base::set_label(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM, _id.get(), static_cast<GLsizei>(label.size()), label.data());
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
	catch (std::bad_alloc&) {
		throw shader_load_error{path.string(), "A memory allocation failed while loading the shader."};
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
	catch (std::bad_alloc&) {
		throw shader_load_error{path.string(), "A memory allocation failed while loading the shader."};
	}
}