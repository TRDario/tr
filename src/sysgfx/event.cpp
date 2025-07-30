#include "../../include/tr/sysgfx/event_queue.hpp"
#include "../../include/tr/sysgfx/event_types.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/keyboard_events.hpp"
#include "../../include/tr/sysgfx/mouse_events.hpp"
#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/sysgfx/window_events.hpp"
#include "../../include/tr/utility/timer.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

namespace tr {
	// Converts SDL keymods to tr keymods.
	constexpr keymod convert_keymods(SDL_Keymod mods);
} // namespace tr

constexpr tr::keymod tr::convert_keymods(SDL_Keymod mods)
{
	if (mods & SDL_KMOD_SHIFT) {
		mods |= SDL_KMOD_SHIFT;
	}
	if (mods & SDL_KMOD_CTRL) {
		mods |= SDL_KMOD_CTRL;
	}
	if (mods & SDL_KMOD_ALT) {
		mods |= SDL_KMOD_ALT;
	}
	mods &= (SDL_KMOD_SHIFT | SDL_KMOD_CTRL | SDL_KMOD_ALT);
	return static_cast<keymod>(mods);
}

tr::key_down_event::key_down_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to key_down_event.");

	const SDL_KeyboardEvent& sdl{(reinterpret_cast<const SDL_Event*>(&event))->key};
	repeat = sdl.repeat;
	scan = static_cast<scancode::enum_t>(sdl.scancode);
	key = static_cast<keycode::enum_t>(sdl.key);
	mods = convert_keymods(sdl.mod);
}

bool tr::operator==(const key_down_event& event, const key_chord& chord)
{
	return key_chord{event.key, event.mods} == chord;
}

bool tr::operator==(const key_down_event& event, const scan_chord& chord)
{
	return scan_chord{event.scan, event.mods} == chord;
}

bool tr::operator==(const key_chord& chord, const key_down_event& event)
{
	return key_chord{event.key, event.mods} == chord;
}

bool tr::operator==(const scan_chord& chord, const key_down_event& event)
{
	return scan_chord{event.scan, event.mods} == chord;
}

tr::key_up_event::key_up_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to key_up_event.");

	const SDL_KeyboardEvent& sdl{(reinterpret_cast<const SDL_Event*>(&event))->key};
	scan = static_cast<scancode::enum_t>(sdl.scancode);
	key = static_cast<keycode::enum_t>(sdl.key);
	mods = convert_keymods(sdl.mod);
}

tr::text_input_event::text_input_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to text_input_event.");

	const SDL_TextInputEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->text};
	text = sdl.text;
}

tr::mouse_motion_event::mouse_motion_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_motion_event.");

	const SDL_MouseMotionEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->motion};
	buttons = static_cast<mouse_button>(sdl.state);
	pos = glm::vec2{sdl.x, sdl.y} * window::pixel_density();
	delta = glm::vec2{sdl.xrel, sdl.yrel} * window::pixel_density();
}

tr::mouse_down_event::mouse_down_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_down_event.");

	const SDL_MouseButtonEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->button};
	button = static_cast<mouse_button>(1 << (sdl.button - 1));
	clicks = sdl.clicks;
	pos = glm::vec2{sdl.x, sdl.y} * window::pixel_density();
}

tr::mouse_up_event::mouse_up_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_up_event.");

	const SDL_MouseButtonEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->button};
	button = static_cast<mouse_button>(1 << (sdl.button - 1));
	pos = glm::vec2{sdl.x, sdl.y} * window::pixel_density();
}

tr::mouse_wheel_event::mouse_wheel_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_wheel_event.");

	const SDL_MouseWheelEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->wheel};
	delta = {sdl.x, sdl.y};
	mouse_pos = glm::vec2{sdl.mouse_x, sdl.mouse_y} * window::pixel_density();
}

tr::backbuffer_resize_event::backbuffer_resize_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to backbuffer_resize_event.");

	const SDL_WindowEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->window};
	size = {sdl.data1, sdl.data2};
}

