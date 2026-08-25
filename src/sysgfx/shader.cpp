/// @file
/// @brief Implements the non-templated parts of shader.hpp.

#include "../../include/tr/sysgfx/shader.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"
#include "../../include/tr/sysgfx/uniform_buffer.hpp"
#include "../../include/tr/utility/hash_map.hpp"
#include "../../include/tr/utility/iostream.hpp"

//

tr::shader_load_error::shader_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load shader from '{}'", path)}
	, m_details{std::move(details)}
{
}

std::string_view tr::shader_load_error::name() const
{
	return "Shader loading error";
}

std::string_view tr::shader_load_error::description() const
{
	return m_description;
}

std::string_view tr::shader_load_error::details() const
{
	return m_details;
}

//

tr::shader::shader(graphics_context& context, zstring_view source, unsigned int type)
	: m_program{context.gl().create_shader_program_v(type, 1, reinterpret_cast<const char**>(&source)), deleter{context}}
{
#ifdef TR_ENABLE_GL_CHECKS
	context.registry().register_shader(id(), unwrap());
#endif

	const gl_api& gl{context.gl()};

	int linked;
	gl.get_program_iv(unwrap(), GL_LINK_STATUS, &linked);
	if (!linked) {
		int info_log_buffer_size;
		gl.get_program_iv(unwrap(), GL_INFO_LOG_LENGTH, &info_log_buffer_size);
		std::string info_log_buffer(info_log_buffer_size, '\0');
		gl.get_program_info_log(unwrap(), info_log_buffer_size, nullptr, info_log_buffer.data());
		throw shader_load_error{"(Embedded)", std::format("Failed to compile/link a shader\n{}", info_log_buffer)};
	}

#ifdef TR_ENABLE_GL_CHECKS
	find_uniforms(gl);
	find_inputs(gl);
	find_outputs(gl);
#endif
}

#ifdef TR_ENABLE_GL_CHECKS
namespace tr
{
	namespace
	{
		/// Properties queried for uniforms.
		constexpr std::array<unsigned int, 5> uniform_properties{GL_BLOCK_INDEX, GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};

		/// Properties queried for inputs and outputs.
		constexpr std::array<unsigned int, 4> input_output_properties{GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};
	} // namespace
} // namespace tr

