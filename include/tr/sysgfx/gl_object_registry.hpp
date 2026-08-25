/// @file
/// @brief Provides `tr::gl_object_registry`.

#pragma once
#include "../utility/integer.hpp"

//

namespace tr
{
	/// Opaque ID for graphics objects.
	enum class graphics_object_id : unsigned int
	{
		/// Sentinel value for an invalid graphics object ID.
		invalid
	};

	/// Generates a unique graphics object ID.
	/// @details This function is guaranteed to be thread-safe.
	/// @return New unique graphics object ID.
	graphics_object_id generate_graphics_object_id();

	//

	/// OpenGL object registry used in checked mode.
	class gl_object_registry
	{
	  public:
		/// Gets the number of registered shaders.
		/// @return Number of registered shaders.
		usize registered_shader_count() const;

		/// Checks if a shader ID is valid.
		/// @param id Graphics object ID of the shader.
		/// @return `true` if the shader ID is valid, `false` otherwise.
		bool is_shader_valid(graphics_object_id id) const;

		/// Registers a shader.
		/// @param id Graphics object ID of the shader.
		/// @param glid OpenGL shader program ID.
		void register_shader(graphics_object_id id, unsigned int glid);

		/// Unregisters a shader.
		/// @param id Graphics object ID of the shader.
		void unregister_shader(graphics_object_id id);

	  private:
		/// Shader id -> glid map.
		boost::unordered_flat_map<graphics_object_id, unsigned int> m_shaders;
	};
} // namespace tr