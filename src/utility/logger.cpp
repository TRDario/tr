/// @file
/// @brief Implements the non-templated parts of logger.hpp.

#include "../../include/tr/utility/logger.hpp"
#include "../../include/tr/utility/chrono.hpp"
#include "../../include/tr/utility/iostream.hpp"
#include "../../include/tr/utility/ranges.hpp"

//

namespace tr
{
	namespace
	{
		/// Must be a function because of the static object initialization fiasco.
		/// @return Reference to the list of registered console loggers.
		std::vector<std::string>& registered_console_loggers()
		{
			static std::vector<std::string> registered_console_loggers;
			return registered_console_loggers;
		}
	} // namespace
} // namespace tr

tr::console_logger::console_logger(std::string&& name)
	: m_name{std::move(name)}
{
	TR_ASSERT(!std::ranges::contains(registered_console_loggers(), m_name), "Tried to register duplicate console logger '{}'", m_name);

	registered_console_loggers().emplace_back(m_name);
}

tr::console_logger::~console_logger()
{
	unstable_erase(registered_console_loggers(), std::ranges::find(registered_console_loggers(), m_name));
}

//

namespace tr
{
	namespace
	{
		/// String size projection.
		/// @param str String to get the size of.
		/// @return Size of the string.
		usize string_size(const std::string& str)
		{
			return str.size();
		}
	} // namespace
} // namespace tr

void tr::console_logger::log(const std::tm& time, severity severity, std::string_view string)
{
	const usize padding{std::ranges::max_element(registered_console_loggers(), std::less{}, string_size)->size() - m_name.size()};
	std::println("[{:02}:{:02}:{:02}] [{}]{:{}} [{}] {}", time.tm_hour, time.tm_min, time.tm_sec, m_name, "", padding,
				 static_cast<char>(severity), string);
}

void tr::console_logger::log_continue(std::string_view string)
{
	const usize padding{std::ranges::max_element(registered_console_loggers(), std::less{}, string_size)->size() + 14};
	std::println("{:{}}--- {}", "", padding, string);
}

//

tr::file_logger::file_logger(std::filesystem::path&& path)
	: m_path{std::move(path)}
{
	try {
		open_file_w(m_path, std::ios::trunc);
	}
	catch (...) {
		return;
	}
}

void tr::file_logger::log(const std::tm& time, severity severity, std::string_view string)
{
	std::ofstream file{m_path, std::ios::app};
	std::println(file, "[{:02}:{:02}:{:02}] [{}] {}", time.tm_hour, time.tm_min, time.tm_sec, char(severity), string);
}

void tr::file_logger::log_continue(std::string_view string)
{
	std::ofstream file{m_path, std::ios::app};
	std::println(file, "           --- {}", string);
}

//

tr::console_and_file_logger::console_and_file_logger(std::string&& name, std::filesystem::path&& path)
	: console_logger{std::move(name)}
	, file_logger{std::move(path)}
{
}

//

void tr::console_and_file_logger::log(const std::tm& time, severity severity, std::string_view string)
{
	console_logger::log(time, severity, string);
	file_logger::log(time, severity, string);
}

void tr::console_and_file_logger::log_continue(std::string_view string)
{
	console_logger::log_continue(string);
	file_logger::log_continue(string);
}

//

tr::logger::logger() {}

tr::logger::logger(std::unique_ptr<logger_backend>&& backend)
	: m_backend{std::move(backend)}
{
}

//

bool tr::logger::active() const
{
	return m_backend != nullptr;
}

const tr::logger_backend& tr::logger::backend() const
{
	return *m_backend;
}

tr::logger_backend& tr::logger::backend()
{
	return *m_backend;
}

//

void tr::logger::clear_backend()
{
	m_backend.reset();
}

//

void tr::logger::log(severity severity, std::string_view str)
{
	m_backend->log(tr::localtime(std::time(nullptr)), severity, str);
}

void tr::logger::log(severity severity, const std::exception& err)
{
	const exception* tr_exception{dynamic_cast<const exception*>(&err)};
	if (tr_exception != nullptr) {
		log(severity, "Exception raised: {}.", tr_exception->name());
		const std::string_view description{tr_exception->description()};
		const std::string_view details{tr_exception->details()};
		if (!description.empty()) {
			log_continue(description);
		}
		if (!details.empty()) {
			log_continue(details);
		}
	}
	else {
		log(severity, "Exception raised:");
		log_continue(err.what());
	}
}

void tr::logger::log_continue(std::string_view str)
{
	m_backend->log_continue(str);
}

void tr::logger::log_continue(const std::exception& err)
{
	log_continue(err.what());
}