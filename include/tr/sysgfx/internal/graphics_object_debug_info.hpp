/// @file
/// @brief Provides debug information structures for graphics objects.

#pragma once
#include <tr/sysgfx/internal/glsl_variable.hpp>
#include <tr/sysgfx/internal/graphics_object_registry.hpp>

//

namespace tr::internal
{
	/// Vertex shader debug information.
	struct vertex_shader_debug_info
	{
		/// Unique graphics object ID of the shader.
		internal::graphics_object_id id{internal::graphics_object_id::invalid};

		/// Label of the shader.
		std::string label{"<unset>"};

		/// Outputs of the shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> outputs;
	};

	/// Fragment shader debug information.
	struct fragment_shader_debug_info
	{
		/// Unique graphics object ID of the shader.
		internal::graphics_object_id id{internal::graphics_object_id::invalid};

		/// Label of the shader.
		std::string label{"<unset>"};

		/// Inputs of the shader.
		boost::unordered_flat_map<unsigned int, internal::glsl_variable> inputs;
	};
} // namespace tr::internal