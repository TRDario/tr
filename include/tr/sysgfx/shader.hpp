///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides shader classes.                                                                                                              //
//                                                                                                                                       //
// Shaders are an abstraction over OpenGL separate program objects.                                                                      //
//                                                                                                                                       //
// Shaders can be constructed directly from GLSL source code, or loaded from a file:                                                     //
//     - tr::gfx::vertex_shader{src} -> constructs a vertex shader from an embedded source code string                                   //
//     - tr::gfx::load_vertex_shader("source.vert") -> loads a vertex shader from a source file                                          //
//     - tr::gfx::tessellation_control_shader{src} -> constructs a tessellation control shader from an embedded source code string       //
//     - tr::gfx::load_tessellation_control_shader("source.tesc") -> loads a tessellation control shader from a source file              //
//     - tr::gfx::tessellation_evaluation_shader{src} -> constructs a tessellation evaluation shader from an embedded source code string //
//     - tr::gfx::load_tessellation_evaluation_shader("source.tese") -> loads a tessellation evaluation shader from a source file        //
//     - tr::gfx::fragment_shader{src} -> constructs a fragment shader from an embedded source code string                               //
//     - tr::gfx::load_fragment_shader("source.frag") -> loads a fragment shader from a source file                                      //
//                                                                                                                                       //
// Setting shader uniforms of any GLSL type except doubles is supported:                                                                 //
//     - shader.set_uniform(0, glm::vec2{100, 100}) -> sets the vec2 uniform at location 0                                               //
//                                                                                                                                       //
// The label of a shader can be set with TR_SET_LABEL(shader, label):                                                                    //
//     - TR_SET_LABEL(shader, "Example shader") -> 'atlas' is now labelled "Example shader"                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
} // namespace tr::gfx

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
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

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the shader.
		void set_label(std::string_view label);
		// Gets the debug label of the shader.
		std::string label() const;
#endif

	  protected:
		// OpenGL texture unit.
		class texture_unit {
		  public:
			struct deleter {
				void operator()(unsigned int unit) const;
			};

			// The ID of the texture unit.
			handle<unsigned int, UINT_MAX, deleter> id;

			// Allocates a texture unit.
			texture_unit();

		  private:
			// Tracks which units are allocated.
			static std::array<bool, 80> g_texture_units;
		};
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL program.
		handle<unsigned int, 0, deleter> m_program;
		// Texture units allocated to this shader.
		std::unordered_map<int, texture_unit> m_texture_units;

		// Constructs a shader.
		shader_base(cstring_view source, unsigned int type);

		friend class shader_pipeline;
		friend class ping_pong_buffer;

#ifdef TR_ENABLE_GL_CHECKS
		// List of non-block uniforms obtained by introspection.
		std::unordered_map<unsigned int, glsl_variable> m_uniforms;
		// List of input variables obtained by introspection.
		std::unordered_map<unsigned int, glsl_variable> m_inputs;
		// List of output variables obtained by introspection.
		std::unordered_map<unsigned int, glsl_variable> m_outputs;

		// Finds the uniforms of the shader using introspection.
		void find_uniforms();
		// Finds the input variables of the shader using introspection.
		void find_inputs();
		// Finds the output variables of the shader using introspection.
		void find_outputs();
#endif
	};

	// GPU vertex shader program.
	class vertex_shader : public shader_base {
	  public:
		// Creates a vertex shader from source code.
		// May throw: shader_load_error.
		explicit vertex_shader(cstring_view source);
	};
	// Loads a vertex shader from file.
	// May throw: shader_load_error.
	vertex_shader load_vertex_shader(const std::filesystem::path& path);

	// GPU tessellation control shader program.
	class tessellation_control_shader : public shader_base {
	  public:
		// Creates a tessellation control shader from source code.
		// May throw: shader_load_error.
		explicit tessellation_control_shader(cstring_view source);
	};
	// Loads a tessellation control shader from file.
	// May throw: shader_load_error.
	tessellation_control_shader load_tessellation_control_shader(const std::filesystem::path& path);

	// GPU tessellation evaluation shader program.
	class tessellation_evaluation_shader : public shader_base {
	  public:
		// Creates a tessellation evaluation shader from source code.
		// May throw: shader_load_error.
		explicit tessellation_evaluation_shader(cstring_view source);
	};
	// Loads a tessellation evaluation shader from file.
	// May throw: shader_load_error.
	tessellation_evaluation_shader load_tessellation_evaluation_shader(const std::filesystem::path& path);

	// GPU fragment shader program.
	class fragment_shader : public shader_base {
	  public:
		// Creates a fragment shader from source code.
		// May throw: shader_load_error.
		explicit fragment_shader(cstring_view source);
	};
	// Loads a fragment shader from file.
	// May throw: shader_load_error.
	fragment_shader load_fragment_shader(const std::filesystem::path& path);
} // namespace tr::gfx