///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of state_machine.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../state_machine.hpp"

////////////////////////////////////////////////////////////////// STATE //////////////////////////////////////////////////////////////////

consteval tr::next_state tr::keep_state()
{
	return keep_state_t{};
}

consteval tr::next_state tr::drop_state()
{
	return drop_state_t{};
}

template <std::derived_from<tr::state> State, typename... Args>
	requires(std::constructible_from<State, Args...>)
tr::next_state tr::make_next_state(Args&&... args)
{
	return static_cast<std::unique_ptr<state>>(std::make_unique<State>(std::forward<Args>(args)...));
}

////////////////////////////////////////////////////////////// STATE MACHINE //////////////////////////////////////////////////////////////

template <std::derived_from<tr::state> State> const State& tr::state_machine::get() const
{
	return static_cast<const State&>(*m_current_state);
}

template <std::derived_from<tr::state> State, typename... Args>
	requires(std::constructible_from<State, Args...>)
void tr::state_machine::emplace(Args&&... args)
{
	m_current_state = std::make_unique<State>(std::forward<Args>(args)...);
}

template <std::derived_from<tr::state> State> State& tr::state_machine::get()
{
	return static_cast<State&>(*m_current_state);
}

template <typename Rep, typename Period> void tr::state_machine::update(std::chrono::duration<Rep, Period> delta)
{
	if (m_current_state != nullptr) {
		m_update_benchmark.start();
		next_state next{m_current_state->update(std::chrono::duration_cast<duration>(delta))};
		m_update_benchmark.stop();
		std::visit([this](auto&& next_state) { handle_next_state(std::move(next_state)); }, std::move(next));
	}
}