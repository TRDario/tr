/// @file
/// @brief Provides `tr::shader_pipeline`.

#pragma once
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include <tr/sysgfx/internal/glsl_variable.hpp>
#endif

namespace tr
{
	class fragment_shader;
	class graphics_context;
	class vertex_shader;
} // namespace tr

//

namespace tr
{
	/// @brief Owning container for a shader program pipeline.
	/// @details
	/// A shader pipeline is an object that references vertex and fragment shader program stages that are to be used during the rendering
	/// process when the pipeline is set on the graphics context. These referenced shaders must be compatible: the outputs of the vertex
	/// shader must match the inputs of the fragment shader.
	///
	/// It's worth remembering that instances of `tr::shader_pipeline` are only containers for these underlying shader pipeline objects.
	/// This means, for example, that setting a shader pipeline on a graphics context does not set the literal `tr::shader_pipeline` object
	/// at a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::shader_pipeline`, that value will still be set on the context. If the instance is overriden with a new value, the old value is
	/// destroyed and the graphics context will no longer have a set shader pipeline.
	///
	/// `tr::shader_pipeline` has a notion of 'completeness'. An instance of the class may be constructed without set shaders; such an
	/// instance is considered incomplete until a valid vertex and fragment shader are set to it. Incomplete shader pipelines may not be set
	/// to a graphics context.
	///
	/// Every instance of `tr::shader_pipeline` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::shader_pipeline` are left in a special 'invalid' state, distinct from the incomplete state. Invalid
	/// `tr::shader_pipeline` instances may not be interacted with besides moving a new value into them and checking for validity using
	/// `valid()`.
	///
	/// `tr::shader_pipeline` instances may be labeled and are formattable. Example format output: `"My pipeline" (ID: 3, PPO: 3)`.
	class shader_pipeline
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an incomplete shader pipeline.
		/// @param context Graphics context to create the shader pipeline on.
		/// @param label Label of the shader pipeline.
		[[nodiscard]] explicit shader_pipeline(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Constructs a complete shader pipeline.
		/// @param context Graphics context to create the shader pipeline on.
		/// @param vertex_shader Vertex shader to initially use on the shader pipeline.
		/// @param fragment_shader Fragment shader to initially use on the shader pipeline.
		/// @param label Label of the shader pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be compatible with each other and be on `%context`.
		/// @post The pipeline will revert to an incomplete state should either one of the current values of `%vertex_shader` or
		/// `%fragment_shader` be destroyed while still set on the shader pipeline.
		[[nodiscard]] shader_pipeline(graphics_context& context, const vertex_shader& vertex_shader, const fragment_shader& fragment_shader,
									  std::string_view label = {}) noexcept;

		/// @brief Shader pipelines are not copyable.
		shader_pipeline(const shader_pipeline&) = delete;

		/// @brief Shader pipelines are not copyable.
		shader_pipeline& operator=(const shader_pipeline&) = delete;

		/// @brief Constructs a shader pipeline by transferring ownership from another `tr::shader_pipeline`.
		/// @param rhs Shader pipeline to move from.
		/// @par Invalidation
		/// Any shaders set on the shader pipeline previously held by `rhs` remain set, as does the shader pipeline itself if it was set on
		/// a graphics context.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] shader_pipeline(shader_pipeline&& rhs) noexcept = default;

		/// @brief Destroys the currently held shader pipeline and transfers ownership from another `tr::shader_pipeline`.
		/// @param rhs Shader pipeline to move from.
		/// @par Invalidation
		/// If the shader pipeline previously held by `*this` was set on a graphics context, it is unset.
		///
		/// Any shaders set on the shader pipeline previously held by `rhs` remain set, as does the shader pipeline itself if it was set on
		/// a graphics context.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		shader_pipeline& operator=(shader_pipeline&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets both shaders of the shader pipeline.
		/// @param vertex_shader Vertex shader to use on the shader pipeline.
		/// @param fragment_shader Fragment shader to use on the shader pipeline.
		/// @pre
		/// The shader pipeline must be in a valid state.
		///
		/// `%vertex_shader` and `%fragment_shader` must be compatible with each other and be on the same context as the shader pipeline.
		/// @post The shader pipeline will revert to an incomplete state should either one of the current values of `%vertex_shader` or
		/// `%fragment_shader` be destroyed while still set on the shader pipeline.
		void set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader) noexcept;

