/// @file
/// @brief Provides `tr::internal::framebuffer_info`.

#pragma once
#include <tr/sysgfx/internal/graphics_object_registry.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class graphics_context;
}

//

namespace tr::internal
{
	/// Information about a framebuffer.
	struct framebuffer_info
	{
		/// OpenGL FBO ID of the framebuffer.
		unsigned int fbo;

		/// Size of the framebuffer.
		glm::ivec2 size;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Reference to the context the render target is on.
		ref<const graphics_context> context;

		/// Unique graphics object ID of the framebuffer.
		internal::graphics_object_id id;

		/// Label of the framebuffer.
		std::string label;
#endif
	};
} // namespace tr::internal