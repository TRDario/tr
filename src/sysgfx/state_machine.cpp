#include "../../include/tr/sysgfx/state_machine.hpp"

std::optional<std::unique_ptr<tr::state>> tr::state::handle_event(const sys::event&)
{
	return KEEP_STATE;
}

std::optional<std::unique_ptr<tr::state>> tr::state::tick()
{
	return KEEP_STATE;
}

std::optional<std::unique_ptr<tr::state>> tr::state::update(duration)
{
	return KEEP_STATE;
}

void tr::state::draw() {}

//

bool tr::state_machine::empty() const
{
	return m_current_state == nullptr;
}

const tr::benchmark& tr::state_machine::tick_benchmark() const
{
	return m_tick_benchmark;
}

const tr::benchmark& tr::state_machine::update_benchmark() const
{
	return m_update_benchmark;
}

const tr::benchmark& tr::state_machine::draw_benchmark() const
{
	return m_draw_benchmark;
}

//

void tr::state_machine::clear()
{
	m_current_state.reset();
}

//

void tr::state_machine::handle_event(const sys::event& event)
{
	if (m_current_state != nullptr) {
		std::optional<std::unique_ptr<tr::state>> next{m_current_state->handle_event(event)};
		if (next.has_value()) {
			m_current_state = *std::move(next);
		}
	}
}

void tr::state_machine::tick()
{
	if (m_current_state != nullptr) {
		m_tick_benchmark.start();
		std::optional<std::unique_ptr<tr::state>> next{m_current_state->tick()};
		m_tick_benchmark.stop();
		if (next.has_value()) {
			m_current_state = *std::move(next);
		}
	}
}

void tr::state_machine::draw()
{
	if (m_current_state != nullptr) {
		m_draw_benchmark.start();
		m_current_state->draw();
		m_draw_benchmark.stop();
	}
}