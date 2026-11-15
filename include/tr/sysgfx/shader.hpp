/// @file
/// @brief Provides `tr::vertex_shader`, `tr::fragment_shader`, and related functionality.

#pragma once
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/sysgfx/internal/image_unit.hpp>
#include <tr/sysgfx/internal/texture_unit.hpp>
#include <tr/sysgfx/shader_array.hpp>
#include <tr/sysgfx/shader_buffer.hpp>
#include <tr/sysgfx/uniform_buffer.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/zstring_view.hpp>

#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include <tr/sysgfx/internal/glsl_variable.hpp>
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
	/// @brief Owning container for a shader program.
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
	/// Moved-from instances of shader objects are left in a special 'invalid' state. Invalid shader objects may not be interacted with
	/// besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// Shader objects may be labeled and are formattable. Example format output: `"My %shader" (ID: 3, PO: 3)`.
	class shader
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a shader.
		/// @param context Graphics context to create the shader on.
		/// @param source Shader GLSL source code.
		/// @param type Shader type (`GL_VERTEX_SHADER` or `GL_FRAGMENT_SHADER`).
		/// @param label Label of the shader.
		/// @exception shader_load_error If loading the shader failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] shader(graphics_context& context, zstring_view source, unsigned int type, std::string_view label);

		/// @brief Shaders are not copyable.
		shader(const shader&) = delete;

		/// @brief Shaders are not copyable.
		shader& operator=(const shader&) = delete;

		/// @brief Constructs a shader by transferring ownership from another `tr::shader`.
		/// @param rhs Shader to move from.
		/// @par Invalidation
		/// If the shader previously held by `rhs` was set on any shader pipelines, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] shader(shader&& rhs) noexcept = default;

		/// @brief Destroys the currently held shader and replaces it with one moved from another `tr::shader`.
		/// @param rhs Shader to move from.
		/// @par Invalidation
		/// If the shader previously held by `*this` was set on any shader pipelines, it is unset, and said pipelines are rendered
		/// incomplete.
		///
		/// If the shader previously held by `rhs` was set on any shader pipelines, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		shader& operator=(shader&& rhs) noexcept = default;

		/// @}
		/// @endcond
		/// @name Uniforms
		/// @{

		/// @brief Sets a boolean uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, bool value) noexcept;

		/// @brief Sets an integer uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, int value) noexcept;

		/// @brief Sets an integer array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const int> value) noexcept;

		/// @brief Sets an ivec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec2 value) noexcept;

		/// @brief Sets an ivec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec2> value) noexcept;

		/// @brief Sets an ivec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec3 value) noexcept;

		/// @brief Sets an ivec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec3> value) noexcept;

		/// @brief Sets an ivec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::ivec4 value) noexcept;

		/// @brief Sets an ivec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::ivec4> value) noexcept;

		/// @brief Sets an unsigned integer uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, unsigned int value) noexcept;

		/// @brief Sets an unsigned integer array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const unsigned int> value) noexcept;

		/// @brief Sets a uvec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec2 value) noexcept;

		/// @brief Sets a uvec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec2> value) noexcept;

		/// @brief Sets a uvec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec3 value) noexcept;

		/// @brief Sets a uvec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec3> value) noexcept;

		/// @brief Sets a uvec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::uvec4 value) noexcept;

		/// @brief Sets a uvec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::uvec4> value) noexcept;

		/// @brief Sets a float uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, float value) noexcept;

		/// @brief Sets a float array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const float> value) noexcept;

		/// @brief Sets a vec2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec2 value) noexcept;

		/// @brief Sets a vec2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec2> value) noexcept;

		/// @brief Sets a vec3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec3 value) noexcept;

		/// @brief Sets a vec3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec3> value) noexcept;

		/// @brief Sets a vec4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, glm::vec4 value) noexcept;

		/// @brief Sets a vec4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::vec4> value) noexcept;

		/// @brief Sets a mat2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2& value) noexcept;

		/// @brief Sets a mat2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2> value) noexcept;

		/// @brief Sets a mat3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3& value) noexcept;

		/// @brief Sets a mat3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3> value) noexcept;

		/// @brief Sets a mat4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4& value) noexcept;

		/// @brief Sets a mat4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4> value) noexcept;

		/// @brief Sets a mat2x3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2x3& value) noexcept;

		/// @brief Sets a mat2x3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2x3> value) noexcept;

		/// @brief Sets a mat2x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat2x4& value) noexcept;

		/// @brief Sets a mat2x4 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat2x4> value) noexcept;

		/// @brief Sets a mat3x2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3x2& value) noexcept;

		/// @brief Sets a mat3x2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3x2> value) noexcept;

		/// @brief Sets a mat3x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat3x4& value) noexcept;

		/// @brief Sets a mat3x4 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat3x4> value) noexcept;

		/// @brief Sets a mat4x2 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4x2& value) noexcept;

		/// @brief Sets a mat4x2 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4x2> value) noexcept;

		/// @brief Sets a mat4x3 uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, const glm::mat4x3& value) noexcept;

		/// @brief Sets a mat4x3 array uniform.
		/// @param index Uniform location index.
		/// @param value Uniform value.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_uniform(int index, std::span<const glm::mat4x3> value) noexcept;

		/// @brief Sets a texture sampler uniform.
		/// @param index Uniform location index.
		/// @param texture Texture to set.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_sampler_uniform(int index, texture_view texture);

		/// @brief Sets a texture image uniform.
		/// @param index Uniform location index.
		/// @param texture Texture to set.
		/// @param access Image access type.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// The uniform at index `index` must exist and be of a matching type.
		void set_image_uniform(int index, mutable_texture_view texture, access access);

		/// @brief Sets a shader storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index`.
		void set_storage_buffer(unsigned int index, untyped_shader_buffer& buffer) noexcept;

		/// @brief Sets a shader storage buffer.
		/// @tparam Header Type of the header object stored at the front of the buffer.
		/// @tparam ArrayElement Type of the buffer dynamic array elements.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index` with a matching layout.
		template <typename Header, typename ArrayElement>
		void set_storage_buffer(unsigned int index, shader_buffer<Header, ArrayElement>& buffer) noexcept
		{
			set_storage_buffer(index, buffer.unwrap(), sizeof(Header) + sizeof(ArrayElement) * buffer.array_size());
		}

		/// @brief Sets a shader storage buffer.
		/// @tparam Element Type of the array elements.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index` with a matching layout.
		template <typename Element>
		void set_storage_buffer(unsigned int index, shader_array<Element>& buffer) noexcept
		{
			set_storage_buffer(index, buffer.unwrap(), sizeof(Element) * buffer.size());
		}

		/// @brief Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index`.
		void set_uniform_buffer(unsigned int index, const untyped_uniform_buffer& buffer) noexcept;

		/// @brief Sets a uniform storage buffer.
		/// @tparam Object Object contained in the buffer.
		/// @param index Storage buffer location index.
		/// @param buffer Buffer to set.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index` with a matching layout.
		template <typename Object>
		void set_uniform_buffer(unsigned int index, const uniform_buffer<Object>& buffer) noexcept
		{
			set_uniform_buffer(index, buffer.unwrap());
		}

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the label of the shader.
		/// @param label Label of the shader.
		/// @pre The shader must be in a valid state.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the shader is in a valid state.
		/// @return `true` if the shader is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the shader is in a valid state.
		/// @return `true` if the shader is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the shader is on.
		/// @return Reference to the graphics context the shader is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Returns the label of the shader.
		/// @pre The shader must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the shader.
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL program object.
		/// @return Handle to the underlying OpenGL program object.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @cond implementation_details
		/// @brief Returns the ID uniquely identifying the shader.
		/// @pre The shader must be in a valid state.
		/// @return ID uniquely identifying the shader.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Returns a map describing the shader's inputs.
		/// @pre The shader must be in a valid state.
		/// @return Map describing the shader's inputs.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& inputs() const noexcept;

		/// @brief Returns a map describing the shader's outputs.
		/// @pre The shader must be in a valid state.
		/// @return Map describing the shader's outputs.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& outputs() const noexcept;
