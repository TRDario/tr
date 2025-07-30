#pragma once
#include "shader.hpp"

namespace tr {
	class shader_pipeline;
	namespace gfx_context {
		void set_shader_pipeline(const shader_pipeline& pipeline);
	}

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
		handle<unsigned int, 0, deleter> ppo;

		friend void gfx_context::set_shader_pipeline(const shader_pipeline& pipeline);
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
		const tr::vertex_shader& vertex_shader() const;
		// Gets the fragment shader.
		fragment_shader& fragment_shader();
		// Gets the fragment shader.
		const tr::fragment_shader& fragment_shader() const;

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label);

	  private:
		// The vertex shader.
		tr::vertex_shader vshader;
		// The fragment shader.
		tr::fragment_shader fshader;
		// The base shader pipeline.
		shader_pipeline base;
	};
} // namespace tr