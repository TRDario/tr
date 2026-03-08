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

template <std::derived_from<tr::state> T, typename... Args>
	requires(std::constructible_from<T, Args...>)
tr::next_state tr::make_next_state(Args&&... args)
{
	return (std::unique_ptr<state>)std::make_unique<T>(std::forward<Args>(args)...);
}

////////////////////////////////////////////////////////////// STATE MACHINE //////////////////////////////////////////////////////////////

template <std::derived_from<tr::state> T> const T& tr::state_machine::get() const
{
	return (const T&)*m_current_state;
}

template <std::derived_from<tr::state> T, typename... Args>
	requires(std::constructible_from<T, Args...>)
void tr::state_machine::emplace(Args&&... args)
{
	m_current_state = std::make_unique<T>(std::forward<Args>(args)...);
}

template <std::derived_from<tr::state> T> T& tr::state_machine::get()
{
	return (T&)*m_current_state;
}

template <typename R, typename P> void tr::state_machine::update(std::chrono::duration<R, P> delta)
{
	if (m_current_state != nullptr) {
		m_update_benchmark.start();
		next_state next{m_current_state->update(std::chrono::duration_cast<duration>(delta))};
		m_update_benchmark.stop();
		std::visit(next_state_handler{m_current_state}, std::move(next));
	}
}