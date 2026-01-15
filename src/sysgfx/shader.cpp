#include "../../include/tr/sysgfx/shader.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/uniform_buffer.hpp"

namespace tr::gfx {
#ifdef TR_ENABLE_GL_CHECKS
	// Properties queried for uniforms.
	constexpr std::array<GLenum, 5> uniform_properties{GL_BLOCK_INDEX, GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};
	// Properties queried for inputs and outputs.
	constexpr std::array<GLenum, 4> input_output_properties{GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};
#endif

	// Tracks which units are allocated.
	std::array<bool, 80> g_texture_units;
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
	TR_ASSERT(std::ranges::find(g_texture_units, false) != g_texture_units.end(), "Ran out of texture units for shaders.");

	auto it{std::ranges::find(g_texture_units, false)};
	*it = true;
	g_texture_unit_textures[it - g_texture_units.begin()] = std::nullopt;
	id.reset((unsigned int)(it - g_texture_units.begin()));
}

void tr::gfx::shader_base::texture_unit::deleter::operator()(unsigned int unit) const
{
	g_texture_units[unit] = false;
}

//

tr::gfx::shader_base::shader_base(cstring_view source, unsigned int type)
{
	m_program.reset(TR_RETURNING_GL_CALL(glCreateShaderProgramv, type, 1, (const char**)&source));
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
	find_uniforms();
	find_inputs();
	find_outputs();
#endif
}

#ifdef TR_ENABLE_GL_CHECKS
void tr::gfx::shader_base::find_uniforms()
{
	GLint uniforms{0};
	glGetProgramInterfaceiv(m_program.get(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniforms);
	for (GLint i = 0; i < uniforms; ++i) {
		std::array<GLint, uniform_properties.size()> values;
		const auto& [block_index, var_type, array_size, name_length, location]{values};
		glGetProgramResourceiv(m_program.get(), GL_UNIFORM, i, uniform_properties.size(), uniform_properties.data(),
							   uniform_properties.size(), NULL, values.data());

		if (block_index != -1) {
			continue;
		}

		std::string buffer(name_length, '\0');
		glGetProgramResourceName(m_program.get(), GL_UNIFORM, i, buffer.size(), NULL, buffer.data());
		m_uniforms.insert({(unsigned int)(location), {std::move(buffer), tr::gfx::glsl_type(var_type), array_size}});
	}
}

void tr::gfx::shader_base::find_inputs()
{
	GLint inputs{0};
	glGetProgramInterfaceiv(m_program.get(), GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputs);
	for (GLint i = 0; i < inputs; ++i) {
		std::array<GLint, input_output_properties.size()> values;
		const auto& [var_type, array_size, name_length, location]{values};
		glGetProgramResourceiv(m_program.get(), GL_PROGRAM_INPUT, i, input_output_properties.size(), input_output_properties.data(),
							   input_output_properties.size(), NULL, values.data());

		std::string buffer(name_length, '\0');
		glGetProgramResourceName(m_program.get(), GL_PROGRAM_INPUT, i, buffer.size(), NULL, buffer.data());
		m_inputs.insert({(unsigned int)(location), {std::move(buffer), tr::gfx::glsl_type(var_type), array_size}});
	}
}

void tr::gfx::shader_base::find_outputs()
{
	GLint inputs{0};
	glGetProgramInterfaceiv(m_program.get(), GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &inputs);
	for (GLint i = 0; i < inputs; ++i) {
		std::array<GLint, input_output_properties.size()> values;
		const auto& [var_type, array_size, name_length, location]{values};
		glGetProgramResourceiv(m_program.get(), GL_PROGRAM_OUTPUT, i, input_output_properties.size(), input_output_properties.data(),
							   input_output_properties.size(), NULL, values.data());

		std::string buffer(name_length, '\0');
		glGetProgramResourceName(m_program.get(), GL_PROGRAM_OUTPUT, i, buffer.size(), NULL, buffer.data());
		if (!buffer.starts_with("gl_")) {
			m_outputs.insert({(unsigned int)(location), {std::move(buffer), tr::gfx::glsl_type(var_type), array_size}});
		}
	}
}
#endif

void tr::gfx::shader_base::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteProgram, id);
}