void tr::shader::find_uniforms(const gl_api& gl)
{
	int uniforms{0};
	gl.get_program_interface_iv(unwrap(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniforms);
	for (int i = 0; i < uniforms; ++i) {
		std::array<int, uniform_properties.size()> values;
		const auto& [block_index, var_type, array_size, name_length, location]{values};
		gl.get_program_resource_iv(unwrap(), GL_UNIFORM, i, uniform_properties.size(), uniform_properties.data(), uniform_properties.size(),
								   NULL, values.data());

		if (block_index != -1) {
			continue;
		}

		std::string uniform_name_buffer(name_length, '\0');
		gl.get_program_resource_name(unwrap(), GL_UNIFORM, i, uniform_name_buffer.size(), NULL, uniform_name_buffer.data());
		m_uniforms.insert({
			static_cast<unsigned int>(location),
			{std::move(uniform_name_buffer), static_cast<glsl_type>(var_type), array_size},
		});
	}
}

void tr::shader::find_inputs(const gl_api& gl)
{
	int inputs{0};
	gl.get_program_interface_iv(unwrap(), GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputs);
	for (int i = 0; i < inputs; ++i) {
		std::array<int, input_output_properties.size()> values;
		const auto& [var_type, array_size, name_length, location]{values};
		gl.get_program_resource_iv(unwrap(), GL_PROGRAM_INPUT, i, input_output_properties.size(), input_output_properties.data(),
								   input_output_properties.size(), NULL, values.data());

		std::string input_name_buffer(name_length, '\0');
		gl.get_program_resource_name(unwrap(), GL_PROGRAM_INPUT, i, input_name_buffer.size(), NULL, input_name_buffer.data());
		m_inputs.insert({
			static_cast<unsigned int>(location),
			{std::move(input_name_buffer), static_cast<glsl_type>(var_type), array_size},
		});
	}
}

void tr::shader::find_outputs(const gl_api& gl)
{
	int inputs{0};
	gl.get_program_interface_iv(unwrap(), GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &inputs);
	for (int i = 0; i < inputs; ++i) {
		std::array<int, input_output_properties.size()> values;
		const auto& [var_type, array_size, name_length, location]{values};
		gl.get_program_resource_iv(unwrap(), GL_PROGRAM_OUTPUT, i, input_output_properties.size(), input_output_properties.data(),
								   input_output_properties.size(), NULL, values.data());

		std::string output_name_buffer(name_length, '\0');
		gl.get_program_resource_name(unwrap(), GL_PROGRAM_OUTPUT, i, output_name_buffer.size(), NULL, output_name_buffer.data());
		if (!output_name_buffer.starts_with("gl_")) {
			m_outputs.insert({
				static_cast<unsigned int>(location),
				{std::move(output_name_buffer), static_cast<glsl_type>(var_type), array_size},
			});
		}
	}
}

/// Asserts that a shader uniform exists and is of the correct type.
#define TR_ASSERT_SHADER_UNIFORM(target_type)                                                                                              \
	do {                                                                                                                                   \
		TR_ASSERT(valid(), "Tried to set  uniform on a shader in an invalid state.");                                                      \
		const opt_ref<glsl_variable> uniform{try_get(m_uniforms, index)};                                                                  \
		TR_ASSERT(uniform.has_ref(), "Tried to set uniform with invalid index '{}' in shader {}.", index, *this);                          \
		TR_ASSERT(uniform->type == as_glsl_type<target_type> && uniform->array_size == 1,                                                  \
				  "Tried to set uniform with signature '{}' in shader {} with a value of type '{}'.", *uniform, *this,                     \
				  as_glsl_type<target_type>);                                                                                              \
	} while (0)

/// Asserts that a shader array uniform exists and is of the correct type.
#define TR_ASSERT_SHADER_ARRAY_UNIFORM(target_type)                                                                                        \
	do {                                                                                                                                   \
		TR_ASSERT(valid(), "Tried to set a uniform on a shader in an invalid state.");                                                     \
		const opt_ref<glsl_variable> uniform{try_get(m_uniforms, index)};                                                                  \
		TR_ASSERT(uniform.has_ref(), "Tried to set uniform with invalid index '{}' in shader {}.", index, *this);                          \
		TR_ASSERT(uniform->type == as_glsl_type<target_type> && uniform->array_size == int(value.size()),                                  \
				  "Tried to set uniform with signature '{}' in shader {} with a value of type '{}[{}]'.", *uniform, *this,                 \
				  as_glsl_type<target_type>, value.size());                                                                                \
	} while (0)

#else
#define TR_ASSERT_SHADER_UNIFORM(target_type) void(0)
#define TR_ASSERT_SHADER_ARRAY_UNIFORM(target_type) void(0)
#endif

void tr::shader::deleter::operator()(unsigned int program) const
{
	context->gl().delete_program(program);
#ifdef TR_ENABLE_GL_CHECKS
	context->registry().unregister_shader(id);
#endif
}

//

tr::graphics_context& tr::shader::context() const
{
	TR_ASSERT(valid(), "Tried to get context of a shader in an invalid state.");

	return m_program.get_deleter().context;
}

//

void tr::shader::set_uniform(int index, bool value)
{
	TR_ASSERT_SHADER_UNIFORM(bool);

	context().gl().set_program_uniform_1i(unwrap(), index, value);
}

void tr::shader::set_uniform(int index, int value)
{
	TR_ASSERT_SHADER_UNIFORM(int);

	context().gl().set_program_uniform_1i(unwrap(), index, value);
}

void tr::shader::set_uniform(int index, std::span<const int> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(int);

	context().gl().set_program_uniform_1iv(unwrap(), index, value.size(), value.data());
}

void tr::shader::set_uniform(int index, glm::ivec2 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::ivec2);

	context().gl().set_program_uniform_2i(unwrap(), index, value.x, value.y);
}

