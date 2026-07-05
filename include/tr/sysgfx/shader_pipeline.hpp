///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides shader pipeline classes.                                                                                                     //
//                                                                                                                                       //
// Shader pipelines are an abstraction over OpenGL program pipeline objects.                                                             //
//                                                                                                                                       //
// tr::shader_pipeline is an object that references a shader of each stage and combines them into a single pipeline. It does not         //
// take ownership of the shaders passed to it:                                                                                           //
//     - tr::shader_pipeline{context, vshader, fshader} -> creates a pipeline with vertex and fragment shader stages                     //
//                                                                                                                                       //
// tr::owning_shader_pipeline is an object that *does* own the shader stages within it. After construction, these shaders can be         //
// accessed from the pipeline:                                                                                                           //
//     - tr::owning_shader_pipeline pipeline{context, tr::vertex_shader{context, vshader_src}, tr::fragment_shader{context, fshader_src}}//
//       -> creates vertex and fragment shaders and then stores them in the pipeline                                                     //
//     - pipeline.vertex_shader().set_uniform(0, 5.0f) -> sets uniform 0 of the vertex shader of the pipeline                            //
//                                                                                                                                       //
// The label of a shader pipeline can be set with .set_label() and gotten with .label():                                                 //
//     - pipeline.set_label("Example pipeline"); pipeline.label() -> "Example pipeline"                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "shader.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Shader program pipeline.
	class shader_pipeline {
	  public:
		// Creates a shader pipeline.
		shader_pipeline(graphics_context& context, const vertex_shader& vshader, const fragment_shader& fshader);

		// Gets a reference to the graphics context the pipeline is on.
		graphics_context& context() const;

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);
		// Gets the debug label of the pipeline.
		std::string label() const;

	  private:
		// Shader pipeline deleter.
		struct deleter {
			// Reference to the graphics context the pipeline is on.
			graphics_context& context;

			// Deletes the shader pipeline.
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_ppo;

		// Creates a shader pipeline.
		shader_pipeline(graphics_context& context);

		friend class graphics_context;
	};

	// Shader program pipeline that owns its shaders.
	class owning_shader_pipeline {
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