///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of state_machine.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/state_machine.hpp"

////////////////////////////////////////////////////////////////// STATE //////////////////////////////////////////////////////////////////

tr::next_state tr::state::handle_event(const event&)
{
	return keep_state();
}

tr::next_state tr::state::update(duration)
{
	return keep_state();
}

void tr::state::draw() {}

////////////////////////////////////////////////////////////// STATE MACHINE //////////////////////////////////////////////////////////////

bool tr::state_machine::empty() const
{
	return m_current_state == nullptr;
}

const tr::benchmark& tr::state_machine::update_benchmark() const
{
	return m_update_benchmark;
}

const tr::benchmark& tr::state_machine::draw_benchmark() const
{
	return m_draw_benchmark;
}

void tr::state_machine::clear()
{
	m_current_state.reset();
}

void tr::state_machine::handle_event(const event& event)
{
	if (m_current_state != nullptr) {
		m_current_state->handle_event(event) | stateful_match{
												   *this,
												   &state_machine::keep_state,
												   &state_machine::drop_state,
												   &state_machine::assign_state,
											   };
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

//

void tr::state_machine::keep_state(keep_state_t) {}

void tr::state_machine::drop_state(drop_state_t)
{
	m_current_state.reset();
}

void tr::state_machine::assign_state(std::unique_ptr<state>&& next)
{
	m_current_state = std::move(next);
}