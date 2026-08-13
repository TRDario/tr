/// @file
/// @brief Provides shader pipeline classes.

#pragma once
#include "../utility/reference.hpp"
#include "shader.hpp"

//

namespace tr
{
	/// Shader program pipeline.
	class shader_pipeline
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an incomplete shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		shader_pipeline(graphics_context& context);

		/// Creates a complete shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to initially use on the pipeline.
		/// @param fragment_shader Fragment shader to initially use on the pipeline.
		/// @pre `vshader` and `fshader` must be compatible shaders.
		/// @post `vshader` and `fshader` should not be moved while in use by this pipeline.
		shader_pipeline(graphics_context& context, const vertex_shader& vertex_shader, const fragment_shader& fragment_shader);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the pipeline is on.
		/// @return Reference to the graphics context the pipeline is on
		graphics_context& context() const;

		/// @}
		/// @name Shaders
		/// @{

		/// Sets both shaders of the pipeline.
		/// @param vertex_shader Vertex shader to use on the pipeline.
		/// @param fragment_shader Fragment shader to use on the pipeline.
		/// @pre `vshader` and `fshader` must be compatible shaders.
		/// @post `vshader` and `fshader` should not be moved while in use by this pipeline.
		void set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader);

		/// Sets a vertex shader to the pipeline.
		/// @param shader Vertex shader to set to the pipeline.
		/// @pre `shader` must be compatible with the currently set fragment shader.
		/// @post `shader` should not be moved while in use by this pipeline.
		void set_vertex_shader(const vertex_shader& shader);

		/// Sets a fragment shader to the pipeline.
		/// @param shader Fragment shader to set to the pipeline.
		/// @pre `shader` must be compatible with the currently set fragment shader.
		/// @post `shader` should not be moved while in use by this pipeline.
		void set_fragment_shader(const fragment_shader& shader);

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
		/// @name ID
		/// @{

		/// Gets the unique ID of the shader pipeline.
		/// @return Unique ID of the shader pipeline.
		unsigned int id() const;

		/// @}

#ifdef TR_ENABLE_GL_CHECKS
		/// @cond __hidden
		/// Gets whether the pipeline is complete.
		/// @return `true` if the pipeline is complete (has both shaders bound to it), `false` otherwise.
		bool complete() const;
		/// @endcond
#endif

	  private:
		/// Shader pipeline deleter.
		struct deleter
		{
			/// Reference to the graphics context the pipeline is on.
			graphics_context& context;

			//

			/// Deletes the shader pipeline.
			/// @param id OpenGL shader pipeline ID.
			void operator()(unsigned int id) const;
		};

		//

		/// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_ppo;

#ifdef TR_ENABLE_GL_CHECKS
		/// Reference to the used vertex shader.
		opt_ref<const vertex_shader> m_vertex_shader;

		/// Reference to the used fragment shader.
		opt_ref<const fragment_shader> m_fragment_shader;
#endif
	};

	/// Shader program pipeline that owns its shaders.
	class owning_shader_pipeline
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an owning shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to move into the pipeline.
		/// @param fragment_shader Fragment shader to move into the pipeline.
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
		/// @return Reference to the graphics context the pipeline is on
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
		/// @name Label
		/// @{

		/// Sets the debug label of the pipeline.
		/// @param label Debug label of the pipeline.
		void set_label(std::string_view label);

		/// Gets the debug label of the pipeline.
		/// @return Debug label of the pipeline.
		std::string label() const;

		///@}
		/// @name ID
		/// @{

		/// Gets the unique ID of the shader pipeline.
		/// @return Unique ID of the shader pipeline.
		unsigned int id() const;

		/// @}

	  private:
		/// Held vertex shader.
		class vertex_shader m_vertex_shader;

		/// Held fragment shader.
		class fragment_shader m_fragment_shader;

		/// Base shader pipeline.
		shader_pipeline m_base;
	};
} // namespace tr