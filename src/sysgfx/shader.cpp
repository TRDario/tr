///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements shader.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/shader.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/uniform_buffer.hpp"
#include "../../include/tr/utility/hash_map.hpp"
#include "../../include/tr/utility/iostream.hpp"

//////////////////////////////////////////////////////////// SHADER LOAD ERROR ////////////////////////////////////////////////////////////

tr::shader_load_error::shader_load_error(std::string_view path, std::string&& details)
	: m_description{TR_FMT::format("Failed to load bitmap from '{}'", path)}, m_details{std::move(details)}
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

/////////////////////////////////////////////////////////////// TEXTURE UNIT //////////////////////////////////////////////////////////////

tr::shader_base::texture_unit::texture_unit(graphics_context& context, unsigned int program, int index)
	: m_id{context.allocate_texture_unit(), {context}}
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.program_uniform_1i(program, index, m_id.get());
}

void tr::shader_base::texture_unit::deleter::operator()(unsigned int unit) const
{
	context.free_texture_unit(unit);
}

void tr::shader_base::texture_unit::set(texture_ref texture)
{
	m_id.get_deleter().context.set_texture_unit(m_id.get(), std::move(texture));
}

////////////////////////////////////////////////////////////////// SHADER /////////////////////////////////////////////////////////////////

tr::shader_base::shader_base(graphics_context& context, zstring_view source, unsigned int type)
	: m_program{context.make_current_and_return_functions().create_shader_program_v(type, 1, (const char**)&source), {context}}
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	int linked;
	gl.get_program_iv(m_program.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		int info_log_buffer_size;
		gl.get_program_iv(m_program.get(), GL_INFO_LOG_LENGTH, &info_log_buffer_size);
		std::string info_log_buffer(info_log_buffer_size, '\0');
		gl.get_program_info_log(m_program.get(), info_log_buffer_size, nullptr, info_log_buffer.data());
		throw shader_load_error{"(Embedded)", TR_FMT::format("Failed to compile/link a shader\n{}", info_log_buffer)};
	}

#ifdef TR_ENABLE_GL_CHECKS
	find_uniforms(gl);
	find_inputs(gl);
	find_outputs(gl);
#endif
}

#ifdef TR_ENABLE_GL_CHECKS
// Properties queried for uniforms.
static constexpr std::array<unsigned int, 5> uniform_properties{GL_BLOCK_INDEX, GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};
// Properties queried for inputs and outputs.
static constexpr std::array<unsigned int, 4> input_output_properties{GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH, GL_LOCATION};