tr::tick_event::tick_event(std::uint32_t id)
	: id{id}
{
}

tr::tick_event::tick_event(const event& event)
	: id{custom_event_base{event}.uint}
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to tick_event.");
}

tr::tick_event::operator tr::event() const
{
	return custom_event_base{ID, id, 0};
}

tr::draw_event::operator tr::event() const
{
	return custom_event_base{ID};
}

tr::custom_event_base::custom_event_base(std::uint32_t type)
	: type{type}, uint{}, sint{}
{
}

tr::custom_event_base::custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint)
	: type{type}, uint{uint}, sint{sint}
{
}

tr::custom_event_base::custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, std::any&& any1, std::any&& any2)
	: type{type}, uint{uint}, sint{sint}, any1{std::move(any1)}, any2{std::move(any2)}
{
}

tr::custom_event_base::custom_event_base(const event& event)
{
	TR_ASSERT(event.type() >= SDL_EVENT_USER, "Tried to cast a non-user event into a user event base.");

	const SDL_UserEvent& sdl{reinterpret_cast<const SDL_Event*>(&event)->user};
	type = sdl.type;
	uint = sdl.windowID;
	sint = sdl.code;
	if (sdl.data1 != nullptr) {
		any1 = *static_cast<const std::any*>(sdl.data1);
	}
	if (sdl.data2 != nullptr) {
		any2 = *static_cast<const std::any*>(sdl.data2);
	}
}

tr::custom_event_base::custom_event_base(event&& event)
{
	TR_ASSERT(event.type() >= SDL_EVENT_USER, "Tried to cast a non-user event into a user event base.");

	SDL_UserEvent& sdl{reinterpret_cast<SDL_Event*>(&event)->user};
	type = sdl.type;
	uint = sdl.windowID;
	sint = sdl.code;
	if (sdl.data1 != nullptr) {
		any1 = std::move(*static_cast<std::any*>(sdl.data1));
	}
	if (sdl.data2 != nullptr) {
		any2 = std::move(*static_cast<std::any*>(sdl.data2));
	}
	sdl.type = 0;
}

tr::custom_event_base::operator tr::event() const&
{
	event event;
	SDL_Event& sdl{*reinterpret_cast<SDL_Event*>(&event)};
	sdl.type = type;
	sdl.user.windowID = uint;
	sdl.user.code = sint;
	sdl.user.data1 = any1.has_value() ? new std::any{any1} : nullptr;
	sdl.user.data2 = any2.has_value() ? new std::any{any2} : nullptr;
	return event;
}

tr::custom_event_base::operator tr::event() &&
{
	event event;
	SDL_Event& sdl{*reinterpret_cast<SDL_Event*>(&event)};
	sdl.type = type;
	sdl.user.windowID = uint;
	sdl.user.code = sint;
	sdl.user.data1 = any1.has_value() ? new std::any{std::move(any1)} : nullptr;
	sdl.user.data2 = any2.has_value() ? new std::any{std::move(any2)} : nullptr;
	return event;
}

tr::event::event(const event& r)
{
	std::ranges::copy(r.buffer, buffer);
	if (type() >= SDL_EVENT_USER) {
		SDL_UserEvent& lsdl{reinterpret_cast<SDL_Event*>(buffer)->user};
		const SDL_UserEvent& rsdl{reinterpret_cast<const SDL_Event*>(r.buffer)->user};
		lsdl.data1 = lsdl.data1 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data1)} : nullptr;
		lsdl.data2 = lsdl.data2 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data2)} : nullptr;
	}
}

tr::event::event(event&& r) noexcept
{
	std::ranges::copy(r.buffer, buffer);
	if (type() >= SDL_EVENT_USER) {
		SDL_UserEvent& rsdl{reinterpret_cast<SDL_Event*>(r.buffer)->user};
		rsdl.data1 = nullptr;
		rsdl.data2 = nullptr;
		rsdl.type = 0;
	}
}

