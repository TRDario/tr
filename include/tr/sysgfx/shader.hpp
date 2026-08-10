/// @file
/// @brief Provides shader classes.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"
#include "texture_unit.hpp"

#ifdef TR_ENABLE_GL_CHECKS
#include "gl_checks.hpp"
#endif

namespace tr
{
	class basic_shader_buffer;
	class basic_uniform_buffer;
	struct gl_api;
	class graphics_context;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// Error thrown when shader loading fails.
	class shader_load_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the file that failed to load.
		/// @param detail Shader loading error details.
		shader_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Shader loading error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Details of the error.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	//

	/// Base GPU shader program class.
	class shader_base
	{
	  public:
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the shader is on.
		/// @return Reference to the graphics context the shader is on.
		graphics_context& context() const;

		/// @}
		/// @name Uniforms
		/// @{

		/// Sets a boolean uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, bool value);

		/// Sets an integer uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, int value);

		/// Sets an integer array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const int> value);

		/// Sets an ivec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec2 value);

		/// Sets an ivec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec2> value);

		/// Sets an ivec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec3 value);

		/// Sets an ivec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec3> value);

		/// Sets an ivec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec4 value);

		/// Sets an ivec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec4> value);

		/// Sets an unsigned integer uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, unsigned int value);

		/// Sets an unsigned integer array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const unsigned int> value);

		/// Sets a uvec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec2 value);

		/// Sets a uvec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec2> value);

		/// Sets a uvec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec3 value);

		/// Sets a uvec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec3> value);

		/// Sets a uvec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec4 value);

		/// Sets a uvec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec4> value);

		/// Sets a float uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, float value);

		/// Sets a float array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const float> value);

		/// Sets a vec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec2 value);

		/// Sets a vec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec2> value);

		/// Sets a vec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec3 value);

		/// Sets a vec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec3> value);

		/// Sets a vec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec4 value);

		/// Sets a vec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec4> value);

		/// Sets a mat2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2& value);

		/// Sets a mat2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2> value);

		/// Sets a mat3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3& value);

		/// Sets a mat3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3> value);

		/// Sets a mat4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4& value);

		/// Sets a mat4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4> value);

		/// Sets a mat2x3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2x3& value);

		/// Sets a mat2x3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2x3> value);

		/// Sets a mat2x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2x4& value);

		/// Sets a mat2x4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2x4> value);

		/// Sets a mat3x2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3x2& value);

		/// Sets a mat3x2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3x2> value);

		/// Sets a mat3x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3x4& value);

		/// Sets a mat3x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3x4> value);

		/// Sets a mat4x2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4x2& value);

		/// Sets a mat4x2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4x2> value);

		/// Sets a mat4x3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4x3& value);

		/// Sets a mat4x3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4x3> value);

		/// Sets a texture sampler uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, texture_view texture);

		/// @}
		/// @name Storage buffers
		/// @{

		/// Sets a shader storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		void set_storage_buffer(unsigned int index, basic_shader_buffer& buffer);

		/// Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		void set_uniform_buffer(unsigned int index, const basic_uniform_buffer& buffer);

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the shader.
		/// @param label Debug label of the shader.
		void set_label(std::string_view label);

		/// Gets the debug label of the shader.
		/// @return Debug label of the shader.
		std::string label() const;

		/// @}

	  protected:
		/// Shader program deleter.
		struct deleter
		{
			/// Reference to the graphics context the shader is on.
			graphics_context& context;

			//

			/// Deletes the shader program.
			/// @param id OpenGL shader program ID.
			void operator()(unsigned int id) const;
		};

		/// Handle to the OpenGL program.
		handle<unsigned int, 0, deleter> m_program;

		/// Texture units allocated to this shader.
		boost::unordered_flat_map<int, texture_unit> m_texture_units;

#ifdef TR_ENABLE_GL_CHECKS
		/// List of non-block uniforms obtained by introspection.
		boost::unordered_flat_map<unsigned int, glsl_variable> m_uniforms;

		/// List of input variables obtained by introspection.
		boost::unordered_flat_map<unsigned int, glsl_variable> m_inputs;

		/// List of output variables obtained by introspection.
		boost::unordered_flat_map<unsigned int, glsl_variable> m_outputs;
#endif

		//

		/// Constructs a shader.
		/// @param context Graphics context to create the shader on.
		/// @param source Shader source code.
		/// @param type Shader type.
		shader_base(graphics_context& context, zstring_view source, unsigned int type);

		//

#ifdef TR_ENABLE_GL_CHECKS
		/// Finds the uniforms of the shader using introspection.
		/// @param gl Structure holding the OpenGL API.
		void find_uniforms(const gl_api& gl);

		/// Finds the input variables of the shader using introspection.
		/// @param gl Structure holding the OpenGL API.
		void find_inputs(const gl_api& gl);

		/// Finds the output variables of the shader using introspection.
		/// @param gl Structure holding the OpenGL API.
		void find_outputs(const gl_api& gl);
#endif

		//

		// Accesses the raw OpenGL shader program ID.
		friend class shader_pipeline;
	};

	//

	/// GPU vertex shader program.
	class vertex_shader : public shader_base
	{
	  public:
		/// Creates a vertex shader from source code.
		/// @param context Graphics context to create the vertex shader on.
		/// @param source Vertex shader source code.
		/// @exception shader_load_error If loading the shader failed.
		explicit vertex_shader(graphics_context& context, zstring_view source);
	};

	/// GPU fragment shader program.
	class fragment_shader : public shader_base
	{
	  public:
		/// Creates a fragment shader from source code.
		/// @param context Graphics context to create the fragment shader on.
		/// @param source Fragment shader source code.
		/// @exception shader_load_error If loading the shader failed.
		explicit fragment_shader(graphics_context& context, zstring_view source);
	};

	/// @name Shaders
	/// @{

	/// Loads a vertex shader from file.
	/// @param context Graphics context to create the vertex shader on.
	/// @param path Path to the shader source code file.
	/// @exception shader_load_error If loading the shader failed.
	/// @return Loaded vertex shader.
	vertex_shader load_vertex_shader(graphics_context& context, const std::filesystem::path& path);

	/// Loads a fragment shader from file.
	/// @param context Graphics context to create the fragment shader on.
	/// @param path Path to the shader source code file.
	/// @exception shader_load_error If loading the shader failed.
	/// @return Loaded frament shader.
	fragment_shader load_fragment_shader(graphics_context& context, const std::filesystem::path& path);

	/// @}
} // namespace tr