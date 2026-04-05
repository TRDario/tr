///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of logger.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../logger.hpp"

////////////////////////////////////////////////////////////////// LOGGER /////////////////////////////////////////////////////////////////

template <std::derived_from<tr::logger_backend> Backend, typename... Args>
	requires(std::constructible_from<Backend, Args...>)
tr::logger tr::make_logger(Args&&... args)
{
	return tr::logger{std::make_unique<Backend>(std::forward<Args>(args)...)};
}

//

template <std::derived_from<tr::logger_backend> Backend, typename... Args>
	requires(std::constructible_from<Backend, Args...>)
void tr::logger::replace_backend_with(Args&&... args)
{
	m_backend.reset();
	m_backend = std::make_unique<Backend>(std::forward<Args>(args)...);
}

//

template <typename... Args> void tr::logger::log(severity severity, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	log(severity, std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args> void tr::logger::log_continue(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	log_continue(std::format(fmt, std::forward<Args>(args)...));
}