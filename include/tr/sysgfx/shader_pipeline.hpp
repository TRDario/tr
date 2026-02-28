///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides shader pipeline classes.                                                                                                     //
//                                                                                                                                       //
// Shader pipelines are an abstraction over OpenGL program pipeline objects.                                                             //
//                                                                                                                                       //
// tr::gfx::shader_pipeline is an object that references a shader of each stage and combines them into a single pipeline. It does not    //
// take ownership of the shaders passed to it:                                                                                           //
//     - tr::gfx::shader_pipeline{vshader, fshader} -> creates a pipeline with vertex and fragment shader stages                         //
//                                                                                                                                       //
// tr::gfx::owning_shader_pipeline is an object that *does* own the shader stages within it. After construction, these shaders can be    //
// accessed from the pipeline:                                                                                                           //
//     - tr::gfx::owning_shader_pipeline pipeline{tr::gfx::vertex_shader{vshader_src}, tr::gfx::fragment_shader{fshader_src}}            //
//       -> creates vertex and fragment shaders and then stores them in the pipeline                                                     //
//     - pipeline.vertex_shader().set_uniform(0, 5.0f) -> sets uniform 0 of the vertex shader of the pipeline                            //
//                                                                                                                                       //
// The label of a shader pipeline can be set with TR_SET_LABEL(pipeline, label):                                                         //
//     - TR_SET_LABEL(pipeline, "Example pipeline") -> 'pipeline' is now labelled "Example pipeline"                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "shader.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Shader program pipeline.
	class shader_pipeline {
	  public:
		// Creates a shader pipeline.
		shader_pipeline(const vertex_shader& vshader, const fragment_shader& fshader);

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);
		// Gets the debug label of the pipeline.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_ppo;

		// Creates a shader pipeline.
		shader_pipeline();

		friend void set_shader_pipeline(const shader_pipeline& pipeline);
	};

	// Shader program pipeline that owns its shaders.
	class owning_shader_pipeline {
	  public:
		// Creates an owning shader pipeline.
		owning_shader_pipeline(vertex_shader&& vshader, fragment_shader&& fshader);

		// Gets the base pipeline object.
		operator const shader_pipeline&() const;
		// Gets the vertex shader.
		vertex_shader& vertex_shader();
		// Gets the vertex shader.
		const gfx::vertex_shader& vertex_shader() const;
		// Gets the fragment shader.
		fragment_shader& fragment_shader();
		// Gets the fragment shader.
		const gfx::fragment_shader& fragment_shader() const;

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);
		// Gets the debug label of the pipeline.
		std::string label() const;
#endif

	  private:
		// The vertex shader.
		gfx::vertex_shader m_vshader;
		// The fragment shader.
		gfx::fragment_shader m_fshader;
		// The base shader pipeline.
		shader_pipeline m_base;
	};
} // namespace tr::gfx