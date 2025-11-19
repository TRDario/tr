#pragma once
#include "state_machine.hpp"

template <class... States> bool tr::state_machine<States...>::empty() const
{
	return std::holds_alternative<std::monostate>(m_current_state);
}

template <class... States> template <tr::one_of<States...> T> bool tr::state_machine<States...>::is() const
{
	return std::holds_alternative<T>(m_current_state);
}

template <class... States> template <class Visitor> auto tr::state_machine<States...>::visit(Visitor&& visitor) const
{
	return std::visit(std::forward<Visitor>(visitor), m_current_state);
}

//

template <class... States> void tr::state_machine<States...>::clear()
{
	m_current_state = std::monostate{};
}

template <class... States>
template <class T, class... Args>
	requires(std::constructible_from<T, Args...>)
void tr::state_machine<States...>::emplace(Args&&... args)
{
	m_current_state.template emplace<T>(std::forward<T>(args)...);
}

template <class... States> template <class Visitor> auto tr::state_machine<States...>::visit(Visitor&& visitor)
{
	return std::visit(std::forward<Visitor>(visitor), m_current_state);
}

//

template <class... States> struct tr::state_machine<States...>::event_handler {
	std::variant<std::monostate, States...>& current_state;
	const tr::sys::event& event;

	template <class T>
		requires(requires(T state) {
			{ state.handle_event(event) } -> std::same_as<tr::state_machine<States...>::next_state>;
		})
	void operator()(T& state) const
	{
		tr::state_machine<States...>::next_state next{state.handle_event(event)};
		if (std::holds_alternative<drop_state_t>(next)) {
			current_state = std::monostate{};
		}
		else if (!std::holds_alternative<keep_state_t>(next)) {
			current_state = std::move(next);
		}
	}

	void operator()(auto&) const {}
};

template <class... States> void tr::state_machine<States...>::handle_event(const tr::sys::event& event)
{
	std::visit(event_handler{m_current_state, event}, m_current_state);
}

template <class... States> struct tr::state_machine<States...>::tick_handler {
	std::variant<std::monostate, States...>& current_state;

	template <class T>
		requires(requires(T state) {
			{ state.tick() } -> std::same_as<tr::state_machine<States...>::next_state>;
		})
	void operator()(T& state) const
	{
		tr::state_machine<States...>::next_state next{state.tick()};
		if (std::holds_alternative<drop_state_t>(next)) {
			current_state = std::monostate{};
		}
		else if (!std::holds_alternative<keep_state_t>(next)) {
			current_state = std::move(next);
		}
	}

	void operator()(auto&) const {}
};

template <class... States> void tr::state_machine<States...>::tick()
{
	m_tick_benchmark.start();
	std::visit(tick_handler{m_current_state}, m_current_state);
	m_tick_benchmark.stop();
}

template <class... States> struct tr::state_machine<States...>::update_handler {
	std::variant<std::monostate, States...>& current_state;
	duration delta;

	template <class T>
		requires(requires(T state) {
			{ state.update(delta) } -> std::same_as<tr::state_machine<States...>::next_state>;
		})
	void operator()(T& state) const
	{
		tr::state_machine<States...>::next_state next{state.update(delta)};
		if (std::holds_alternative<drop_state_t>(next)) {
			current_state = std::monostate{};
		}
		else if (!std::holds_alternative<keep_state_t>(next)) {
			current_state = std::move(next);
		}
	}

	void operator()(auto&) const {}
};

template <class... States> template <class R, class P> void tr::state_machine<States...>::update(std::chrono::duration<R, P> delta)
{
	m_update_benchmark.start();
	std::visit(update_handler{m_current_state, std::chrono::duration_cast<duration>(delta)}, m_current_state);
	m_update_benchmark.stop();
}

template <class... States> struct tr::state_machine<States...>::draw_handler {
	template <class T>
		requires(requires(T state) { state.draw(); })
	void operator()(T& state) const
	{
		state.draw();
	}

	void operator()(auto&) const {}
};

template <class... States> void tr::state_machine<States...>::draw()
{
	m_draw_benchmark.start();
	std::visit(draw_handler{}, m_current_state);
	m_draw_benchmark.stop();
}