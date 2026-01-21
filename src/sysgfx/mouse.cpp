///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements mouse.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/mouse.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

//////////////////////////////////////////////////////////////// MOUSE MODE ///////////////////////////////////////////////////////////////

void tr::sys::set_mouse_mode(mouse_mode mode)
{
	TR_ASSERT(g_window.is_open(), "Tried to set mouse mode before opening the window.");

	if (!SDL_SetWindowRelativeMouseMode(g_window.ptr(), bool(mode))) {
		TR_LOG_SDL_ERROR("Failed to set mouse mode.");
	}

	// Workaround for an SDL bug where the cursor does not properly disappear sometimes in relative mode on Windows.
#ifdef _WIN32
	switch (mode) {
	case mouse_mode::absolute:
		g_window.disable_cursor_resets();
		break;
	case mouse_mode::relative:
		g_window.enable_cursor_resets();
		break;
	}
#endif
}