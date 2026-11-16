/// @file
/// @brief Implements event_view.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/event_view.hpp>

//

tr::event_view::event_view(const SDL_Event& event) noexcept
	: m_ref{event}
{
}

//

const SDL_Event& tr::event_view::sdl_event() const noexcept
{
	return m_ref;
}

tr::u32 tr::event_view::type_id() const noexcept
{
	return sdl_event().type;
}
