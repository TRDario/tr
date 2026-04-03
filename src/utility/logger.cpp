///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of logger.hpp.                                                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/logger.hpp"
#include "../../include/tr/utility/chrono.hpp"
#include "../../include/tr/utility/iostream.hpp"
#include "../../include/tr/utility/print.hpp"
#include "../../include/tr/utility/ranges.hpp"

////////////////////////////////////////////////////////////// CONSOLE LOGGER /////////////////////////////////////////////////////////////

tr::console_logger::console_logger(std::string&& name)
	: m_name{std::move(name)}
{
	TR_ASSERT(std::ranges::find(g_registered_console_loggers, m_name) == g_registered_console_loggers.end(),
			  "Tried to register duplicate console logger '{}'", m_name);
	g_registered_console_loggers.emplace_back(m_name);
}

tr::console_logger::~console_logger()
{
	unstable_erase(g_registered_console_loggers, std::ranges::find(g_registered_console_loggers, m_name));
}

//

// String size projection.
static tr::usize string_size(const std::string& str)
{
	return str.size();
}

void tr::console_logger::log(const std::tm& time, severity severity, std::string_view string)
{
	const usize padding{std::ranges::max_element(g_registered_console_loggers, std::less{}, string_size)->size() - m_name.size()};
	println("[{:02}:{:02}:{:02}] [{}]{:{}} [{}] {}", time.tm_hour, time.tm_min, time.tm_sec, m_name, "", padding, char(severity), string);
}

void tr::console_logger::log_continue(std::string_view string)
{
	const usize padding{std::ranges::max_element(g_registered_console_loggers, std::less{}, string_size)->size() + 14};
	println("{:{}}--- {}", "", padding, string);
}

/////////////////////////////////////////////////////////////// FILE LOGGER ///////////////////////////////////////////////////////////////

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
	println_to(file, "[{:02}:{:02}:{:02}] [{}] {}", time.tm_hour, time.tm_min, time.tm_sec, char(severity), string);
}

void tr::file_logger::log_continue(std::string_view string)
{
	std::ofstream file{m_path, std::ios::app};
	println_to(file, "           --- {}", string);
}

///////////////////////////////////////////////////////// CONSOLE AND FILE LOGGER /////////////////////////////////////////////////////////

tr::console_and_file_logger::console_and_file_logger(std::string&& name, std::filesystem::path&& path)
	: console_logger{std::move(name)}, file_logger{std::move(path)}
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

////////////////////////////////////////////////////////////////// LOGGER /////////////////////////////////////////////////////////////////

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