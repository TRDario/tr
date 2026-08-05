/// @file
/// @brief Implements audio_command.hpp.

#pragma once
#include "../audio_command.hpp"

//

template <typename First, typename... Rest>
tr::audio_command<First, Rest...>::audio_command(audio_source& source, method_type method, const value_type& begin, const value_type& end,
												 fsecs length)
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

template <typename First, typename... Rest>
tr::audio_source& tr::audio_command<First, Rest...>::source() const
{
	return m_source;
}

//

namespace tr
{
	/// Linearly interpolates a set of values memberwise.
	/// @tparam Ts Value types.
	/// @tparam Is Value indices.
	/// @param begin, end Sample values.
	/// @param ratio Interpolation factor.
	/// @return `begin + ratio * (end − begin)`.
	template <typename... Ts, usize... Is>
	std::tuple<Ts...> lerp_tuple(const std::tuple<Ts...>& begin, const std::tuple<Ts...>& end, float ratio, std::index_sequence<Is...>)
	{
		return {tr::lerp(std::get<Is>(begin), std::get<Is>(end), ratio)...};
	}
} // namespace tr

template <typename First, typename... Rest>
tr::audio_command<First, Rest...>::status tr::audio_command<First, Rest...>::execute()
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