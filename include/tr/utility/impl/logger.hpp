///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of logger.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../chrono.hpp"
#include "../logger.hpp"
#include "../print.hpp"

////////////////////////////////////////////////////////////////// LOGGER /////////////////////////////////////////////////////////////////

template <typename... Args> void tr::logger::log(severity level, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	const std::tm time{tr::localtime(std::time(nullptr))};
	const std::string time_str{TR_FMT::format("{:02}:{:02}:{:02}", time.tm_hour, time.tm_min, time.tm_sec)};
	const std::string fmt_str{TR_FMT::format(fmt, std::forward<Args>(args)...)};

	println("[{}] [{}] [{}] {}", time_str, m_prefix, char(level), fmt_str);
	if (!m_path.empty()) {
		std::ofstream file{m_path, std::ios::app};
		if (file.is_open()) {
			println_to(file, "[{}] [{}] {}", time_str, char(level), fmt_str);
		}
	}
}

template <typename... Args> void tr::logger::log_continue(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	const std::string fmt_str{TR_FMT::format(fmt, std::forward<Args>(args)...)};
	const std::string fill(m_prefix.size() + 14, ' ');

	println("{}--- {}", fill, fmt_str);
	if (!m_path.empty()) {
		std::ofstream file{m_path, std::ios::app};
		if (file.is_open()) {
			println_to(file, "{}--- {}", std::string_view{fill.begin(), fill.end() - (m_prefix.size() + 3)}, fmt_str);
		}
	}
}