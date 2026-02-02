///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides shader pipeline classes.                                                                                                     //
//                                                                                                                                       //
// Shader pipelines are an abstraction over OpenGL program pipeline objects.                                                             //
//                                                                                                                                       //
// tr::gfx::shader_pipeline is an object that references a shader of each stage and combines them into a single pipeline. It does not    //
// take ownership of the shaders passed to it. A pipeline can be created with just the vertex and fragment shaders, or with the two      //
// tessellation shader stages included:                                                                                                  //
//     - tr::gfx::shader_pipeline{vshader, fshader} -> creates a pipeline with vertex and fragment shader stages                         //
//     - tr::gfx::shader_pipeline{vshader, tcshader, teshader, fshader} -> creates a pipeline with tessellation stages included          //
//                                                                                                                                       //
// tr::gfx::owning_shader_pipeline and tr::gfx::owning_tessellation_shader_pipeline are objects that *do* own the shader stages within   //
// them. After construction, these shaders can be accessed from the pipeline:                                                            //
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
		// Creates a shader pipeline with tessellation stages.
		shader_pipeline(const vertex_shader& vshader, const tessellation_control_shader& tcshader,
						const tessellation_evaluation_shader& teshader, const fragment_shader& fshader);

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

	// Shader program pipeline with tessellation stages that owns its shaders.
	class owning_tessellation_shader_pipeline {
	  public:
		// Creates an owning shader pipeline with tessellation stages.
		owning_tessellation_shader_pipeline(vertex_shader&& vshader, tessellation_control_shader&& tcshader,
											tessellation_evaluation_shader&& teshader, fragment_shader&& fshader);

		// Gets the base pipeline object.
		operator const shader_pipeline&() const;
		// Gets the vertex shader.
		vertex_shader& vertex_shader();
		// Gets the vertex shader.
		const gfx::vertex_shader& vertex_shader() const;
		// Gets the tessellation control shader.
		tessellation_control_shader& tessellation_control_shader();
		// Gets the tessellation control shader.
		const gfx::tessellation_control_shader& tessellation_control_shader() const;
		// Gets the tessellation evaluation shader.
		tessellation_evaluation_shader& tessellation_evaluation_shader();
		// Gets the tessellation evaluation shader.
		const gfx::tessellation_evaluation_shader& tessellation_evaluation_shader() const;
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
		// The tessellation control shader.
		gfx::tessellation_control_shader m_tcshader;
		// The tessellation evaluation shader.
		gfx::tessellation_evaluation_shader m_teshader;
		// The fragment shader.
		gfx::fragment_shader m_fshader;
		// The base shader pipeline.
		shader_pipeline m_base;
	};
} // namespace tr::gfx