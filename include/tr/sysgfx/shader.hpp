#pragma once
#include "../utility/exception.hpp"
#include "../utility/handle.hpp"

#ifdef TR_ENABLE_GL_CHECKS
#include "gl_checks.hpp"
#endif

namespace tr::gfx {
	class basic_shader_buffer;
	class basic_uniform_buffer;
	class texture_ref;

	// Error thrown when shader loading fails.
	class shader_load_error : public exception {
	  public:
		// Constructs an exception.
		shader_load_error(std::string_view path, std::string&& details);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string m_description;
		// The details of the error.
		std::string m_details;
	};

	// Base GPU shader program class.
	class shader_base {
	  public:
		// Sets a boolean uniform.
		void set_uniform(int index, bool value);

		// Sets an integer uniform.
		void set_uniform(int index, int value);
		// Sets an integer array uniform.
		void set_uniform(int index, std::span<const int> value);
		// Sets an ivec2 uniform.
		void set_uniform(int index, glm::ivec2 value);
		// Sets an ivec2 array uniform.
		void set_uniform(int index, std::span<const glm::ivec2> value);
		// Sets an ivec3 uniform.
		void set_uniform(int index, glm::ivec3 value);
		// Sets an ivec3 array uniform.
		void set_uniform(int index, std::span<const glm::ivec3> value);
		// Sets an ivec4 uniform.
		void set_uniform(int index, glm::ivec4 value);
		// Sets an ivec4 array uniform.
		void set_uniform(int index, std::span<const glm::ivec4> value);

		// Sets an unsigned integer uniform.
		void set_uniform(int index, unsigned int value);
		// Sets an unsigned integer array uniform.
		void set_uniform(int index, std::span<const unsigned int> value);
		// Sets a uvec2 uniform.
		void set_uniform(int index, glm::uvec2 value);
		// Sets a uvec2 array uniform.
		void set_uniform(int index, std::span<const glm::uvec2> value);
		// Sets a uvec3 uniform.
		void set_uniform(int index, glm::uvec3 value);
		// Sets a uvec3 array uniform.
		void set_uniform(int index, std::span<const glm::uvec3> value);
		// Sets a uvec4 uniform.
		void set_uniform(int index, glm::uvec4 value);
		// Sets a uvec4 array uniform.
		void set_uniform(int index, std::span<const glm::uvec4> value);

		// Sets a float uniform.
		void set_uniform(int index, float value);
		// Sets a float array uniform.
		void set_uniform(int index, std::span<const float> value);
		// Sets a vec2 uniform.
		void set_uniform(int index, glm::vec2 value);
		// Sets a vec2 array uniform.
		void set_uniform(int index, std::span<const glm::vec2> value);
		// Sets a vec3 uniform.
		void set_uniform(int index, glm::vec3 value);
		// Sets a vec3 array uniform.
		void set_uniform(int index, std::span<const glm::vec3> value);
		// Sets a vec4 uniform.
		void set_uniform(int index, glm::vec4 value);
		// Sets a vec4 array uniform.
		void set_uniform(int index, std::span<const glm::vec4> value);

		// Sets a mat2 uniform.
		void set_uniform(int index, const glm::mat2& value);
		// Sets a mat2 array uniform.
		void set_uniform(int index, std::span<const glm::mat2> value);
		// Sets a mat3 uniform.
		void set_uniform(int index, const glm::mat3& value);
		// Sets a mat3 array uniform.
		void set_uniform(int index, std::span<const glm::mat3> value);
		// Sets a mat4 uniform.
		void set_uniform(int index, const glm::mat4& value);
		// Sets a mat4 array uniform.
		void set_uniform(int index, std::span<const glm::mat4> value);
		// Sets a mat2x3 uniform.
		void set_uniform(int index, const glm::mat2x3& value);
		// Sets a mat2x3 array uniform.
		void set_uniform(int index, std::span<const glm::mat2x3> value);
		// Sets a mat2x4 uniform.
		void set_uniform(int index, const glm::mat2x4& value);
		// Sets a mat2x4 array uniform.
		void set_uniform(int index, std::span<const glm::mat2x4> value);
		// Sets a mat3x2 uniform.
		void set_uniform(int index, const glm::mat3x2& value);
		// Sets a mat3x2 array uniform.
		void set_uniform(int index, std::span<const glm::mat3x2> value);
		// Sets a mat3x4 uniform.
		void set_uniform(int index, const glm::mat3x4& value);
		// Sets a mat3x4 uniform.
		void set_uniform(int index, std::span<const glm::mat3x4> value);
		// Sets a mat4x2 uniform.
		void set_uniform(int index, const glm::mat4x2& value);
		// Sets a mat4x2 array uniform.
		void set_uniform(int index, std::span<const glm::mat4x2> value);
		// Sets a mat4x3 uniform.
		void set_uniform(int index, const glm::mat4x3& value);
		// Sets a mat4x3 array uniform.
		void set_uniform(int index, std::span<const glm::mat4x3> value);

		// Sets a texture sampler uniform.
		void set_uniform(int index, texture_ref texture);

		// Sets a shader storage buffer.
		void set_storage_buffer(unsigned int index, basic_shader_buffer& buffer);
		// Sets a uniform storage buffer.
		void set_uniform_buffer(unsigned int index, const basic_uniform_buffer& buffer);

		// Sets the debug label of the shader.
		void set_label(std::string_view label);

	  protected:
		struct texture_unit {
			struct deleter {
				void operator()(unsigned int unit) const;
			};

			handle<unsigned int, UINT_MAX, deleter> id;

			texture_unit();
		};
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL program.
		handle<unsigned int, 0, deleter> m_program;
		// Texture units allocated to this shader.
		std::unordered_map<int, texture_unit> m_texture_units;

		// Constructs a shader.
		shader_base(const char* source, unsigned int type);

		friend class shader_pipeline;
		friend class ping_pong_buffer;

#ifdef TR_ENABLE_GL_CHECKS
		// List of non-block uniforms obtained by introspection.
		std::unordered_map<unsigned int, glsl_variable> m_uniforms;
		// List of input variables obtained by introspection.
		std::unordered_map<unsigned int, glsl_variable> m_inputs;
		// List of output variables obtained by introspection.
		std::unordered_map<unsigned int, glsl_variable> m_outputs;

		void find_uniforms();
		void find_inputs();
		void find_outputs();
#endif
	};

	// GPU vertex shader program.
	class vertex_shader : public shader_base {
	  public:
		// Creates a vertex shader from source code.
		// May throw: shader_load_error.
		explicit vertex_shader(const char* source);
	};
	// Loads a vertex shader from file.
	// May throw: shader_load_error.
	vertex_shader load_vertex_shader(const std::filesystem::path& path);

	// GPU fragment shader program.
	class fragment_shader : public shader_base {
	  public:
		// Creates a fragment shader from source code.
		// May throw: shader_load_error.
		explicit fragment_shader(const char* source);
	};
	// Loads a fragment shader from file.
	// May throw: shader_load_error.
	fragment_shader load_fragment_shader(const std::filesystem::path& path);
} // namespace tr::gfx