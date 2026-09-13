/// @file
/// @brief Provides `tr::vertex_shader`, `tr::fragment_shader`, and related functionality.
/// @details For an explanation of shaders, see the description of `tr::shader`.

#pragma once
#include <tr/sysgfx/internal/texture_unit.hpp>
#include <tr/sysgfx/shader_array.hpp>
#include <tr/sysgfx/shader_buffer.hpp>
#include <tr/sysgfx/uniform_buffer.hpp>
#include <tr/utility/exception.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/zstring_view.hpp>

#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include <tr/sysgfx/internal/glsl_variable.hpp>
#include <tr/sysgfx/internal/graphics_object_registry.hpp>
#endif

namespace tr
{
	class graphics_context;
	class texture_view;
	namespace internal
	{
		struct opengl;
	}
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
		[[nodiscard]] shader_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Shader loading error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

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
	/// Shader objects may be labeled and are formattable. Example format output: `"My %shader" (OpenGL ID: 5)`.
	class shader
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors
		/// @{

		/// Constructs a shader.
		/// @param context Graphics context to create the shader on.
		/// @param source Shader GLSL source code.
		/// @param type Shader type (`GL_VERTEX_SHADER` or `GL_FRAGMENT_SHADER`).
		/// @exception shader_load_error If loading the shader failed.
		[[nodiscard]] shader(graphics_context& context, zstring_view source, unsigned int type);

		/// @}
		/// @endcond
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the shader is on.
		/// @return Reference to the graphics context the shader is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name Uniforms
		/// @{

		/// Sets a boolean uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, bool value) noexcept;

		/// Sets an integer uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, int value) noexcept;

		/// Sets an integer array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const int> value) noexcept;

		/// Sets an ivec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec2 value) noexcept;

		/// Sets an ivec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec2> value) noexcept;

		/// Sets an ivec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec3 value) noexcept;

		/// Sets an ivec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec3> value) noexcept;

		/// Sets an ivec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec4 value) noexcept;

		/// Sets an ivec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec4> value) noexcept;

		/// Sets an unsigned integer uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, unsigned int value) noexcept;

		/// Sets an unsigned integer array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const unsigned int> value) noexcept;

		/// Sets a uvec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec2 value) noexcept;

		/// Sets a uvec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec2> value) noexcept;

		/// Sets a uvec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec3 value) noexcept;

		/// Sets a uvec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec3> value) noexcept;

		/// Sets a uvec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec4 value) noexcept;

		/// Sets a uvec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec4> value) noexcept;

		/// Sets a float uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, float value) noexcept;

		/// Sets a float array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const float> value) noexcept;

		/// Sets a vec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec2 value) noexcept;

		/// Sets a vec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec2> value) noexcept;

		/// Sets a vec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec3 value) noexcept;

		/// Sets a vec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec3> value) noexcept;

		/// Sets a vec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec4 value) noexcept;

		/// Sets a vec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec4> value) noexcept;

		/// Sets a mat2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2& value) noexcept;

		/// Sets a mat2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2> value) noexcept;

		/// Sets a mat3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3& value) noexcept;

		/// Sets a mat3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3> value) noexcept;

		/// Sets a mat4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4& value) noexcept;

		/// Sets a mat4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4> value) noexcept;

		/// Sets a mat2x3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2x3& value) noexcept;

		/// Sets a mat2x3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2x3> value) noexcept;

		/// Sets a mat2x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2x4& value) noexcept;

		/// Sets a mat2x4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2x4> value) noexcept;

		/// Sets a mat3x2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3x2& value) noexcept;

		/// Sets a mat3x2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3x2> value) noexcept;

		/// Sets a mat3x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3x4& value) noexcept;

		/// Sets a mat3x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3x4> value) noexcept;

		/// Sets a mat4x2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4x2& value) noexcept;

		/// Sets a mat4x2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4x2> value) noexcept;

		/// Sets a mat4x3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4x3& value) noexcept;

		/// Sets a mat4x3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4x3> value) noexcept;

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
		void set_storage_buffer(unsigned int index, untyped_shader_buffer& buffer) noexcept;

		/// Sets a shader storage buffer.
		/// @tparam Header Type of the header object stored at the front of the buffer.
		/// @tparam ArrayElement Type of the buffer dynamic array elements.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		template <typename Header, typename ArrayElement>
		void set_storage_buffer(unsigned int index, shader_buffer<Header, ArrayElement>& buffer) noexcept
		{
			set_storage_buffer(index, buffer.unwrap(), sizeof(Header) + sizeof(ArrayElement) * buffer.array_size());
		}

		/// Sets a shader storage buffer.
		/// @tparam Element Type of the array elements.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		template <typename Element>
		void set_storage_buffer(unsigned int index, shader_array<Element>& buffer) noexcept
		{
			set_storage_buffer(index, buffer.unwrap(), sizeof(Element) * buffer.size());
		}