#endif
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL program object deleter.
		struct gl_program_object_deleter
		{
			/// @brief Reference to the graphics context the shader is on.
			ref<graphics_context> context;

			/// @brief ID uniquely identifying the shader.
			unique_handle<internal::graphics_object_id, internal::graphics_object_id::invalid> id{internal::generate_graphics_object_id()};

			//

			/// @brief Deletes a program object.
			/// @param handle OpenGL program object handle.
			void operator()(unsigned int handle) const noexcept;
		};

		/// @brief Managed handle to an OpenGL program object.
		unique_handle<unsigned int, 0, gl_program_object_deleter> m_handle;

		/// @brief Texture units allocated to and managed by this shader.
		boost::unordered_flat_map<int, internal::texture_unit> m_texture_units;

		/// @brief Image units allocated to and managed by this shader.
		boost::unordered_flat_map<int, internal::image_unit> m_image_units;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Description of the non-block uniforms of the shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_uniforms;

		/// @brief Description of the input variables of the shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_inputs;

		/// @brief Description of the output variables of the shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_outputs;
#endif

		/// @name Introspection
		/// @{

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Finds the uniforms of the shader using introspection and stores them in `m_uniforms`.
		/// @param gl Structure holding the OpenGL API.
		/// @pre The shader must be in a valid state.
		void find_uniforms(const internal::opengl& gl);

		/// @brief Finds the input variables of the shader using introspection and stores them in `m_inputs`.
		/// @param gl Structure holding the OpenGL API.
		/// @pre The shader must be in a valid state.
		void find_inputs(const internal::opengl& gl);

		/// @brief Finds the output variables of the shader using introspection and stores them in `m_outputs`.
		/// @param gl Structure holding the OpenGL API.
		/// @pre The shader must be in a valid state.
		void find_outputs(const internal::opengl& gl);
