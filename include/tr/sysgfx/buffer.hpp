///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an internally-used RAII wrapper over a generic OpenGL buffer object.                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Wrapper around a generic OpenGL buffer object.
	class buffer {
	  public:
		// Constructs a buffer.
		buffer();

		// Gets the buffer's ID.
		unsigned int id() const;
		// Reallocates the buffer while preserving its label (if applicable).
		void reallocate();

#ifdef TR_ENABLE_ASSERTS
		// Gets the buffer's label.
		std::string label() const;
		// Sets the buffer's label.
		void set_label(std::string_view label);
#endif

	  private:
		// Handle deleter.
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_handle;
	};
} // namespace tr::gfx