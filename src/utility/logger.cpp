#include "../../include/tr/utility/logger.hpp"

tr::logger::logger(std::string_view prefix)
	: m_prefix{prefix}
{
}

tr::logger::logger(std::string_view prefix, std::filesystem::path path)
	: m_prefix{prefix}, m_path{std::move(path)}
{
	try {
		open_file_w(path, std::ios::trunc);
	}
	catch (...) {
		return;
	}
}

bool tr::logger::active() const
{
	return !m_prefix.empty();
}

void tr::logger::log(severity level, const std::exception& err)
{
	const exception* tr_exception{dynamic_cast<const exception*>(&err)};
	if (tr_exception != nullptr) {
		log(level, "Exception raised: {}.", tr_exception->name());
		const std::string_view description{tr_exception->description()};
		const std::string_view details{tr_exception->details()};
		if (!description.empty()) {
			log_continue("{}", description);
		}
		if (!details.empty()) {
			log_continue("{}", details);
		}
	}
	else {
		log(level, "Exception raised:");
		log_continue("{}", err.what());
	}
}

void tr::logger::log_continue(const std::exception& err)
{
	log_continue("{}", err.what());
}