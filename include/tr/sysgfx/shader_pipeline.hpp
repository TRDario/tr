/// @file
/// @brief Provides shader pipeline classes.

#pragma once
#include "shader.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr
{
	// Shader program pipeline.
	class shader_pipeline
	{
	  public:
		// Creates a shader pipeline.
		shader_pipeline(graphics_context& context, const vertex_shader& vshader, const fragment_shader& fshader);

		// Gets a reference to the graphics context the pipeline is on.
		graphics_context& context() const;

		// Resets the shaders of the pipeline.
		void set_shaders(const vertex_shader& vshader, const fragment_shader& fshader);

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);
		// Gets the debug label of the pipeline.
		std::string label() const;

	  private:
		// Shader pipeline deleter.
		struct deleter
		{
			// Reference to the graphics context the pipeline is on.
			graphics_context& context;

			// Deletes the shader pipeline.
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_ppo;

		friend class graphics_context;
	};

	// Shader program pipeline that owns its shaders.
	class owning_shader_pipeline
	{
	  public:
		// Creates an owning shader pipeline.
		owning_shader_pipeline(graphics_context& context, vertex_shader&& vshader, fragment_shader&& fshader);

		// Gets the base pipeline object.
		operator const shader_pipeline&() const;

		// Gets a reference to the graphics context the pipeline is on.
		graphics_context& context() const;

		// Gets the vertex shader.
		vertex_shader& vertex_shader();
		// Gets the vertex shader.
		const class vertex_shader& vertex_shader() const;
		// Gets the fragment shader.
		fragment_shader& fragment_shader();
		// Gets the fragment shader.
		const class fragment_shader& fragment_shader() const;

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);
		// Gets the debug label of the pipeline.
		std::string label() const;

	  private:
		// The vertex shader.
		class vertex_shader m_vshader;
		// The fragment shader.
		class fragment_shader m_fshader;
		// The base shader pipeline.
		shader_pipeline m_base;
	};
} // namespace tr