		/// @brief Sets a vertex shader to the shader pipeline.
		/// @param vertex_shader Vertex shader to set to the shader pipeline.
		/// @pre
		/// The shader pipeline must be in a valid state.
		///
		/// `%vertex_shader` must be compatible with the currently set fragment shader and be on the same context as the shader pipeline.
		/// @post The shader pipeline will revert to an incomplete state should the current value of `%vertex_shader` be destroyed while
		/// still set on the shader pipeline.
		void set_vertex_shader(const vertex_shader& vertex_shader) noexcept;

		/// @brief Sets a fragment shader to the shader pipeline.
		/// @param fragment_shader Fragment shader to set to the shader pipeline.
		/// @pre
		/// The shader pipeline must be in a valid state.
		///
		/// `%fragment_shader` must be compatible with the currently set vertex shader and be on the same context as the shader pipeline.
		/// @post The shader pipeline will revert to an incomplete state should the current value of `%fragment_shader` be destroyed while
		/// still set on the shader pipeline.
		void set_fragment_shader(const fragment_shader& fragment_shader) noexcept;

		/// @brief Sets the label of the pipeline.
		/// @param label Label of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the pipeline is on.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the graphics context the pipeline is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Returns the label of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the pipeline.
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL program pipeline object.
		/// @return Handle to the underlying OpenGL program pipeline object.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @cond implementation_details
		/// @brief Returns the ID uniquely identifying the shader pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return ID uniquely identifying the shader pipeline.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;

		/// @brief Returns the ID uniquely identifying the vertex shader set on the shader pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return ID uniquely identifying the vertex shader set on the shader pipeline.
		[[nodiscard]] internal::graphics_object_id vertex_shader_id() const noexcept;

		/// @brief Returns the ID uniquely identifying the fragment shader set on the shader pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return ID uniquely identifying the fragment shader set on the shader pipeline.
		[[nodiscard]] internal::graphics_object_id fragment_shader_id() const noexcept;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Returns the outputs of the vertex shader currently set on the shader pipeline.
		/// @pre The shader pipeline must be in a valid state and a vertex shader must be set on it.
		/// @return Reference to the outputs of the vertex shader set on the shader pipeline.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& vertex_shader_outputs() const noexcept;

		/// @brief Returns the inputs of the fragment shader set on the pipeline.
		/// @pre The shader pipeline must be in a valid state and a fragment shader must be set on it.
		/// @return Reference to the inputs of the fragment shader set on the shader pipeline.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& fragment_shader_inputs() const noexcept;
#endif
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL program pipeline object deleter.
		struct gl_program_pipeline_object_deleter
		{
			/// @brief Reference to the graphics context the shader pipeline is on.
			ref<graphics_context> context;

			/// @brief ID uniquely identifying the shader pipeline.
			unique_handle<internal::graphics_object_id, internal::graphics_object_id::invalid> id{internal::generate_graphics_object_id()};

			//

			/// @brief Deletes a program pipeline object.
			/// @param handle OpenGL program pipeline object handle.
			void operator()(unsigned int handle) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL program pipeline object.
		unique_handle<unsigned int, 0, gl_program_pipeline_object_deleter> m_handle;

		/// @brief ID uniquely identifying the vertex shader currently set on the shader pipeline.
		internal::graphics_object_id m_vertex_shader_id{internal::graphics_object_id::invalid};

		/// @brief ID uniquely identifying the fragment shader currently set on the shader pipeline.
		internal::graphics_object_id m_fragment_shader_id{internal::graphics_object_id::invalid};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Outputs of the vertex shader currently set on the shader pipeline.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_vertex_shader_outputs;

		/// @brief Inputs of the fragment shader currently set on the shader pipeline.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_fragment_shader_inputs;
#endif
	};
} // namespace tr

//

/// @brief Shader pipeline formatter.
template <>
struct std::formatter<tr::shader_pipeline>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid shader pipeline format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a shader pipeline.
	/// @tparam FormatContext Formatting context type.
	/// @param pipeline Shader pipeline to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader_pipeline& pipeline, FormatContext& context) const
	{
		if (pipeline.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, PPO: {})", pipeline.label(), std::to_underlying(pipeline.id()),
								  pipeline.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid shader pipeline at {}>", static_cast<const void*>(&pipeline));
		}
	}
};