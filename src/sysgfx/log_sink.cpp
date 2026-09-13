/// @file
/// @brief Implements log_sink.hpp.

#include <tr/sysgfx/log_message.hpp>
#include <tr/sysgfx/log_sink.hpp>
#include <tr/utility/iostream.hpp>

//

tr::stdout_log_sink::~stdout_log_sink() noexcept
{
	try {
		std::cout.flush();
	}
	catch (...) {
	}
}

void tr::stdout_log_sink::log(const log_message& message)
{
	std::format_to(std::ostreambuf_iterator<char>{std::cout}, "{}\n", message);
}

void tr::stdout_log_sink::flush()
{
	std::cout.flush();
}

//

tr::directory_log_sink::directory_log_sink(std::filesystem::path directory_path)
	: m_directory_path{std::move(directory_path)}
{
	namespace fs = std::filesystem;

	if (!fs::exists(m_directory_path)) {
		fs::create_directory(m_directory_path);
		return;
	}

	const fs::path old_path{m_directory_path / "old"};
	const fs::path older_path{m_directory_path / "older"};
	const fs::path oldest_path{m_directory_path / "oldest"};
	if (fs::exists(oldest_path)) {
		fs::remove_all(oldest_path);
	}
	if (fs::exists(older_path)) {
		fs::rename(older_path, oldest_path);
	}
	if (fs::exists(old_path)) {
		fs::rename(old_path, older_path);
	}
	fs::create_directory(old_path);
	for (const fs::directory_entry& entry : fs::directory_iterator{m_directory_path}) {
		if (entry.is_regular_file()) {
			const fs::path entry_path{entry.path()};
			fs::rename(entry_path, old_path / entry_path.filename());
		}
	}
}

void tr::directory_log_sink::log(const log_message& message) noexcept
{
	try {
		std::ofstream file{open_file_w(m_directory_path / std::format("{}.log", message.source), std::ios::app)};
		std::format_to(std::ostreambuf_iterator<char>{file}, "{:n}\n", message);
	}
	catch (...) {
	}
}