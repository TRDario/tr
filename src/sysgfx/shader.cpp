#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/shader.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

namespace tr::gfx {
	// Properties queried for uniforms.
	constexpr std::array<GLenum, 5> UNIFORM_PROPERTIES{GL_BLOCK_INDEX, GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};

	// Tracks which units are allocated.
	std::array<bool, 80> texture_units;
} // namespace tr::gfx

tr::gfx::shader_load_error::shader_load_error(std::string_view path, std::string&& details)
	: m_description{TR_FMT::format("Failed to load bitmap from '{}'", path)}, m_details{std::move(details)}
{
}

std::string_view tr::gfx::shader_load_error::name() const
{
	return "Bitmap loading error";
}

std::string_view tr::gfx::shader_load_error::description() const
{
	return m_description;
}

std::string_view tr::gfx::shader_load_error::details() const
{
	return m_details;
}

//

tr::gfx::shader_base::texture_unit::texture_unit()
{
	TR_ASSERT(std::ranges::find(texture_units, false) != texture_units.end(), "Ran out of texture units for shaders.");

	auto it{std::ranges::find(texture_units, false)};
	*it = true;
	texture_unit_textures[it - texture_units.begin()] = std::nullopt;
	id.reset((unsigned int)(it - texture_units.begin()));
}

void tr::gfx::shader_base::texture_unit::deleter::operator()(unsigned int unit) const
{
	texture_units[unit] = false;
}

//

tr::gfx::shader_base::shader_base(const char* source, unsigned int type)
{
	m_program.reset(TR_RETURNING_GL_CALL(glCreateShaderProgramv, type, 1, &source));
	int linked;
	TR_GL_CALL(glGetProgramiv, m_program.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		int buffer_size;
		TR_GL_CALL(glGetProgramiv, m_program.get(), GL_INFO_LOG_LENGTH, &buffer_size);
		std::string buffer(buffer_size, '\0');
		TR_GL_CALL(glGetProgramInfoLog, m_program.get(), buffer_size, nullptr, buffer.data());
		throw shader_load_error{"(Embedded)", TR_FMT::format("Failed to compile/link a shader\n{}", buffer)};
	}

#ifdef TR_ENABLE_GL_CHECKS
	GLint uniforms{0};
	glGetProgramInterfaceiv(m_program.get(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniforms);
	for (GLint i = 0; i < uniforms; ++i) {
		std::array<GLint, UNIFORM_PROPERTIES.size()> values;
		const auto& [block_index, var_type, array_size, name_length, location]{values};
		glGetProgramResourceiv(m_program.get(), GL_UNIFORM, i, UNIFORM_PROPERTIES.size(), UNIFORM_PROPERTIES.data(),
							   UNIFORM_PROPERTIES.size(), NULL, values.data());

		if (values[0] != -1) {
			continue;
		}

		std::string buffer(name_length, '\0');
		glGetProgramResourceName(m_program.get(), GL_UNIFORM, i, buffer.size(), NULL, buffer.data());
		m_uniforms.insert({(unsigned int)(location), {std::move(buffer), tr::gfx::glsl_type(var_type), array_size}});
	}
#endif
}

void tr::gfx::shader_base::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteProgram, id);
}

void tr::gfx::shader_base::set_uniform(int index, bool value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::BOOL && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'bool'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform1i, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, int value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::INT && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'int'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform1i, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const int> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::INT && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'int[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform1iv, m_program.get(), index, GLsizei(value.size()), value.data());
}

void tr::gfx::shader_base::set_uniform(int index, glm::ivec2 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::IVEC2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'ivec2'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform2i, m_program.get(), index, value.x, value.y);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::ivec2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::IVEC2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'ivec2[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform2iv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::ivec3 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::IVEC3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'ivec3'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform3i, m_program.get(), index, value.x, value.y, value.z);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::ivec3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::IVEC3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'ivec3[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform3iv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::ivec4 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::IVEC4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'ivec4'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform4i, m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::ivec4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::IVEC4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'ivec4[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform4iv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, unsigned int value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UINT && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'unsigned int'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform1ui, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const unsigned int> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UINT && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'unsigned int[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform1uiv, m_program.get(), index, GLsizei(value.size()), value.data());
}

