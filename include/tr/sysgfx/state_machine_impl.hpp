#pragma once
#include "state_machine.hpp"

template <class... States> bool tr::static_state_machine<States...>::empty() const
{
	return std::holds_alternative<std::monostate>(m_current_state);
}

template <class... States> template <tr::one_of<States...> T> bool tr::static_state_machine<States...>::is() const
{
	return std::holds_alternative<T>(m_current_state);
}

template <class... States> template <tr::one_of<States...> T> const T& tr::static_state_machine<States...>::get() const
{
	return unchecked_get<T>(m_current_state);
}

template <class... States> template <class Visitor> auto tr::static_state_machine<States...>::visit(Visitor&& visitor) const
{
	return std::visit(std::forward<Visitor>(visitor), m_current_state);
}

//

template <class... States> const tr::benchmark& tr::static_state_machine<States...>::tick_benchmark() const
{
	return m_tick_benchmark;
}

template <class... States> const tr::benchmark& tr::static_state_machine<States...>::update_benchmark() const
{
	return m_update_benchmark;
}

template <class... States> const tr::benchmark& tr::static_state_machine<States...>::draw_benchmark() const
{
	return m_draw_benchmark;
}

//

template <class... States> void tr::static_state_machine<States...>::clear()
{
	m_current_state = std::monostate{};
}

template <class... States>
template <class T, class... Args>
	requires(std::constructible_from<T, Args...>)
void tr::static_state_machine<States...>::emplace(Args&&... args)
{
	m_current_state.template emplace<T>(std::forward<T>(args)...);
}

template <class... States> template <tr::one_of<States...> T> T& tr::static_state_machine<States...>::get()
{
	return unchecked_get<T>(m_current_state);
}

template <class... States> template <class Visitor> auto tr::static_state_machine<States...>::visit(Visitor&& visitor)
{
	return std::visit(std::forward<Visitor>(visitor), m_current_state);
}

//

template <class... States> struct tr::static_state_machine<States...>::next_state_assigner {
	std::variant<std::monostate, States...>& current_state;

	void operator()(drop_state_t) const
	{
		current_state = std::monostate{};
	}

	void operator()(keep_state_t) const {}

	void operator()(auto&& v) const
	{
		current_state = std::move(v);
	}
};

template <class... States> struct tr::static_state_machine<States...>::event_handler {
	std::variant<std::monostate, States...>& current_state;
	const tr::sys::event& event;

	template <class T>
		requires(requires(T state) {
			{ state.handle_event(event) } -> std::same_as<tr::static_state_machine<States...>::next_state>;
		})
	void operator()(T& state) const
	{
		std::visit(next_state_assigner{current_state}, state.handle_event(event));
	}

	void operator()(auto&) const {}
};

template <class... States> void tr::static_state_machine<States...>::handle_event(const tr::sys::event& event)
{
	std::visit(event_handler{m_current_state, event}, m_current_state);
}

template <class... States> struct tr::static_state_machine<States...>::tick_handler {
	std::variant<std::monostate, States...>& current_state;

	template <class T>
		requires(requires(T state) {
			{ state.tick() } -> std::same_as<tr::static_state_machine<States...>::next_state>;
		})
	void operator()(T& state) const
	{
		std::visit(next_state_assigner{current_state}, state.tick());
	}

	void operator()(auto&) const {}
};

template <class... States> void tr::static_state_machine<States...>::tick()
{
	m_tick_benchmark.start();
	std::visit(tick_handler{m_current_state}, m_current_state);
	m_tick_benchmark.stop();
}

template <class... States> struct tr::static_state_machine<States...>::update_handler {
	std::variant<std::monostate, States...>& current_state;
	duration delta;

	template <class T>
		requires(requires(T state) {
			{ state.update(delta) } -> std::same_as<tr::static_state_machine<States...>::next_state>;
		})
	void operator()(T& state) const
	{
		std::visit(next_state_assigner{current_state}, state.update(delta));
	}

	void operator()(auto&) const {}
};

template <class... States> template <class R, class P> void tr::static_state_machine<States...>::update(std::chrono::duration<R, P> delta)
{
	m_update_benchmark.start();
	std::visit(update_handler{m_current_state, std::chrono::duration_cast<duration>(delta)}, m_current_state);
	m_update_benchmark.stop();
}

template <class... States> struct tr::static_state_machine<States...>::draw_handler {
	template <class T>
		requires(requires(T state) { state.draw(); })
	void operator()(T& state) const
	{
		state.draw();
	}

	void operator()(auto&) const {}
};

template <class... States> void tr::static_state_machine<States...>::draw()
{
	m_draw_benchmark.start();
	std::visit(draw_handler{}, m_current_state);
	m_draw_benchmark.stop();
}

//

template <class T, class... Args>
	requires(std::constructible_from<T, Args...>)
void tr::dyn_state_machine::emplace(Args&&... args);

// Does a delta-time update on the state.
template <class R, class P> void tr::dyn_state_machine::update(std::chrono::duration<R, P> delta)
{
	std::unique_ptr<state> next{m_current_state->update(std::chrono::duration_cast<duration>(delta))};
	if ()
}