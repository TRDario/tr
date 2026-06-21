///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of audio_context.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../audio_context.hpp"

/////////////////////////////////////////////////////////// AUDIO CONTEXT COMMAND /////////////////////////////////////////////////////////

template <typename First, typename... Rest>
tr::audio_context::command<First, Rest...>::command(audio_source& source, method_type method, const value_type& begin,
													const value_type& end, fsecs length)
	: m_source{source}
	, m_method{method}
	, m_begin{begin}
	, m_end{end}
	, m_length{std::chrono::duration_cast<duration>(length)}
	, m_last_update{std::chrono::steady_clock::now()}
	, m_elapsed{}
{
}

//

template <typename First, typename... Rest> tr::audio_source& tr::audio_context::command<First, Rest...>::source() const
{
	return m_source;
}

template <typename First, typename... Rest>
void tr::audio_context::command<First, Rest...>::reset_moved_source_reference(audio_source& source)
{
	m_source = source;
}

//

template <typename... Ts, tr::usize... Is>
static std::tuple<Ts...> lerp_tuple(const std::tuple<Ts...>& begin, const std::tuple<Ts...>& end, float ratio, std::index_sequence<Is...>)
{
	return {tr::lerp(std::get<Is>(begin), std::get<Is>(end), ratio)...};
}

template <typename First, typename... Rest>
tr::audio_context::command<First, Rest...>::status tr::audio_context::command<First, Rest...>::execute()
{
	const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
	m_elapsed = std::min(m_elapsed + now - m_last_update, m_length);
	m_last_update = now;

	const float t{ratio(m_elapsed, m_length)};
	if constexpr (sizeof...(Rest) > 0) {
		std::apply([this](First first, Rest... rest) { ((*m_source).*m_method)(first, rest...); },
				   lerp_tuple(m_begin, m_end, t, std::index_sequence_for<First, Rest...>{}));
	}
	else {
		((*m_source).*m_method)(lerp(m_begin, m_end, t));
	}

	return (m_elapsed == m_length) ? status::done : status::ongoing;
}

////////////////////////////////////////////////////////////// AUDIO CONTEXT //////////////////////////////////////////////////////////////

template <typename T>
void tr::audio_context::create_command(audio_source& source, command<T>::method_type method, T begin, T end, fsecs length)
{
	const std::lock_guard lock{m_mutex};
	m_commands.emplace_back(std::in_place_type_t<command<T>>{}, source, method, begin, end, length);
}

template <typename... Ts>
void tr::audio_context::create_command(audio_source& source, command<Ts...>::method_type method, const std::tuple<Ts...>& begin,
									   const std::tuple<Ts...>& end, fsecs length)
{
	const std::lock_guard lock{m_mutex};
	m_commands.emplace_back(std::in_place_type_t<command<Ts...>>{}, source, method, begin, end, length);
}