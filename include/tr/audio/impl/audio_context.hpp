/// @file
/// @brief Implements the templated parts of audio_context.hpp.

#pragma once
#include "../audio_context.hpp"

//

template <typename T>
void tr::audio_context::create_command(audio_source& source, audio_command<T>::method_type method, T begin, T end, fsecs length)
{
	const std::lock_guard lock{m_mutex};
	m_commands.emplace_back(std::in_place_type_t<audio_command<T>>{}, source, method, begin, end, length);
}

template <typename... Ts>
void tr::audio_context::create_command(audio_source& source, audio_command<Ts...>::method_type method, const std::tuple<Ts...>& begin,
									   const std::tuple<Ts...>& end, fsecs length)
{
	const std::lock_guard lock{m_mutex};
	m_commands.emplace_back(std::in_place_type_t<audio_command<Ts...>>{}, source, method, begin, end, length);
}