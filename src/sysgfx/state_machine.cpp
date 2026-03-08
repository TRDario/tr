///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of state_machine.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/state_machine.hpp"

////////////////////////////////////////////////////////////////// STATE //////////////////////////////////////////////////////////////////

tr::next_state tr::state::handle_event(const sys::event&)
{
	return keep_state{};
}

tr::next_state tr::state::update(duration)
{
	return keep_state{};
}

void tr::state::draw() {}

//////////////////////////////////////////////////////////// NEXT STATE HANDLER ///////////////////////////////////////////////////////////

void tr::state_machine::next_state_handler::operator()(keep_state) {}

void tr::state_machine::next_state_handler::operator()(drop_state)
{
	m_current_state.reset();
}

void tr::state_machine::next_state_handler::operator()(std::unique_ptr<state>&& next)
{
	m_current_state = std::move(next);
}

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

void tr::state_machine::handle_event(const sys::event& event)
{
	if (m_current_state != nullptr) {
		std::visit(next_state_handler{m_current_state}, m_current_state->handle_event(event));
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