#include "../../include/tr/utility/logger.hpp"

tr::logger::logger(std::string_view prefix)
	: prefix{prefix}
{
}

tr::logger::logger(std::string_view prefix, std::filesystem::path path)
	: prefix{prefix}, path{std::move(path)}
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
	return !prefix.empty();
}

void tr::logger::log(severity level, const exception& err)
{
	log(level, "Exception raised: {}.", err.name());
	const std::string_view description{err.description()};
	const std::string_view details{err.details()};
	if (!description.empty()) {
		log_continue("{}", description);
	}
	if (!details.empty()) {
		log_continue("{}", details);
	}
}

void tr::logger::log(severity level, const std::exception& err)
{
	log(level, "Exception raised:");
	log_continue("{}", err.what());
}