void tr::shader_base::find_uniforms(const graphics_context::functions& gl)
{
	int uniforms{0};
	gl.get_program_interface_iv(m_program.get(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniforms);
	for (int i = 0; i < uniforms; ++i) {
		std::array<int, uniform_properties.size()> values;
		const auto& [block_index, var_type, array_size, name_length, location]{values};
		gl.get_program_resource_iv(m_program.get(), GL_UNIFORM, i, uniform_properties.size(), uniform_properties.data(),
								   uniform_properties.size(), NULL, values.data());

		if (block_index != -1) {
			continue;
		}

		std::string uniform_name_buffer(name_length, '\0');
		gl.get_program_resource_name(m_program.get(), GL_UNIFORM, i, uniform_name_buffer.size(), NULL, uniform_name_buffer.data());
		m_uniforms.insert({(unsigned int)(location), {std::move(uniform_name_buffer), glsl_type(var_type), array_size}});
	}
}

void tr::shader_base::find_inputs(const graphics_context::functions& gl)
{
	int inputs{0};
	gl.get_program_interface_iv(m_program.get(), GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputs);
	for (int i = 0; i < inputs; ++i) {
		std::array<int, input_output_properties.size()> values;
		const auto& [var_type, array_size, name_length, location]{values};
		gl.get_program_resource_iv(m_program.get(), GL_PROGRAM_INPUT, i, input_output_properties.size(), input_output_properties.data(),
								   input_output_properties.size(), NULL, values.data());

		std::string input_name_buffer(name_length, '\0');
		gl.get_program_resource_name(m_program.get(), GL_PROGRAM_INPUT, i, input_name_buffer.size(), NULL, input_name_buffer.data());
		m_inputs.insert({(unsigned int)(location), {std::move(input_name_buffer), glsl_type(var_type), array_size}});
	}
}

void tr::shader_base::find_outputs(const graphics_context::functions& gl)
{
	int inputs{0};
	gl.get_program_interface_iv(m_program.get(), GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &inputs);
	for (int i = 0; i < inputs; ++i) {
		std::array<int, input_output_properties.size()> values;
		const auto& [var_type, array_size, name_length, location]{values};
		gl.get_program_resource_iv(m_program.get(), GL_PROGRAM_OUTPUT, i, input_output_properties.size(), input_output_properties.data(),
								   input_output_properties.size(), NULL, values.data());

		std::string output_name_buffer(name_length, '\0');
		gl.get_program_resource_name(m_program.get(), GL_PROGRAM_OUTPUT, i, output_name_buffer.size(), NULL, output_name_buffer.data());
		if (!output_name_buffer.starts_with("gl_")) {
			m_outputs.insert({(unsigned int)(location), {std::move(output_name_buffer), glsl_type(var_type), array_size}});
		}
	}
}

// Asserts that a shader uniform exists and is of the correct type.
#define TR_ASSERT_SHADER_UNIFORM(target_type)                                                                                              \
	do {                                                                                                                                   \
		const opt_ref<glsl_variable> uniform{try_get(m_uniforms, index)};                                                                  \
		TR_ASSERT(uniform.has_ref(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());                      \
		TR_ASSERT(uniform->type == as_glsl_type<target_type> && uniform->array_size == 1,                                                  \
				  "Tried to set uniform with signature '{}' in shader '{}' with a value of type '{}'.", *uniform, label(),                 \
				  as_glsl_type<target_type>);                                                                                              \
	} while (0)
// Asserts that a shader array uniform exists and is of the correct type.
#define TR_ASSERT_SHADER_ARRAY_UNIFORM(target_type)                                                                                        \
	do {                                                                                                                                   \
		const opt_ref<glsl_variable> uniform{try_get(m_uniforms, index)};                                                                  \
		TR_ASSERT(uniform.has_ref(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());                      \
		TR_ASSERT(uniform->type == as_glsl_type<target_type> && uniform->array_size == int(value.size()),                                  \
				  "Tried to set uniform with signature '{}' in shader '{}' with a value of type '{}[{}]'.", *uniform, label(),             \
				  as_glsl_type<target_type>, value.size());                                                                                \
	} while (0)

#else
#define TR_ASSERT_SHADER_UNIFORM(target_type) void(0)
#define TR_ASSERT_SHADER_ARRAY_UNIFORM(target_type) void(0)
#endif

void tr::shader_base::deleter::operator()(unsigned int id) const
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.delete_program(id);
}

//

tr::graphics_context& tr::shader_base::context() const
{
	return m_program.get_deleter().context;
}

//

void tr::shader_base::set_uniform(int index, bool value)
{
	TR_ASSERT_SHADER_UNIFORM(bool);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1i(m_program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, int value)
{
	TR_ASSERT_SHADER_UNIFORM(int);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1i(m_program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const int> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(int);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1iv(m_program.get(), index, value.size(), value.data());
}

void tr::shader_base::set_uniform(int index, glm::ivec2 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::ivec2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_2i(m_program.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::ivec2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_2iv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::ivec3 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::ivec3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_3i(m_program.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::ivec3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_3iv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::ivec4 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::ivec4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_4i(m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::ivec4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::ivec4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_4iv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, unsigned int value)
{
	TR_ASSERT_SHADER_UNIFORM(unsigned int);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1ui(m_program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const unsigned int> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(unsigned int);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1uiv(m_program.get(), index, value.size(), value.data());
}

void tr::shader_base::set_uniform(int index, glm::uvec2 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::uvec2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_2ui(m_program.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::uvec2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_2uiv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::uvec3 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::uvec3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_3ui(m_program.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::uvec3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_3uiv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::uvec4 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::uvec4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_4ui(m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::uvec4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::uvec4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_4uiv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, float value)
{
	TR_ASSERT_SHADER_UNIFORM(float);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1f(m_program.get(), index, value);
}

void tr::shader_base::set_uniform(int index, std::span<const float> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(float);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_1fv(m_program.get(), index, value.size(), value.data());
}

void tr::shader_base::set_uniform(int index, glm::vec2 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::vec2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_2f(m_program.get(), index, value.x, value.y);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::vec2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_2fv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::vec3 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::vec3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_3f(m_program.get(), index, value.x, value.y, value.z);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::vec3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_3fv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, glm::vec4 value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::vec4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_4f(m_program.get(), index, value.x, value.y, value.z, value.w);
}

void tr::shader_base::set_uniform(int index, std::span<const glm::vec4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::vec4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_4fv(m_program.get(), index, value.size(), value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_2fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_2fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_3fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_3fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_4fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_4fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2x3& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat2x3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_2x3fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2x3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat2x3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_2x3fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat2x4& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat2x4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_2x4fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat2x4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat2x4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_2x4fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3x2& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat3x2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_3x2fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3x2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat3x2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_3x2fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat3x4& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat3x4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_3x4fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat3x4> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat3x4);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_3x4fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4x2& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat4x2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_4x2fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4x2> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat4x2);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_4x2fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, const glm::mat4x3& value)
{
	TR_ASSERT_SHADER_UNIFORM(glm::mat4x3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_4x3fv(m_program.get(), index, 1, false, value_ptr(value));
}

void tr::shader_base::set_uniform(int index, std::span<const glm::mat4x3> value)
{
	TR_ASSERT_SHADER_ARRAY_UNIFORM(glm::mat4x3);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.program_uniform_matrix_4x3fv(m_program.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::shader_base::set_uniform(int index, texture_ref texture)
{
#ifdef TR_ENABLE_GL_CHECKS
	const auto uniform_it{m_uniforms.find(index)};
	TR_ASSERT(uniform_it != m_uniforms.end(), "Tried to set uniform with invalid index '{}' in shader '{}'.", index, label());
	TR_ASSERT(uniform_it->second.type == glsl_type::sampler2D && uniform_it->second.array_size == 1,
			  "Tried to set uniform with signature '{}' in shader '{}' with a value of type 'sampler2D'.", uniform_it->second, label());
#endif

	auto unit_it{m_texture_units.find(index)};
	if (unit_it == m_texture_units.end()) {
		unit_it = m_texture_units.insert({index, texture_unit{context(), m_program.get(), index}}).first;
	}
	unit_it->second.set(std::move(texture));
}

void tr::shader_base::set_storage_buffer(unsigned int index, basic_shader_buffer& buffer)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.bind_buffer_range(GL_SHADER_STORAGE_BUFFER, index, buffer.id(), 0, buffer.header_size() + buffer.array_size());
}

void tr::shader_base::set_uniform_buffer(unsigned int index, const basic_uniform_buffer& buffer)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.bind_buffer_base(GL_UNIFORM_BUFFER, index, buffer.id());
}

#ifdef TR_ENABLE_ASSERTS
void tr::shader_base::set_label(std::string_view label)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.object_label(GL_PROGRAM, m_program.get(), label.size(), label.data());
}

std::string tr::shader_base::label() const
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	int label_length;
	gl.get_object_label(GL_PROGRAM, m_program.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_PROGRAM, m_program.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}
#endif

////////////////////////////////////////////////////////////// SHADER CLASSES /////////////////////////////////////////////////////////////

tr::vertex_shader::vertex_shader(graphics_context& context, zstring_view source)
	: shader_base{context, source, GL_VERTEX_SHADER}
{
}

tr::vertex_shader tr::load_vertex_shader(graphics_context& context, const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		vertex_shader shader{context, std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
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

tr::fragment_shader::fragment_shader(graphics_context& context, zstring_view source)
	: shader_base{context, source, GL_FRAGMENT_SHADER}
{
}

tr::fragment_shader tr::load_fragment_shader(graphics_context& context, const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		fragment_shader shader{context, std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}}};
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