void tr::shader::set_uniform(int index, std::span<const glm::ivec2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::ivec2);

	context().gl().set_program_uniform_2iv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, glm::ivec3 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::ivec3);

	context().gl().set_program_uniform_3i(unwrap(), index, value.x, value.y, value.z);
}

void tr::shader::set_uniform(int index, std::span<const glm::ivec3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::ivec3);

	context().gl().set_program_uniform_3iv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, glm::ivec4 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::ivec4);

	context().gl().set_program_uniform_4i(unwrap(), index, value.x, value.y, value.z, value.w);
}

void tr::shader::set_uniform(int index, std::span<const glm::ivec4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::ivec4);

	context().gl().set_program_uniform_4iv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, unsigned int value)
{
	TR_ASSERT_SHADER_UNIFORM(unsigned int);

	context().gl().set_program_uniform_1ui(unwrap(), index, value);
}

void tr::shader::set_uniform(int index, std::span<const unsigned int> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(unsigned int);

	context().gl().set_program_uniform_1uiv(unwrap(), index, value.size(), value.data());
}

void tr::shader::set_uniform(int index, glm::uvec2 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::uvec2);

	context().gl().set_program_uniform_2ui(unwrap(), index, value.x, value.y);
}

void tr::shader::set_uniform(int index, std::span<const glm::uvec2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::uvec2);

	context().gl().set_program_uniform_2uiv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, glm::uvec3 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::uvec3);

	context().gl().set_program_uniform_3ui(unwrap(), index, value.x, value.y, value.z);
}

void tr::shader::set_uniform(int index, std::span<const glm::uvec3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::uvec3);

	context().gl().set_program_uniform_3uiv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, glm::uvec4 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::uvec4);

	context().gl().set_program_uniform_4ui(unwrap(), index, value.x, value.y, value.z, value.w);
}

void tr::shader::set_uniform(int index, std::span<const glm::uvec4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::uvec4);

	context().gl().set_program_uniform_4uiv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, float value)
{
	TR_ASSERT_SHADER_UNIFORM(float);

	context().gl().set_program_uniform_1f(unwrap(), index, value);
}

void tr::shader::set_uniform(int index, std::span<const float> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(float);

	context().gl().set_program_uniform_1fv(unwrap(), index, value.size(), value.data());
}

void tr::shader::set_uniform(int index, glm::vec2 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::vec2);

	context().gl().set_program_uniform_2f(unwrap(), index, value.x, value.y);
}

void tr::shader::set_uniform(int index, std::span<const glm::vec2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::vec2);

	context().gl().set_program_uniform_2fv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, glm::vec3 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::vec3);

	context().gl().set_program_uniform_3f(unwrap(), index, value.x, value.y, value.z);
}

void tr::shader::set_uniform(int index, std::span<const glm::vec3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::vec3);

	context().gl().set_program_uniform_3fv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, glm::vec4 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::vec4);

	context().gl().set_program_uniform_4f(unwrap(), index, value.x, value.y, value.z, value.w);
}

void tr::shader::set_uniform(int index, std::span<const glm::vec4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::vec4);

	context().gl().set_program_uniform_4fv(unwrap(), index, value.size(), value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat2& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat2);

	context().gl().set_program_uniform_matrix2fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat2);

	context().gl().set_program_uniform_matrix2fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat3& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat3);

	context().gl().set_program_uniform_matrix3fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat3);

	context().gl().set_program_uniform_matrix3fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat4& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat4);

	context().gl().set_program_uniform_matrix4fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat4);

	context().gl().set_program_uniform_matrix4fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat2x3& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat2x3);

	context().gl().set_program_uniform_matrix2x3fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat2x3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat2x3);

	context().gl().set_program_uniform_matrix2x3fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat2x4& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat2x4);

	context().gl().set_program_uniform_matrix2x4fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat2x4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat2x4);

	context().gl().set_program_uniform_matrix2x4fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat3x2& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat3x2);

	context().gl().set_program_uniform_matrix3x2fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat3x2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat3x2);

	context().gl().set_program_uniform_matrix3x2fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat3x4& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat3x4);

	context().gl().set_program_uniform_matrix3x4fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat3x4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat3x4);

	context().gl().set_program_uniform_matrix3x4fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat4x2& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat4x2);

	context().gl().set_program_uniform_matrix4x2fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat4x2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat4x2);

	context().gl().set_program_uniform_matrix4x2fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, const glm::mat4x3& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat4x3);

	context().gl().set_program_uniform_matrix4x3fv(unwrap(), index, 1, false, value_ptr(value));
}