#endif

		/// @}
		/// @name Uniforms
		/// @{

		/// @brief Sets a shader storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer_handle OpenGL buffer object handle.
		/// @param buffer_size Size of the buffer.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index`.
		void set_storage_buffer(unsigned int index, unsigned int buffer_handle, std::intptr_t buffer_size) noexcept;

		/// @brief Sets a uniform storage buffer.
		/// @param index Storage buffer location index.
		/// @param buffer_handle OpenGL buffer object handle.
		/// @pre
		/// The shader must be in a valid state.
		///
		/// There must be a buffer at index `index`.
		void set_uniform_buffer(unsigned int index, unsigned int buffer_handle) noexcept;

		/// @}
	};

	//

	/// @brief Owning container for a vertex shader program.
	/// @details
	/// A vertex shader is a shader program that transforms vertices passed to the drawing operation and outputs data passed along to the
	/// fragment shader.
	///
	/// Everything brought up in the description of `tr::shader` applies to this class as well.
	class vertex_shader : public shader
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a vertex shader.
		/// @param context Graphics context to create the vertex shader on.
		/// @param source Vertex shader GLSL source code.
		/// @param label Label of the shader.
		/// @exception shader_load_error If loading the shader failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] vertex_shader(graphics_context& context, zstring_view source, std::string_view label = {});

		/// @brief Vertex shaders are not copyable.
		vertex_shader(const vertex_shader&) = delete;

		/// @brief Vertex shaders are not copyable.
		vertex_shader& operator=(const vertex_shader&) = delete;

		/// @brief Constructs a vertex shader by transferring ownership from another `tr::vertex_shader`.
		/// @param rhs Vertex shader to move from.
		/// @par Invalidation
		/// If the vertex shader previously held by `rhs` was set on any shader pipelines, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] vertex_shader(vertex_shader&& rhs) noexcept = default;

		/// @brief Destroys the currently held vertex shader and replaces it with one moved from another `tr::vertex_shader`.
		/// @param rhs Vertex shader to move from.
		/// @par Invalidation
		/// If the vertex shader previously held by `*this` was set on any shader pipelines, it is unset, and said pipelines are rendered
		/// incomplete.
		///
		/// If the vertex shader previously held by `rhs` was set on any shader pipelines, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		vertex_shader& operator=(vertex_shader&& rhs) noexcept = default;

		/// @}
	};

	/// @brief Owning container for a fragment shader program.
	/// @details
	/// A fragment shader is a shader program that calculates the final color of a fragment (in most cases a pixel) given data output by the
	/// vertex shader and interpolated along a primitive.
	///
	/// Everything brought up in the description of `tr::shader` applies to this class as well.
	class fragment_shader : public shader
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a fragment shader.
		/// @param context Graphics context to create the fragment shader on.
		/// @param source Fragment shader GLSL source code.
		/// @param label Label of the shader.
		/// @exception shader_load_error If loading the shader failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] fragment_shader(graphics_context& context, zstring_view source, std::string_view label = {});

		/// @brief Fragment shaders are not copyable.
		fragment_shader(const fragment_shader&) = delete;

		/// @brief Fragment shaders are not copyable.
		fragment_shader& operator=(const fragment_shader&) = delete;

		/// @brief Constructs a fragment shader by transferring ownership from another `tr::fragment_shader`.
		/// @param rhs Fragment shader to move from.
		/// @par Invalidation
		/// If the fragment shader previously held by `rhs` was set on any shader pipelines, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] fragment_shader(fragment_shader&& rhs) noexcept = default;

		/// @brief Destroys the currently held fragment shader and replaces it with one moved from another `tr::fragment_shader`.
		/// @param rhs Fragment shader to move from.
		/// @par Invalidation
		/// If the fragment shader previously held by `*this` was set on any shader pipelines, it is unset, and said pipelines are rendered
		/// incomplete.
		///
		/// If the fragment shader previously held by `rhs` was set on any shader pipelines, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		fragment_shader& operator=(fragment_shader&& rhs) noexcept = default;

		/// @}
	};

	/// @brief Owning container for a compute shader program.
	/// @details
	/// A compute shader is a shader program that is executed to compute arbitrary information independently of the regular shader pipeline.
	///
	/// Everything brought up in the description of `tr::shader` applies to this class as well.
	class compute_shader : public shader
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a compute shader.
		/// @param context Graphics context to create the compute shader on.
		/// @param source Compute shader GLSL source code.
		/// @param label Label of the shader.
		/// @exception shader_load_error If loading the shader failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] compute_shader(graphics_context& context, zstring_view source, std::string_view label = {});

		/// @brief Compute shaders are not copyable.
		compute_shader(const compute_shader&) = delete;

		/// @brief Compute shaders are not copyable.
		compute_shader& operator=(const compute_shader&) = delete;

		/// @brief Constructs a compute shader by transferring ownership from another `tr::compute_shader`.
		/// @param rhs Compute shader to move from.
		/// @par Invalidation
		/// If the compute shader previously held by `rhs` was set on a graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] compute_shader(compute_shader&& rhs) noexcept = default;

		/// @brief Destroys the currently held compute shader and replaces it with one moved from another `tr::compute_shader`.
		/// @param rhs Compute shader to move from.
		/// @par Invalidation
		/// If the compute shader previously held by `*this` was set on a graphics context, it is unset.
		///
		/// If the compute shader previously held by `rhs` was set on a graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		compute_shader& operator=(compute_shader&& rhs) noexcept = default;

		/// @}
	};

	//

	/// @brief Loads a vertex shader from a source file.
	/// @param context Graphics context to create the vertex shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @param label Label of the shader.
	/// @exception shader_load_error If loading the shader failed.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Loaded vertex shader.
	[[nodiscard]] vertex_shader load_vertex_shader(graphics_context& context, const std::filesystem::path& path,
												   std::string_view label = {});

	/// @brief Loads a fragment shader from a source file.
	/// @param context Graphics context to create the fragment shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @param label Label of the shader.
	/// @exception shader_load_error If loading the shader failed.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Loaded frament shader.
	[[nodiscard]] fragment_shader load_fragment_shader(graphics_context& context, const std::filesystem::path& path,
													   std::string_view label = {});

	/// @brief Loads a compute shader from a source file.
	/// @param context Graphics context to create the compute shader on.
	/// @param path Path to the shader GLSL source code file.
	/// @param label Label of the shader.
	/// @exception shader_load_error If loading the shader failed.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Loaded compute shader.
	[[nodiscard]] compute_shader load_compute_shader(graphics_context& context, const std::filesystem::path& path,
													 std::string_view label = {});
} // namespace tr

