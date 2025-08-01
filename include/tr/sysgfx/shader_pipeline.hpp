#pragma once
#include "shader.hpp"

namespace tr::gfx {
	// Shader program pipeline.
	class shader_pipeline {
	  public:
		// Creates a shader pipeline.
		shader_pipeline(const vertex_shader& vshader, const fragment_shader& fshader);

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> m_ppo;

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

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);

	  private:
		// The vertex shader.
		gfx::vertex_shader m_vshader;
		// The fragment shader.
		gfx::fragment_shader m_fshader;
		// The base shader pipeline.
		shader_pipeline m_base;
	};
} // namespace tr::gfx