tr::event::~event()
{
	if (type() >= SDL_EVENT_USER) {
		SDL_UserEvent& sdl{reinterpret_cast<SDL_Event*>(buffer)->user};
		delete static_cast<std::any*>(sdl.data1);
		delete static_cast<std::any*>(sdl.data2);
	}
}

tr::event& tr::event::operator=(const event& r)
{
	std::ignore = std::move(*this);
	std::ranges::copy(r.buffer, buffer);
	if (type() >= SDL_EVENT_USER) {
		SDL_UserEvent& lsdl{reinterpret_cast<SDL_Event*>(buffer)->user};
		const SDL_UserEvent& rsdl{reinterpret_cast<const SDL_Event*>(r.buffer)->user};
		lsdl.data1 = lsdl.data1 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data1)} : nullptr;
		lsdl.data2 = lsdl.data2 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data2)} : nullptr;
	}
	return *this;
}

tr::event& tr::event::operator=(event&& r) noexcept
{
	std::ignore = std::move(*this);
	std::ranges::copy(r.buffer, buffer);
	if (type() >= SDL_EVENT_USER) {
		SDL_UserEvent& rsdl{reinterpret_cast<SDL_Event*>(r.buffer)->user};
		rsdl.data1 = nullptr;
		rsdl.data2 = nullptr;
	}
	return *this;
}

std::uint32_t tr::event::type() const
{
	return (reinterpret_cast<const SDL_Event*>(buffer))->type;
}

tr::timer tr::create_tick_timer(float frequency, std::uint32_t id)
{
	return timer{1.0s / frequency, [=] { event_queue::push(tick_event{id}); }};
}

tr::timer tr::create_draw_timer(float frequency)
{
	return timer{1.0s / frequency, [] { event_queue::push(draw_event{}); }};
}

std::optional<tr::event> tr::event_queue::poll()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to poll for events before opening the window.");

	event event;
	return SDL_PollEvent(reinterpret_cast<SDL_Event*>(&event)) ? std::optional<tr::event>{std::move(event)} : std::nullopt;
}

tr::event tr::event_queue::wait()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to wait for events before opening the window.");

	event event;
	SDL_WaitEvent(reinterpret_cast<SDL_Event*>(&event));
	return event;
}

std::optional<tr::event> tr::event_queue::wait(imsecs timeout)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to wait for events before opening the window.");

	event event;
	if (SDL_WaitEventTimeout(reinterpret_cast<SDL_Event*>(&event), static_cast<Sint32>(timeout.count()))) {
		return event;
	}
	else {
		return std::nullopt;
	}
}

void tr::event_queue::enable_text_input_events()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to enable text input event sending before opening the window.");

	SDL_StartTextInput(sdl_window);
}

void tr::event_queue::disable_text_input_events()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to disable text input event sending before opening the window.");

	SDL_StopTextInput(sdl_window);
}

void tr::event_queue::push(const event& event)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to push an event before opening the window.");

	SDL_Event sdl{*reinterpret_cast<const SDL_Event*>(&event)};
	if (event.type() >= SDL_EVENT_USER) {
		const SDL_UserEvent& rsdl{reinterpret_cast<const SDL_Event*>(&event)->user};
		sdl.user.data1 = rsdl.data1 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data1)} : nullptr;
		sdl.user.data2 = rsdl.data2 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data2)} : nullptr;
	}

	if (!SDL_PushEvent(&sdl)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to push event.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::event_queue::push(event&& event)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to push an event before opening the window.");

	SDL_Event sdl{*reinterpret_cast<const SDL_Event*>(&event)};
	if (!SDL_PushEvent(&sdl)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to push event.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}

	if (event.type() >= SDL_EVENT_USER) {
		SDL_UserEvent& rsdl{reinterpret_cast<SDL_Event*>(&event)->user};
		rsdl.data1 = nullptr;
		rsdl.data2 = nullptr;
	}
}