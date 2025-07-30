#pragma once
#include "../utility/handle.hpp"

namespace tr {
	class texture_ref;
} // namespace tr

namespace tr {
	// A texture unit onto which textures can be bound.
	class texture_unit {
	  public:
		// Constructs a new texture unit.
		texture_unit();

		// Binds a texture to the texture unit.
		void set_texture(texture_ref texture);

	  private:
		struct deleter {
			void operator()(unsigned int id);
		};

		// Handle to the ID of the texture unit.
		handle<unsigned int, std::numeric_limits<unsigned int>::max(), deleter> unit;

		friend class shader_base;
	};

	// Replaces all bindings of a certain texture with another.
	void replace_texture_bindings(texture_ref old, texture_ref fresh);
} // namespace tr
