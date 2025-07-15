#include "../../include/tr/utility/localization_map.hpp"

namespace tr {
	// Whether to skip a line.
	bool _skip_line(const std::string& line) noexcept;
	// Validates a delimiter.
	bool _validate_delimiter(std::size_t delimiter, std::size_t lineSize, std::vector<std::string>& errors, int line_number);
	// Validates a key.
	bool _validate_key(std::string_view key, const string_hash_map<std::string>& map, std::vector<std::string>& errors, int line_number);
	// Processes a value.
	std::string _process_value(std::string_view raw_value, std::vector<std::string>& errors, int line_number);
} // namespace tr

bool tr::_skip_line(const std::string& line) noexcept
{
	constexpr auto WHITESPACE{[](char c) { return std::isspace(c); }};

	return line.empty() || std::ranges::all_of(line, WHITESPACE) || *std::ranges::find_if_not(line, WHITESPACE) == '#';
}

bool tr::_validate_delimiter(std::size_t delimiter, std::size_t lineSize, std::vector<std::string>& errors, int line_number)
{
	if (delimiter == std::string::npos) {
		errors.emplace_back(std::format("line {}: Expected a delimiting colon.", line_number));
		return false;
	}
	else if (delimiter == 0) {
		errors.emplace_back(std::format("line {}: Expected a key string before the delimiting colon.", line_number));
		return false;
	}
	else if (delimiter == lineSize - 1) {
		errors.emplace_back(std::format("line {}: Expected a value string after the delimiting colon.", line_number));
		return false;
	}
	else {
		return true;
	}
}

bool tr::_validate_key(std::string_view key, const string_hash_map<std::string>& map, std::vector<std::string>& errors, int line_number)
{
	if (map.contains(key)) {
		errors.emplace_back(std::format("line {}: Duplicate key '{}'.", line_number, key));
		return false;
	}
	else {
		return true;
	}
}

std::string tr::_process_value(std::string_view raw_value, std::vector<std::string>& errors, int line)
{
	std::string value;
	value.reserve(raw_value.size());
	for (std::string_view::iterator it = raw_value.begin(); it != raw_value.end(); ++it) {
		if (*it == '\\') {
			if (++it == raw_value.end()) {
				errors.emplace_back(std::format("line {}: Unterminated escape sequence in value string.", line));
				break;
			}

			if (*it == 'n') {
				value.push_back('\n');
			}
			else if (*it == '\\') {
				value.push_back('\\');
			}
			else if (*it == '"') {
				value.push_back('"');
			}
			else {
				errors.emplace_back(std::format("line {}: Unknown escape sequence \\{} in value string.", line, *it));
			}
		}
		else {
			value.push_back(*it);
		}
	}
	value.shrink_to_fit();
	return value;
}

tr::localization_load_error::localization_load_error(std::string&& description) noexcept
	: _description{description}
{
}

std::string_view tr::localization_load_error::name() const noexcept
{
	return "Unrecoverable localization loading error";
}

std::string_view tr::localization_load_error::description() const noexcept
{
	return _description;
}

std::string_view tr::localization_load_error::details() const noexcept
{
	return {};
}

tr::localization_map::localization_map(const string_hash_map<std::string>& map) noexcept
	: _map{map}
{
}

tr::localization_map::localization_map(string_hash_map<std::string>&& map) noexcept
	: _map{std::move(map)}
{
}

void tr::localization_map::clear() noexcept
{
	_map.clear();
}

std::vector<std::string> tr::localization_map::load(const std::filesystem::path& path)
{
	try {
		std::ifstream file{open_file_r(path)};
		std::vector<std::string> errors;

		std::string buffer;
		for (int line = 1; !file.eof(); ++line) {
			std::getline(file, buffer);
			if (_skip_line(buffer)) {
				continue;
			}

			constexpr auto IS_WS{[](char c) { return std::isspace(c); }};
			constexpr auto NOT_ALNUM{[](char c) { return !std::isalnum(c) && c != '_'; }};

			std::string::iterator begin{std::find_if_not(buffer.begin(), buffer.end(), IS_WS)};
			std::string::iterator end{std::find_if(begin, buffer.end(), NOT_ALNUM)};
			if (begin == end) {
				errors.emplace_back(std::format("line {}: Expected a key.", line));
				continue;
			}
			std::string_view key{begin, end};
			if (_map.contains(key)) {
				errors.emplace_back(std::format("line {}: Duplicate key '{}'.", line, key));
				continue;
			}

			begin = std::find_if_not(end, buffer.end(), IS_WS);
			if (begin == buffer.end() || *begin != '=') {
				errors.emplace_back(std::format("line {}: Expected '=' after key.", line));
				continue;
			}

			begin = std::find_if_not(begin + 1, buffer.end(), IS_WS);
			if (begin == buffer.end() || *begin != '"') {
				errors.emplace_back(std::format("line {}: Expected quoted string after '[key] = '.", line));
				continue;
			}
			++begin;
			end = begin;
			while ((end = std::find(end + 1, buffer.end(), '"')) != buffer.end()) {
				if (*std::prev(end) != '\\') {
					break;
				}
			}
			if (end == buffer.end()) {
				errors.emplace_back(std::format("line {}: Unterminated quoted string.", line));
				continue;
			}
			if (!std::all_of(end + 1, buffer.end(), IS_WS) && *std::find_if_not(end + 1, buffer.end(), IS_WS) != '#') {
				errors.emplace_back(std::format("line {}: Expected comment or newline after quoted string.", line));
				continue;
			}
			const std::string_view value{begin, end};

			_map.emplace(key, _process_value(value, errors, line));
		}

		return errors;
	}
	catch (file_open_error& err) {
		throw localization_load_error{std::format("{}: {}", err.name(), err.description())};
	}
	catch (file_not_found& err) {
		throw localization_load_error{std::format("{}: {}", err.name(), err.description())};
	}
	catch (std::bad_alloc&) {
		throw localization_load_error{"Failed to allocate memory while loading the localization."};
	}
}

bool tr::localization_map::contains(std::string_view key) const noexcept
{
	return _map.contains(key);
}

std::string_view tr::localization_map::operator[](std::string_view key) const noexcept
{
	const string_hash_map<std::string>::const_iterator it{_map.find(key)};
	return it != _map.end() ? it->second : key;
}