void tr::gfx::shader_base::set_uniform(int index, glm::uvec2 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UVEC2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'uvec2'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform2ui, m_program.get(), index, value.x, value.y);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::uvec2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UVEC2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'uvec2[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform2uiv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::uvec3 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UVEC3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'uvec3'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform3ui, m_program.get(), index, value.x, value.y, value.z);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::uvec3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UVEC3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'uvec3[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform3uiv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::uvec4 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UVEC4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'uvec4'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform4ui, m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::uvec4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::UVEC4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'uvec4[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform4uiv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, float value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::FLOAT && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'float'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform1f, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const float> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::FLOAT && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'float[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform1fv, m_program.get(), index, GLsizei(value.size()), value.data());
}

void tr::gfx::shader_base::set_uniform(int index, glm::vec2 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::VEC2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'vec2'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform2f, m_program.get(), index, value.x, value.y);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::vec2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::VEC2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'vec2[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform2fv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::vec3 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::VEC3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'vec3'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform3f, m_program.get(), index, value.x, value.y, value.z);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::vec3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::VEC3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'vec3[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform3fv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::vec4 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::VEC4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'vec4'.", it->second);
#endif
	TR_GL_CALL(glProgramUniform4f, m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::vec4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::VEC4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'vec4[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniform4fv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat2& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat2'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix2fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat2[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix2fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat3& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat3'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix3fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat3[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix3fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat4& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat4'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix4fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat4[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix4fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat2x3& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT2x3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat2x3'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix2x3fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat2x3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT2x3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat2x3[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix2x3fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat2x4& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT2x4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat2x4'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix2x4fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat2x4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT2x4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat2x4[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix2x4fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat3x2& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT3x2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat3x2'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix3x2fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat3x2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT3x2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat3x2[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix3x2fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat3x4& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT3x4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat3x4'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix3x4fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat3x4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT3x4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat3x4[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix3x4fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat4x2& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT4x2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat4x2'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix4x2fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat4x2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT4x2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat4x2[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix4x2fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat4x3& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT4x3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'mat4x3'.", it->second);
#endif
	TR_GL_CALL(glProgramUniformMatrix4x3fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat4x3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::MAT4x3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' with a value of type 'mat4x3[{}]'.", it->second, value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix4x3fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, texture_ref texture)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}'.", index);
	TR_ASSERT(it->second.type == glsl_type::SAMPLER2D && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' with a value of type 'sampler2D'.", it->second);
#endif

	auto unit_it{m_texture_units.find(index)};
	if (unit_it == m_texture_units.end()) {
		unit_it = m_texture_units.insert({index, texture_unit{}}).first;
		TR_GL_CALL(glProgramUniform1i, m_program.get(), index, unit_it->second.id.get());
	}

	const unsigned int unit_id{unit_it->second.id.get()};
	if (!texture.empty()) {
		if (texture_unit_textures[unit_id] != texture && texture.m_ref->m_handle != 0) {
			TR_GL_CALL(glBindTextures, unit_id, 1, &texture.m_ref->m_handle);
		}
	}
	texture_unit_textures[unit_id] = std::move(texture);
}

void tr::gfx::shader_base::set_storage_buffer(unsigned int index, shader_buffer& buffer)
{
	TR_GL_CALL(glBindBufferRange, GL_SHADER_STORAGE_BUFFER, index, buffer.m_sbo.get(), 0, buffer.header_size() + buffer.array_size());
}

void tr::gfx::shader_base::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM, m_program.get(), GLsizei(label.size()), label.data());
}

tr::gfx::vertex_shader::vertex_shader(const char* source)
	: shader_base{source, GL_VERTEX_SHADER}
{
}

tr::gfx::vertex_shader tr::gfx::load_vertex_shader(const std::filesystem::path& path)
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

tr::gfx::fragment_shader::fragment_shader(const char* source)
	: shader_base{source, GL_FRAGMENT_SHADER}
{
}

tr::gfx::fragment_shader tr::gfx::load_fragment_shader(const std::filesystem::path& path)
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