void tr::shader::set_uniform(int index, std::span<const glm::mat4x3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat4x3);

	context().gl().set_program_uniform_matrix4x3fv(unwrap(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader::set_uniform(int index, texture_view texture)
{
	TR_ASSERT(valid(), "Tried to set a uniform on a shader in an invalid state.");
#ifdef TR_ENABLE_GL_CHECKS
	const auto uniform_it{m_uniforms.find(index)};
	TR_ASSERT(uniform_it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader {}.", index, *this);
	TR_ASSERT(uniform_it->second.type == glsl_type::sampler2D && uniform_it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader {} with a value of type 'sampler2D'.", uniform_it->second, *this);
#endif

	auto unit_it{m_texture_units.find(index)};
	if (unit_it == m_texture_units.end()) {
		unit_it = m_texture_units.insert({index, texture_unit{context()}}).first;
		context().gl().set_program_uniform_1i(unwrap(), index, unit_it->second.id());
	}
	unit_it->second.set(texture);
}

void tr::shader::set_storage_buffer(unsigned int index, unsigned int buffer_id, std::intptr_t buffer_size)
{
	TR_ASSERT(valid(), "Tried to set a storage buffer on a shader in an invalid state.");

	context().gl().bind_buffer_range(GL_SHADER_STORAGE_BUFFER, index, buffer_id, 0, buffer_size);
}

void tr::shader::set_storage_buffer(unsigned int index, basic_shader_buffer& buffer)
{
	set_storage_buffer(index, buffer.id(), buffer.header_size() + buffer.array_size());
}

void tr::shader::set_uniform_buffer(unsigned int index, unsigned int buffer_id)
{
	TR_ASSERT(valid(), "Tried to set a uniform buffer on a shader in an invalid state.");

	context().gl().bind_buffer_base(GL_UNIFORM_BUFFER, index, buffer_id);
}

void tr::shader::set_uniform_buffer(unsigned int index, const basic_uniform_buffer& buffer)
{
	set_uniform_buffer(index, buffer.id());
}

//

bool tr::shader::valid() const
{
	return m_program.has_value();
}

//

void tr::shader::set_label(std::string_view label)
{
	TR_ASSERT(valid(), "Tried to set the label of a shader in an invalid state.");

	context().gl().set_object_label(GL_PROGRAM, unwrap(), label.size(), label.data());
}

std::string tr::shader::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a shader in an invalid state.");

	const gl_api& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_PROGRAM, unwrap(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_PROGRAM, unwrap(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

//

unsigned int tr::shader::unwrap() const
{
	return m_program.get();
}

#ifdef TR_ENABLE_GL_CHECKS
tr::graphics_object_id tr::shader::id() const
{
	return m_program.get_deleter().id;
}

const boost::unordered_flat_map<unsigned int, tr::glsl_variable>& tr::shader::inputs() const
{
	return m_inputs;
}

const boost::unordered_flat_map<unsigned int, tr::glsl_variable>& tr::shader::outputs() const
{
	return m_outputs;
}
#endif

//

tr::vertex_shader::vertex_shader(graphics_context& context, zstring_view source)
	: shader{context, source, GL_VERTEX_SHADER}
{
}

tr::vertex_shader tr::load_vertex_shader(graphics_context& context, const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		vertex_shader shader{context, std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
		shader.set_label(path.filename().string());
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

tr::fragment_shader::fragment_shader(graphics_context& context, zstring_view source)
	: shader{context, source, GL_FRAGMENT_SHADER}
{
}

tr::fragment_shader tr::load_fragment_shader(graphics_context& context, const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		fragment_shader shader{context, std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
		shader.set_label(path.filename().string());
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