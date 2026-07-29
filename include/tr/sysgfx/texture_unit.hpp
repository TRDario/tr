///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an internally used texture unit class.                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

namespace tr {
	class graphics_context;
	class texture_view;
} // namespace tr

namespace tr {
	// A location a texture can bind to.
	class texture_unit {
	  public:
		// Allocates a texture unit on a graphics context.
		texture_unit(graphics_context& context);

		// Gets the ID of the texture unit.
		unsigned int id() const;

		// Sets the texture unit.
		void set(texture_view texture);

	  private:
		// Texture unit freer.
		struct deleter {
			// Reference to the graphics context the texture unit is on.
			graphics_context& context;

			// Frees the texture unit.
			void operator()(unsigned int unit) const;
		};

		// Handle to the texture unit.
		handle<unsigned int, UINT_MAX, deleter> m_handle;
	};
} // namespace tr