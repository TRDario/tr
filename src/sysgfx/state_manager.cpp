#include "../../include/tr/sysgfx/state_manager.hpp"

tr::u32 tr::state::type() const
{
	return NO_ID;
}

tr::u32 tr::drop_state::type() const
{
	return std::numeric_limits<u32>::max();
}

std::unique_ptr<tr::state> tr::drop_state::handle_event(const sys::event&)
{
	return nullptr;
}

std::unique_ptr<tr::state> tr::drop_state::update(duration)
{
	return nullptr;
}

void tr::drop_state::draw() {}

tr::state_manager::state_manager(std::unique_ptr<tr::state>&& state)
	: state{std::move(state)}
{
}

const tr::benchmark& tr::state_manager::update_benchmark() const
{
	return m_update;
}

const tr::benchmark& tr::state_manager::draw_benchmark() const
{
	return m_draw;
}

void tr::state_manager::handle_event(const sys::event& event)
{
	if (state != nullptr) {
		std::unique_ptr<tr::state> next{state->handle_event(event)};
		if (next != nullptr) {
			state = next->type() != UINT32_MAX ? std::move(next) : nullptr;
		}
	}
}

void tr::state_manager::update(duration delta)
{
	if (state != nullptr) {
		m_update.start();
		std::unique_ptr<tr::state> next{state->update(delta)};
		m_update.stop();
		if (next != nullptr) {
			state = next->type() != UINT32_MAX ? std::move(next) : nullptr;
			m_update.clear();
			m_draw.clear();
		}
	}
}

void tr::state_manager::draw()
{
	if (state != nullptr) {
		m_draw.start();
		state->draw();
		m_draw.stop();
	}
}