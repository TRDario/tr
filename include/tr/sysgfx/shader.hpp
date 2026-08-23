/// @file
/// @brief Provides `tr::vertex_shader`, `tr::fragment_shader`, and related functionality.
/// @details For an explanation of shaders, see the description of `tr::shader`.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/reference.hpp"
#include "../utility/zstring_view.hpp"
#include "texture_unit.hpp"

#ifdef TR_ENABLE_GL_CHECKS
#include "gl_checks.hpp"
#endif

namespace tr
{
	// clang-format off
	class basic_shader_buffer;
	class basic_uniform_buffer;
	struct gl_api;
	class graphics_context;
	template <typename Element> class shader_array;
	template <typename Header, typename ArrayElement> class shader_buffer;
	class texture_view;
	template <typename Object> class uniform_buffer;
	// clang-format on
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
		/// @param details Shader loading error details.
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

	/// Base shader program class.
	/// @details
	/// Shader programs are small programs run on the GPU during a step of the graphics pipeline. tr allows the creation and use of custom
	/// vertex and fragment shader programs written in the OpenGL shading language (GLSL).
	///
	/// Objects of this type may not be instantiated directly. Rather, vertex and fragment shaders are subclassed as `tr::vertex_shader` and
	/// `tr::fragment_shader` respectively.
	///
	/// Strictly speaking, shader objects are only containers for the underlying shader programs. This means, for example, that setting a
	/// shader on a shader pipeline object does not set the literal shader object at a specific location in memory, but rather the value it
	/// contains. If the value is moved to a different shader object, that value will still be set on the pipeline. If the object is
	/// overriden with a new value, the old value is destroyed and the pipeline reverts to an incomplete state.
	///
	/// Every shader object is associated with a graphics context and cannot outlive its parent context.
	///
	/// Shader objects are movable, but not copyable. A moved-from instance of a shader object is left in a special 'invalid' state. Invalid
	/// shader objects may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// Shader objects may be labeled and are formattable. Example format output: `"My %shader" (GID: 5)`. The GID is the OpenGL ID of the
	/// shader program.
	class shader
	{
	  public:
		/// @name Constructors
		/// @{

		/// @cond __hidden
		/// Constructs a shader.
		/// @param context Graphics context to create the shader on.
		/// @param source Shader GLSL source code.
		/// @param type Shader type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
		shader(graphics_context& context, zstring_view source, unsigned int type);
		/// @endcond

		/// @}
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
		/// @param texture Texture to set.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, texture_view texture);

		/// @}
		/// @name Storage buffers
		/// @{

		/// Sets a shader storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		void set_storage_buffer(unsigned int index, basic_shader_buffer& buffer);

		/// Sets a shader storage buffer.
		/// @tparam Header Type of the header object stored at the front of the buffer.
		/// @tparam ArrayElement Type of the buffer dynamic array elements.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		template <typename Header, typename ArrayElement>
		void set_storage_buffer(unsigned int index, shader_buffer<Header, ArrayElement>& buffer);

		/// Sets a shader storage buffer.
		/// @tparam Element Type of the array elements.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		template <typename Element>
		void set_storage_buffer(unsigned int index, shader_array<Element>& buffer);

		/// Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		void set_uniform_buffer(unsigned int index, const basic_uniform_buffer& buffer);

		/// Sets a uniform storage buffer.
		/// @tparam Object Objet contained in the buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		template <typename Object>
		void set_uniform_buffer(unsigned int index, const uniform_buffer<Object>& buffer);

		/// @}
		/// @name State
		/// @{

		/// Gets whether the shader is in a valid state.
		/// @return `true` if the shader is in a valid state, `false` if it is in an invalid state.
		bool valid() const;

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

		/// @cond __hidden
		/// Gets the OpenGL shader program ID.
		/// @return OpenGL shader program ID.
		unsigned int gid() const;

#ifdef TR_ENABLE_GL_CHECKS
		/// Gets the unique ID of the shader.
		/// @return Unique ID of the shader.
		unsigned int tid() const;

		/// Gets the shader's inputs.
		/// @return Map of shader inputs.
		const boost::unordered_flat_map<unsigned int, glsl_variable>& inputs() const;

		/// Gets the shader's outputs.
		/// @return Map of shader outputs.
		const boost::unordered_flat_map<unsigned int, glsl_variable>& outputs() const;
#endif
		/// @endcond

	  private:
		/// Shader program deleter.
		struct deleter
		{
			/// Reference to the graphics context the shader is on.
			ref<graphics_context> context;

#ifdef TR_ENABLE_GL_CHECKS
			/// Unique object ID of the shader.
			unsigned int tid{0};
#endif

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

		/// Sets a shader storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer_id ID of the buffer to set.
		/// @param buffer_size Size of the buffer.
		void set_storage_buffer(unsigned int index, unsigned int buffer_id, std::intptr_t buffer_size);

		/// Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer_id ID of the buffer to set.
		void set_uniform_buffer(unsigned int index, unsigned int buffer_id);
	};

	//

	/// Vertex shader program.
	/// @details
	/// A vertex shader is a shader program that transforms vertices passed to the drawing operation and outputs data passed along to the
	/// fragment shader.
	///
	/// Everything brought up in the description of `tr::shader` applies to this class as well.
	class vertex_shader : public shader
	{
	  public:
		/// Creates a vertex shader from source code.
		/// @param context Graphics context to create the vertex shader on.
		/// @param source Vertex shader GLSL source code.
		/// @exception shader_load_error If loading the shader failed.
		explicit vertex_shader(graphics_context& context, zstring_view source);
	};

	/// Fragment shader program.
	/// @details
	/// A vertex shader is a shader program that calculates the final color of a fragment (in most cases a pixel) given data output by the
	/// vertex shader and interpolated along a primitive.
	///
	/// Everything brought up in the description of `tr::shader` applies to this class as well.
	class fragment_shader : public shader
	{
	  public:
		/// Creates a fragment shader from source code.
		/// @param context Graphics context to create the fragment shader on.
		/// @param source Fragment shader GLSL source code.
		/// @exception shader_load_error If loading the shader failed.
		explicit fragment_shader(graphics_context& context, zstring_view source);
	};

	/// @name Shaders
	/// @{

	/// Loads a vertex shader from file.
	/// @param context Graphics context to create the vertex shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @exception shader_load_error If loading the shader failed.
	/// @return Loaded vertex shader.
	vertex_shader load_vertex_shader(graphics_context& context, const std::filesystem::path& path);

	/// Loads a fragment shader from file.
	/// @param context Graphics context to create the fragment shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @exception shader_load_error If loading the shader failed.
	/// @return Loaded frament shader.
	fragment_shader load_fragment_shader(graphics_context& context, const std::filesystem::path& path);

	/// @}
} // namespace tr