		/// Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		void set_uniform_buffer(unsigned int index, const untyped_uniform_buffer& buffer) noexcept;

		/// Sets a uniform storage buffer.
		/// @tparam Object Objet contained in the buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		template <typename Object>
		void set_uniform_buffer(unsigned int index, const uniform_buffer<Object>& buffer) noexcept
		{
			set_uniform_buffer(index, buffer.unwrap());
		}

		/// @}
		/// @name State
		/// @{

		/// Gets whether the shader is in a valid state.
		/// @return `true` if the shader is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the shader.
		/// @param label Debug label of the shader.
		void set_label(std::string_view label) noexcept;

		/// Gets the debug label of the shader.
		/// @return Debug label of the shader.
		[[nodiscard]] std::string label() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability

		/// Unwraps the OpenGL shader program.
		/// @note This does not release the shader program.
		/// @return OpenGL shader program ID.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Gets the unique graphics object ID of the shader.
		/// @return Unique graphics object ID of the shader.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;

		/// Gets the shader's inputs.
		/// @return Map of shader inputs.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& inputs() const noexcept;

		/// Gets the shader's outputs.
		/// @return Map of shader outputs.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& outputs() const noexcept;

		/// @}
		/// @endcond
#endif

	  private:
		/// Shader program deleter.
		struct deleter
		{
			/// Reference to the graphics context the shader is on.
			ref<graphics_context> context;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			/// Handle to the unique graphics object ID of the shader.
			internal::graphics_object_id_handle id{};
#endif

			//

			/// Deletes the shader program.
			/// @param id OpenGL shader program ID.
			void operator()(unsigned int id) const noexcept;
		};

		/// Handle to the OpenGL program.
		handle<unsigned int, 0, deleter> m_program;

		/// Texture units allocated to this shader.
		boost::unordered_flat_map<int, internal::texture_unit> m_texture_units;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// List of non-block uniforms obtained by introspection.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_uniforms;

		/// List of input variables obtained by introspection.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_inputs;

		/// List of output variables obtained by introspection.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_outputs;
#endif

		//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Finds the uniforms of the shader using introspection.
		/// @param gl Structure holding the OpenGL API.
		void find_uniforms(const internal::opengl& gl);

		/// Finds the input variables of the shader using introspection.
		/// @param gl Structure holding the OpenGL API.
		void find_inputs(const internal::opengl& gl);

		/// Finds the output variables of the shader using introspection.
		/// @param gl Structure holding the OpenGL API.
		void find_outputs(const internal::opengl& gl);
#endif

		//

		/// Sets a shader storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer_id ID of the buffer to set.
		/// @param buffer_size Size of the buffer.
		void set_storage_buffer(unsigned int index, unsigned int buffer_id, std::intptr_t buffer_size) noexcept;

		/// Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer_id ID of the buffer to set.
		void set_uniform_buffer(unsigned int index, unsigned int buffer_id) noexcept;
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
		[[nodiscard]] vertex_shader(graphics_context& context, zstring_view source);
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
		[[nodiscard]] fragment_shader(graphics_context& context, zstring_view source);
	};

	/// @name Shaders
	/// @{

	/// Loads a vertex shader from file.
	/// @param context Graphics context to create the vertex shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @exception shader_load_error If loading the shader failed.
	/// @return Loaded vertex shader.
	[[nodiscard]] vertex_shader load_vertex_shader(graphics_context& context, const std::filesystem::path& path);

	/// Loads a fragment shader from file.
	/// @param context Graphics context to create the fragment shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @exception shader_load_error If loading the shader failed.
	/// @return Loaded frament shader.
	[[nodiscard]] fragment_shader load_fragment_shader(graphics_context& context, const std::filesystem::path& path);

	/// @}
} // namespace tr

//

/// Shader formatter.
template <>
struct std::formatter<tr::shader>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid shader format specification."};
		}
		return context.begin();
	}

	/// Formats a shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader& shader, FormatContext& context) const
	{
		if (shader.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", shader.label(), shader.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid shader at {}>", static_cast<const void*>(&shader));
		}
	}
};

/// Vertex shader formatter.
template <>
struct std::formatter<tr::vertex_shader>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid vertex shader format specification."};
		}
		return context.begin();
	}

	/// Formats a vertex shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Vertex shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::vertex_shader& shader, FormatContext& context) const
	{
		if (shader.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", shader.label(), shader.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid vertex shader at {}>", static_cast<const void*>(&shader));
		}
	}
};

/// Fragment shader formatter.
template <>
struct std::formatter<tr::fragment_shader>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid fragment shader format specification."};
		}
		return context.begin();
	}

	/// Formats a fragment shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Fragment shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::fragment_shader& shader, FormatContext& context) const
	{
		if (shader.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", shader.label(), shader.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid fragment shader at {}>", static_cast<const void*>(&shader));
		}
	}
};