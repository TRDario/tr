/// @file
/// @brief Provides `tr::shader_pipeline`.

#pragma once
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/utility/handle.hpp>
#include <tr/utility/ref.hpp>

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
	/// Container for a shader program pipeline.
	/// @details
	/// A shader pipeline is an object that references vertex and fragment shader program stages that are to be used during the rendering
	/// process when the pipeline is set on the graphics context. These referenced shaders must be compatible: the outputs of the vertex
	/// shader must match the inputs of the fragment shader.
	///
	/// Strictly speaking, instances of `tr::shader_pipeline` are only containers for these underlying shader pipeline objects. This means,
	/// for example, that setting a shader pipeline on a graphics context does not set the literal `tr::shader_pipeline` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of
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
		/// @name Constructors and destructors
		/// @{

		/// Constructs an incomplete shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param label Label of the pipeline.
		[[nodiscard]] explicit shader_pipeline(graphics_context& context, std::string_view label = {}) noexcept;

		/// Constructs a complete shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to initially use on the pipeline.
		/// @param fragment_shader Fragment shader to initially use on the pipeline.
		/// @param label Label of the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be compatible with each other and be on `%context`.
		/// @post The pipeline will revert to an incomplete state should either one of the current values of `%vertex_shader` or
		/// `%fragment_shader` be destroyed while still set on the pipeline.
		[[nodiscard]] shader_pipeline(graphics_context& context, const vertex_shader& vertex_shader, const fragment_shader& fragment_shader,
									  std::string_view label = {}) noexcept;

		/// Shader pipelines are not copyable.
		shader_pipeline(const shader_pipeline&) = delete;

		/// Moves a shader pipeline.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Shader pipeline to move.
		[[nodiscard]] shader_pipeline(shader_pipeline&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Shader pipelines are not copyable.
		shader_pipeline& operator=(const shader_pipeline&) = delete;

		/// Moves a shader pipeline.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Shader pipeline to move.
		/// @return Reference to `*this`.
		shader_pipeline& operator=(shader_pipeline&& rhs) noexcept = default;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the pipeline is on.
		/// @return Reference to the graphics context the pipeline is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name Shaders
		/// @{

		/// Sets both shaders of the pipeline.
		/// @param vertex_shader Vertex shader to use on the pipeline.
		/// @param fragment_shader Fragment shader to use on the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be compatible with each other and be on the same context as the pipeline.
		/// @post The pipeline will revert to an incomplete state should either one of the current values of `%vertex_shader` or
		/// `%fragment_shader` be destroyed while still set on the pipeline.
		void set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader) noexcept;

		/// Sets a vertex shader to the pipeline.
		/// @param vertex_shader Vertex shader to set to the pipeline.
		/// @pre `%vertex_shader` must be compatible with the currently set fragment shader and be on the same context as the pipeline.
		/// @post The pipeline will revert to an incomplete state should the current value of `%vertex_shader` be destroyed while still set
		/// on the pipeline.
		void set_vertex_shader(const vertex_shader& vertex_shader) noexcept;

		/// Sets a fragment shader to the pipeline.
		/// @param fragment_shader Fragment shader to set to the pipeline.
		/// @pre `%fragment_shader` must be compatible with the currently set vertex shader and be on the same context as the pipeline.
		/// @post The pipeline will revert to an incomplete state should the current value of `%fragment_shader` be destroyed while still
		/// set on the pipeline.
		void set_fragment_shader(const fragment_shader& fragment_shader) noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the pipeline.
		/// @param label Debug label of the pipeline.
		void set_label(std::string_view label) noexcept;

		/// Gets the debug label of the pipeline.
		/// @return Debug label of the pipeline.
		[[nodiscard]] std::string label() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL shader pipeline.
		/// @note This does not release the shader pipeline.
		/// @return OpenGL shader pipeline ID.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Gets the unique graphics object ID of the shader pipeline.
		/// @return Unique graphics object ID of the shader pipeline.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;

		/// Gets the unique graphics object ID of the vertex shader set on the pipeline.
		/// @return Unique graphics object ID of the vertex shader set on the pipeline.
		[[nodiscard]] internal::graphics_object_id vertex_shader_id() const noexcept;

		/// Gets the unique graphics object ID of the fragment shader set on the pipeline.
		/// @return Unique graphics object ID of the fragment shader set on the pipeline.
		[[nodiscard]] internal::graphics_object_id fragment_shader_id() const noexcept;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Gets the outputs of the vertex shader set on the pipeline.
		/// @return Reference to the outputs of the vertex shader set on the pipeline.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& vertex_shader_outputs() const noexcept;

		/// Gets the inputs of the fragment shader set on the pipeline.
		/// @return Reference to the inputs of the fragment shader set on the pipeline.
		[[nodiscard]] const boost::unordered_flat_map<unsigned int, internal::glsl_variable>& fragment_shader_inputs() const noexcept;
#endif

		/// @}
		/// @endcond

	  private:
		/// Shader pipeline deleter.
		struct deleter
		{
			/// Reference to the graphics context the pipeline is on.
			ref<graphics_context> context;

			/// Handle to the unique graphics object ID of the pipeline.
			internal::graphics_object_id_handle id{};

			//

			/// Deletes the shader pipeline.
			/// @param ppo OpenGL shader pipeline ID.
			void operator()(unsigned int ppo) const noexcept;
		};

		//

		/// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_handle;

		/// Unique graphics object ID of the vertex shader.
		internal::graphics_object_id m_vertex_shader_id{internal::graphics_object_id::invalid};

		/// Unique graphics object ID of the fragment shader.
		internal::graphics_object_id m_fragment_shader_id{internal::graphics_object_id::invalid};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Outputs of the vertex shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_vertex_shader_outputs;

		/// Inputs of the fragment shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> m_fragment_shader_inputs;
#endif
	};
} // namespace tr

//

/// Shader pipeline formatter.
template <>
struct std::formatter<tr::shader_pipeline>
{
	/// Parses the format specification.
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

	/// Formats a shader pipeline.
	/// @tparam FormatContext Formatting context type.
	/// @param pipeline Shader pipeline to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader_pipeline& pipeline, FormatContext& context) const
	{
		if (pipeline.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, PPO: {})", pipeline.label(), std::to_underlying(pipeline.id()),
								  pipeline.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid shader pipeline at {}>", static_cast<const void*>(&pipeline));
		}
	}
};