/// @file
/// @brief Provides `tr::shader_pipeline` and `tr::owning_shader_pipeline`.
/// @details For an explanation of shader pipelines, see the description of `tr::shader_pipeline`.

#pragma once
#include "../utility/reference.hpp"
#include "shader.hpp"

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
	/// `tr::shader_pipeline` instances are movable, but not copyable. A moved-from instance of `tr::shader_pipeline` is left in a special
	/// 'invalid' state, distinct from the incomplete state. Invalid `tr::shader_pipeline` instances may not be interacted with besides
	/// moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::shader_pipeline` instances may be labeled and are formattable. Example format output: `"My pipeline" (OpenGL ID: 5)`.
	class shader_pipeline
	{
	  public:
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details

		/// Debug information about the set vertex shader.
		struct vertex_shader_debug_info_t
		{
			/// Unique graphics object ID of the shader.
			graphics_object_id id{graphics_object_id::invalid};

			/// Label of the shader.
			std::string label{"<unset>"};

			/// Outputs of the shader.
			boost::unordered_flat_map<unsigned int, glsl_variable> outputs;
		};

		/// Debug information about the set fragment shader.
		struct fragment_shader_debug_info_t
		{
			/// Unique graphics object ID of the shader.
			graphics_object_id id{graphics_object_id::invalid};

			/// Label of the shader.
			std::string label{"<unset>"};

			/// Inputs of the shader.
			boost::unordered_flat_map<unsigned int, glsl_variable> inputs;
		};

		/// @endcond
#endif

		/// @name Constructors
		/// @{

		/// Constructs an incomplete shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		shader_pipeline(graphics_context& context);

		/// Constructs a complete shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to initially use on the pipeline.
		/// @param fragment_shader Fragment shader to initially use on the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be compatible with each other and be on `%context`.
		/// @post The pipeline will revert to an incomplete state should either one of the current values of `%vertex_shader` or
		/// `%fragment_shader` be destroyed while still set on the pipeline.
		shader_pipeline(graphics_context& context, const vertex_shader& vertex_shader, const fragment_shader& fragment_shader);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the pipeline is on.
		/// @return Reference to the graphics context the pipeline is on.
		graphics_context& context() const;

		/// @}
		/// @name Shaders
		/// @{

		/// Sets both shaders of the pipeline.
		/// @param vertex_shader Vertex shader to use on the pipeline.
		/// @param fragment_shader Fragment shader to use on the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be compatible with each other and be on the same context as the pipeline.
		/// @post The pipeline will revert to an incomplete state should either one of the current values of `%vertex_shader` or
		/// `%fragment_shader` be destroyed while still set on the pipeline.
		void set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader);

		/// Sets a vertex shader to the pipeline.
		/// @param vertex_shader Vertex shader to set to the pipeline.
		/// @pre `%vertex_shader` must be compatible with the currently set fragment shader and be on the same context as the pipeline.
		/// @post The pipeline will revert to an incomplete state should the current value of `%vertex_shader` be destroyed while still set
		/// on the pipeline.
		void set_vertex_shader(const vertex_shader& vertex_shader);

		/// Sets a fragment shader to the pipeline.
		/// @param fragment_shader Fragment shader to set to the pipeline.
		/// @pre `%fragment_shader` must be compatible with the currently set vertex shader and be on the same context as the pipeline.
		/// @post The pipeline will revert to an incomplete state should the current value of `%fragment_shader` be destroyed while still
		/// set on the pipeline.
		void set_fragment_shader(const fragment_shader& fragment_shader);

		/// @}
		/// @name State
		/// @{

		/// Gets whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		bool valid() const;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the pipeline.
		/// @param label Debug label of the pipeline.
		void set_label(std::string_view label);

		/// Gets the debug label of the pipeline.
		/// @return Debug label of the pipeline.
		std::string label() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL shader pipeline.
		/// @note This does not release the shader pipeline.
		/// @return OpenGL shader pipeline ID.
		unsigned int unwrap() const;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Gets the unique graphics object ID of the shader pipeline.
		/// @return Unique graphics object ID of the shader pipeline.
		graphics_object_id id() const;

		/// Gets debug information about the set vertex shader.
		/// @return Reference to the structure containing debug information about the set vertex shader.
		const vertex_shader_debug_info_t& vertex_shader_debug_info() const;

		/// Gets debug information about the set fragment shader.
		/// @return Reference to the structure containing debug information about the set fragment shader.
		const fragment_shader_debug_info_t& fragment_shader_debug_info() const;

		/// @}
		/// @endcond
#endif

	  private:
		/// Shader pipeline deleter.
		struct deleter
		{
			/// Reference to the graphics context the pipeline is on.
			ref<graphics_context> context;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			/// Handle to the unique graphics object ID of the pipeline.
			graphics_object_id_handle id{};
#endif

			//

			/// Deletes the shader pipeline.
			/// @param ppo OpenGL shader pipeline ID.
			void operator()(unsigned int ppo) const;
		};

		//

		/// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_handle;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Debug information about the set vertex shader.
		vertex_shader_debug_info_t m_vertex_shader_debug_info;

		/// Debug information about the set fragment shader.
		fragment_shader_debug_info_t m_fragment_shader_debug_info;
#endif

		//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Asserts that the set vertex and fragment shaders are compatible.
		void assert_shaders_compatible() const;
#endif
	};

	/// Container for a shader program pipeline that owns its shaders.
	/// @details
	/// Most things brought up in the description of `tr::shader_pipeline` apply to this class as well, though instances of
	/// `tr::owning_shader_pipeline` are always complete.
	class owning_shader_pipeline
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an owning shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to move into the pipeline.
		/// @param fragment_shader Fragment shader to move into the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be valid shaders and be on `%context`.
		owning_shader_pipeline(graphics_context& context, vertex_shader&& vertex_shader, fragment_shader&& fragment_shader);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets the base pipeline object.
		/// @return Reference to the base pipeline object.
		operator const shader_pipeline&() const;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the pipeline is on.
		/// @return Reference to the graphics context the pipeline is on.
		graphics_context& context() const;

		/// @}
		/// @name Shaders
		/// @{

		/// Gets the vertex shader.
		/// @return Reference to the vertex shader of the pipeline.
		vertex_shader& vertex_shader();

		/// Gets the vertex shader.
		/// @return Reference to the vertex shader of the pipeline.
		const class vertex_shader& vertex_shader() const;

		/// Gets the fragment shader.
		/// @return Reference to the fragment shader of the pipeline.
		fragment_shader& fragment_shader();

		/// Gets the fragment shader.
		/// @return Reference to the fragment shader of the pipeline.
		const class fragment_shader& fragment_shader() const;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		bool valid() const;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the pipeline.
		/// @param label Debug label of the pipeline.
		void set_label(std::string_view label);

		/// Gets the debug label of the pipeline.
		/// @return Debug label of the pipeline.
		std::string label() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL shader pipeline.
		/// @note This does not release the shader pipeline.
		/// @return OpenGL shader pipeline ID.
		unsigned int unwrap() const;

		/// @}
		/// @endcond

	  private:
		/// Held vertex shader.
		class vertex_shader m_vertex_shader;

		/// Held fragment shader.
		class fragment_shader m_fragment_shader;

		/// Base shader pipeline.
		shader_pipeline m_shader_pipeline;
	};
} // namespace tr

#include "impl/shader_pipeline.hpp" // IWYU pragma: export