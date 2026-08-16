/// @file
/// @brief Provides `tr::gl_object_registry`.

#pragma once
#include "../utility/common.hpp"

//

namespace tr
{
	/// OpenGL object registry used in checked mode.
	class gl_object_registry
	{
	  public:
		/// Checks if a shader ID pair is valid.
		/// @param tid tr ID of the shader.
		/// @param gid OpenGL shader program ID.
		/// @return `true` if the shader ID pair is valid, `false` otherwise.
		bool check_shader(unsigned int tid, unsigned gid) const;

		/// Registers a shader.
		/// @param tid tr ID of the shader.
		/// @param gid OpenGL shader program ID.
		void register_shader(unsigned int tid, unsigned int gid);

		/// Unregisters a shader.
		/// @param tid tr ID of the shader.
		void unregister_shader(unsigned int tid);

	  private:
		/// Shader tid -> gid map.
		boost::unordered_flat_map<unsigned int, unsigned int> m_shaders;
	};
} // namespace tr