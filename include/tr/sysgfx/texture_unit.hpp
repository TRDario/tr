#pragma once
#include "../utility/handle.hpp"

namespace tr::gfx {
	class texture_ref;

	// A texture unit onto which textures can be bound.
	class texture_unit {
	  public:
		// Constructs a new texture unit.
		texture_unit();

		// Binds a texture to the texture unit.
		void set_texture(texture_ref ref);

	  private:
		struct deleter {
			void operator()(unsigned int id);
		};

		// Handle to the ID of the texture unit.
		handle<unsigned int, UINT_MAX, deleter> m_unit;

		friend class shader_base;
	};
} // namespace tr::gfx
