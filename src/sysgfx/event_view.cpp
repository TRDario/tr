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

tr::u32 tr::event_view::type() const noexcept
{
	return unwrap().type;
}

//

const SDL_Event& tr::event_view::unwrap() const noexcept
{
	return m_ref;
}