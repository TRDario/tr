#pragma once
#include "shader.hpp"

namespace tr {
	// Shader program pipeline.
	class shader_pipeline {
	  public:
		// Creates a shader pipeline.
		shader_pipeline(const vertex_shader& vshader, const fragment_shader& fshader) noexcept;

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label) noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL shader pipeline.
		handle<unsigned int, 0, deleter> _id;

		// Binds the shader pipeline.
		void bind() const noexcept;

		friend struct gfx_context;
	};

	// Shader program pipeline that owns its shaders.
	class owning_shader_pipeline {
	  public:
		// Creates an owning shader pipeline.
		owning_shader_pipeline(vertex_shader&& vshader, fragment_shader&& fshader) noexcept;

		// Gets the base pipeline object.
		operator const shader_pipeline&() const noexcept;
		// Gets the vertex shader.
		vertex_shader& vertex_shader() noexcept;
		// Gets the vertex shader.
		const tr::vertex_shader& vertex_shader() const noexcept;
		// Gets the fragment shader.
		fragment_shader& fragment_shader() noexcept;
		// Gets the fragment shader.
		const tr::fragment_shader& fragment_shader() const noexcept;

		// Sets the debug label of the pipeline.
		void set_label(std::string_view label) noexcept;

	  private:
		// The vertex shader.
		tr::vertex_shader _vshader;
		// The fragment shader.
		tr::fragment_shader _fshader;
		// The base shader pipeline.
		shader_pipeline _pipeline;

		friend struct gfx_context;
	};
} // namespace tr