void tr::gfx::shader_base::set_uniform(int index, bool value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::bool_ && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'bool'.", it->second, label(), label());
#endif
	TR_GL_CALL(glProgramUniform1i, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, int value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::int_ && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'int'.", it->second, label(), label());
#endif
	TR_GL_CALL(glProgramUniform1i, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const int> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::int_ && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'int[{}]'.", it->second, label(), label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform1iv, m_program.get(), index, GLsizei(value.size()), value.data());
}

void tr::gfx::shader_base::set_uniform(int index, glm::ivec2 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::ivec2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'ivec2'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform2i, m_program.get(), index, value.x, value.y);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::ivec2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::ivec2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'ivec2[{}]'.", it->second, label(), label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform2iv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::ivec3 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::ivec3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'ivec3'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform3i, m_program.get(), index, value.x, value.y, value.z);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::ivec3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::ivec3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'ivec3[{}]'.", it->second, label(), label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform3iv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::ivec4 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::ivec4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'ivec4'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform4i, m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::ivec4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::ivec4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'ivec4[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform4iv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, unsigned int value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uint && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'unsigned int'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform1ui, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const unsigned int> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uint && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'unsigned int[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform1uiv, m_program.get(), index, GLsizei(value.size()), value.data());
}

void tr::gfx::shader_base::set_uniform(int index, glm::uvec2 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uvec2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'uvec2'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform2ui, m_program.get(), index, value.x, value.y);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::uvec2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uvec2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'uvec2[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform2uiv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::uvec3 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uvec3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'uvec3'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform3ui, m_program.get(), index, value.x, value.y, value.z);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::uvec3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uvec3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'uvec3[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform3uiv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::uvec4 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uvec4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'uvec4'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform4ui, m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::uvec4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::uvec4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'uvec4[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform4uiv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, float value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::float_ && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'float'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform1f, m_program.get(), index, value);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const float> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::float_ && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'float[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform1fv, m_program.get(), index, GLsizei(value.size()), value.data());
}

void tr::gfx::shader_base::set_uniform(int index, glm::vec2 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::vec2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'vec2'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform2f, m_program.get(), index, value.x, value.y);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::vec2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::vec2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'vec2[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform2fv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::vec3 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::vec3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'vec3'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform3f, m_program.get(), index, value.x, value.y, value.z);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::vec3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::vec3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'vec3[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform3fv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, glm::vec4 value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::vec4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'vec4'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniform4f, m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::vec4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::vec4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'vec4[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniform4fv, m_program.get(), index, GLsizei(value.size()), value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat2& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat2'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix2fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat2[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix2fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat3& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat3'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix3fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat3[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix3fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat4& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat4'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix4fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat4[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix4fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat2x3& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat2x3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat2x3'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix2x3fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat2x3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat2x3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat2x3[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix2x3fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat2x4& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat2x4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat2x4'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix2x4fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat2x4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat2x4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat2x4[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix2x4fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat3x2& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat3x2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat3x2'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix3x2fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat3x2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat3x2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat3x2[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix3x2fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat3x4& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat3x4 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat3x4'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix3x4fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat3x4> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat3x4 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat3x4[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix3x4fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat4x2& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat4x2 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat4x2'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix4x2fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat4x2> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat4x2 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat4x2[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix4x2fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, const glm::mat4x3& value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat4x3 && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat4x3'.", it->second, label());
#endif
	TR_GL_CALL(glProgramUniformMatrix4x3fv, m_program.get(), index, 1, false, value_ptr(value));
}

void tr::gfx::shader_base::set_uniform(int index, std::span<const glm::mat4x3> value)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::mat4x3 && it->second.array_size == int(value.size()),
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'mat4x3[{}]'.", it->second, label(),
			  value.size());
#endif
	TR_GL_CALL(glProgramUniformMatrix4x3fv, m_program.get(), index, GLsizei(value.size()), false, value_ptr(value[0]));
}

void tr::gfx::shader_base::set_uniform(int index, texture_ref texture)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto it{m_uniforms.find(index)};
	TR_ASSERT(it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(it->second.type == glsl_type::sampler2D && it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'sampler2D'.", it->second, label());
#endif

	auto unit_it{m_texture_units.find(index)};
	if (unit_it == m_texture_units.end()) {
		unit_it = m_texture_units.insert({index, texture_unit{}}).first;
		TR_GL_CALL(glProgramUniform1i, m_program.get(), index, unit_it->second.id.get());
	}

	const unsigned int unit_id{unit_it->second.id.get()};
	if (!texture.empty()) {
		if (g_texture_unit_textures[unit_id] != texture && texture.m_ref->m_handle != 0) {
			TR_GL_CALL(glBindTextures, unit_id, 1, &texture.m_ref->m_handle);
		}
	}
	g_texture_unit_textures[unit_id] = std::move(texture);
}

void tr::gfx::shader_base::set_storage_buffer(unsigned int index, basic_shader_buffer& buffer)
{
	TR_GL_CALL(glBindBufferRange, GL_SHADER_STORAGE_BUFFER, index, buffer.m_sbo.get(), 0,
			   GLsizeiptr(buffer.header_size() + buffer.array_size()));
}

void tr::gfx::shader_base::set_uniform_buffer(unsigned int index, const basic_uniform_buffer& buffer)
{
	TR_GL_CALL(glBindBufferBase, GL_UNIFORM_BUFFER, index, buffer.m_ubo.get());
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::shader_base::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM, m_program.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::shader_base::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_PROGRAM, m_program.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_PROGRAM, m_program.get(), length + 1, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif

tr::gfx::vertex_shader::vertex_shader(cstring_view source)
	: shader_base{source, GL_VERTEX_SHADER}
{
}

tr::gfx::vertex_shader tr::gfx::load_vertex_shader(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		vertex_shader shader{std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
		TR_SET_LABEL(shader, path.filename().string());
		return shader;
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

tr::gfx::tessellation_control_shader::tessellation_control_shader(cstring_view source)
	: shader_base{source, GL_TESS_CONTROL_SHADER}
{
}

tr::gfx::tessellation_control_shader tr::gfx::load_tessellation_control_shader(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		tessellation_control_shader shader{std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
		TR_SET_LABEL(shader, path.filename().string());
		return shader;
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

tr::gfx::tessellation_evaluation_shader::tessellation_evaluation_shader(cstring_view source)
	: shader_base{source, GL_TESS_EVALUATION_SHADER}
{
}

tr::gfx::tessellation_evaluation_shader tr::gfx::load_tessellation_evaluation_shader(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		tessellation_evaluation_shader shader{std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
		TR_SET_LABEL(shader, path.filename().string());
		return shader;
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

tr::gfx::fragment_shader::fragment_shader(cstring_view source)
	: shader_base{source, GL_FRAGMENT_SHADER}
{
}

tr::gfx::fragment_shader tr::gfx::load_fragment_shader(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		fragment_shader shader{std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
		TR_SET_LABEL(shader, path.filename().string());
		return shader;
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