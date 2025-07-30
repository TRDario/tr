#pragma once
#include "../utility/handle.hpp"

namespace tr {
	class texture_ref;
	class dyn_atlas;
} // namespace tr

namespace tr {
	// A texture unit onto which textures can be bound.
	class texture_unit {
	  public:
		// Constructs a new texture unit.
		texture_unit();

		// Binds a texture to the texture unit.
		void set_texture(texture_ref texture);
		// Binds a dynamic atlas texture to the texture unit.
		void set_texture(const dyn_atlas& atlas);

	  private:
		struct deleter {
			void operator()(unsigned int id);
		};

		// Handle to the ID of the texture unit.
		handle<unsigned int, std::numeric_limits<unsigned int>::max(), deleter> unit;

		friend class shader_base;
	};
} // namespace tr
