#pragma once
#include "../utility/exception.hpp"
#include "../utility/handle.hpp"

namespace tr {
	class shader_buffer;
	class texture_unit;
} // namespace tr

namespace tr {
	// Error thrown when shader loading fails.
	class shader_load_error : public exception {
	  public:
		// Constructs an exception.
		shader_load_error(std::string_view location, std::string_view path, std::string&& details) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The description of the error.
		std::string _description;
		// The details of the error.
		std::string _details;
	};

	// Base GPU shader program class.
	class shader_base {
	  public:
		// Sets a boolean uniform.
		void set_uniform(int index, bool value) noexcept;
		// Sets a bvec2 uniform.
		void set_uniform(int index, glm::bvec2 value) noexcept;
		// Sets a bvec3 uniform.
		void set_uniform(int index, glm::bvec3 value) noexcept;
		// Sets a bvec4 uniform.
		void set_uniform(int index, glm::bvec4 value) noexcept;

		// Sets an integer uniform.
		void set_uniform(int index, int value) noexcept;
		// Sets an integer array uniform.
		void set_uniform(int index, std::span<const int> value) noexcept;
		// Sets an ivec2 uniform.
		void set_uniform(int index, glm::ivec2 value) noexcept;
		// Sets an ivec2 array uniform.
		void set_uniform(int index, std::span<const glm::ivec2> value) noexcept;
		// Sets an ivec3 uniform.
		void set_uniform(int index, glm::ivec3 value) noexcept;
		// Sets an ivec3 array uniform.
		void set_uniform(int index, std::span<const glm::ivec3> value) noexcept;
		// Sets an ivec4 uniform.
		void set_uniform(int index, glm::ivec4 value) noexcept;
		// Sets an ivec4 array uniform.
		void set_uniform(int index, std::span<const glm::ivec4> value) noexcept;

		// Sets an unsigned integer uniform.
		void set_uniform(int index, unsigned int value) noexcept;
		// Sets an unsigned integer array uniform.
		void set_uniform(int index, std::span<const unsigned int> value) noexcept;
		// Sets a uvec2 uniform.
		void set_uniform(int index, glm::uvec2 value) noexcept;
		// Sets a uvec2 array uniform.
		void set_uniform(int index, std::span<const glm::uvec2> value) noexcept;
		// Sets a uvec3 uniform.
		void set_uniform(int index, glm::uvec3 value) noexcept;
		// Sets a uvec3 array uniform.
		void set_uniform(int index, std::span<const glm::uvec3> value) noexcept;
		// Sets a uvec4 uniform.
		void set_uniform(int index, glm::uvec4 value) noexcept;
		// Sets a uvec4 array uniform.
		void set_uniform(int index, std::span<const glm::uvec4> value) noexcept;

		// Sets a float uniform.
		void set_uniform(int index, float value) noexcept;
		// Sets a float array uniform.
		void set_uniform(int index, std::span<const float> value) noexcept;
		// Sets a vec2 uniform.
		void set_uniform(int index, glm::vec2 value) noexcept;
		// Sets a vec2 array uniform.
		void set_uniform(int index, std::span<const glm::vec2> value) noexcept;
		// Sets a vec3 uniform.
		void set_uniform(int index, glm::vec3 value) noexcept;
		// Sets a vec3 array uniform.
		void set_uniform(int index, std::span<const glm::vec3> value) noexcept;
		// Sets a vec4 uniform.
		void set_uniform(int index, glm::vec4 value) noexcept;
		// Sets a vec4 array uniform.
		void set_uniform(int index, std::span<const glm::vec4> value) noexcept;

		// Sets a mat2 uniform.
		void set_uniform(int index, const glm::mat2& value) noexcept;
		// Sets a mat2 array uniform.
		void set_uniform(int index, std::span<const glm::mat2> value) noexcept;
		// Sets a mat3 uniform.
		void set_uniform(int index, const glm::mat3& value) noexcept;
		// Sets a mat3 array uniform.
		void set_uniform(int index, std::span<const glm::mat3> value) noexcept;
		// Sets a mat4 uniform.
		void set_uniform(int index, const glm::mat4& value) noexcept;
		// Sets a mat4 array uniform.
		void set_uniform(int index, std::span<const glm::mat4> value) noexcept;
		// Sets a mat2x3 uniform.
		void set_uniform(int index, const glm::mat2x3& value) noexcept;
		// Sets a mat2x3 array uniform.
		void set_uniform(int index, std::span<const glm::mat2x3> value) noexcept;
		// Sets a mat2x4 uniform.
		void set_uniform(int index, const glm::mat2x4& value) noexcept;
		// Sets a mat2x4 array uniform.
		void set_uniform(int index, std::span<const glm::mat2x4> value) noexcept;
		// Sets a mat3x2 uniform.
		void set_uniform(int index, const glm::mat3x2& value) noexcept;
		// Sets a mat3x2 array uniform.
		void set_uniform(int index, std::span<const glm::mat3x2> value) noexcept;
		// Sets a mat3x4 uniform.
		void set_uniform(int index, const glm::mat3x4& value) noexcept;
		// Sets a mat3x4 uniform.
		void set_uniform(int index, std::span<const glm::mat3x4> value) noexcept;
		// Sets a mat4x2 uniform.
		void set_uniform(int index, const glm::mat4x2& value) noexcept;
		// Sets a mat4x2 array uniform.
		void set_uniform(int index, std::span<const glm::mat4x2> value) noexcept;
		// Sets a mat4x3 uniform.
		void set_uniform(int index, const glm::mat4x3& value) noexcept;
		// Sets a mat4x3 array uniform.
		void set_uniform(int index, std::span<const glm::mat4x3> value) noexcept;

		// Sets a texture sampler uniform.
		void set_uniform(int index, const texture_unit& unit) noexcept;

		// Sets a shader storage buffer.
		void set_storage_buffer(unsigned int index, shader_buffer& buffer) noexcept;

		// Sets the debug label of the shader.
		void set_label(std::string_view label) noexcept;

	  protected:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL program.
		handle<unsigned int, 0, deleter> _id;

		// Constructs a shader.
		shader_base(const char* source, unsigned int type);

		friend class shader_pipeline;
		friend class ping_pong_buffer;
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
} // namespace tr