//

/// @brief Shader formatter.
template <>
struct std::formatter<tr::shader>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader& shader, FormatContext& context) const
	{
		if (shader) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, PO: {})", shader.label(), std::to_underlying(shader.id()),
								  shader.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid shader at {}>", static_cast<const void*>(&shader));
		}
	}
};

/// @brief Vertex shader formatter.
template <>
struct std::formatter<tr::vertex_shader>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a vertex shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Vertex shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::vertex_shader& shader, FormatContext& context) const
	{
		if (shader) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, PO: {})", shader.label(), std::to_underlying(shader.id()),
								  shader.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid vertex shader at {}>", static_cast<const void*>(&shader));
		}
	}
};

/// @brief Fragment shader formatter.
template <>
struct std::formatter<tr::fragment_shader>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a fragment shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Fragment shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::fragment_shader& shader, FormatContext& context) const
	{
		if (shader) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, PO: {})", shader.label(), std::to_underlying(shader.id()),
								  shader.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid fragment shader at {}>", static_cast<const void*>(&shader));
		}
	}
};

/// @brief Compute shader formatter.
template <>
struct std::formatter<tr::compute_shader>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid compute shader format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a compute shader.
	/// @tparam FormatContext Formatting context type.
	/// @param shader Compute shader to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::compute_shader& shader, FormatContext& context) const
	{
		if (shader) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, PO: {})", shader.label(), std::to_underlying(shader.id()),
								  shader.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid compute shader at {}>", static_cast<const void*>(&shader));
		}
	}
};