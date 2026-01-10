#include "../../include/tr/utility/localization_map.hpp"

tr::localization_load_error::localization_load_error(std::string&& description)
	: m_description{description}
{
}

std::string_view tr::localization_load_error::name() const
{
	return "Unrecoverable localization loading error";
}

std::string_view tr::localization_load_error::description() const
{
	return m_description;
}

std::string_view tr::localization_load_error::details() const
{
	return {};
}

//

std::string::iterator tr::localization_map::parser::skip_whitespace(std::string::iterator it)
{
	return std::find_if_not(it, m_buffer.end(), [](char c) { return std::isspace(c); });
}

bool tr::localization_map::parser::read_line(std::istream& is)
{
	if (reached_eof(is)) {
		return false;
	}

	++m_line;
	std::getline(is, m_buffer);
	m_cur = skip_whitespace(m_buffer.begin());
	return true;
}

bool tr::localization_map::parser::parse_key(std::string_view& out)
{
	std::string::iterator end{std::find_if(m_cur, m_buffer.end(), [](char c) { return !std::isalnum(c) && c != '_'; })};
	if (m_cur == end) {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected a key.", m_line));
		return false;
	}
	out = {m_cur, end};
	m_cur = skip_whitespace(end);
	return true;
}

bool tr::localization_map::parser::parse_delimiter()
{
	if (m_cur == m_buffer.end() || *m_cur != '=') {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected '=' after key.", m_line));
		return false;
	}
	m_cur = skip_whitespace(m_cur + 1);
	return true;
}

bool tr::localization_map::parser::process_escape_sequences(std::string& out, std::string_view raw)
{
	out.reserve(raw.size());
	for (std::string_view::iterator it = raw.begin(); it != raw.end(); ++it) {
		if (*it == '\\') {
			if (++it == raw.end()) {
				m_errors.emplace_back(TR_FMT::format("line {}: Unterminated escape sequence in value string.", m_line));
				return false;
			}

			if (*it == 'n') {
				out.push_back('\n');
			}
			else if (*it == '\\') {
				out.push_back('\\');
			}
			else if (*it == '"') {
				out.push_back('"');
			}
			else {
				m_errors.emplace_back(TR_FMT::format("line {}: Unknown escape sequence \\{} in value string.", m_line, *it));
				return false;
			}
		}
		else {
			out.push_back(*it);
		}
	}
	out.shrink_to_fit();
	return true;
}

bool tr::localization_map::parser::parse_value(std::string& out)
{
	if (m_cur == m_buffer.end() || *m_cur++ != '"') {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected quoted string after '<key> = '.", m_line));
		return false;
	}

	std::string::iterator end{m_cur};
	while ((end = std::find(end + 1, m_buffer.end(), '"')) != m_buffer.end()) {
		if (*std::prev(end) != '\\') {
			break;
		}
	}

	if (end == m_buffer.end()) {
		m_errors.emplace_back(TR_FMT::format("line {}: Unterminated quoted string.", m_line));
		return false;
	}

	m_cur = skip_whitespace(end + 1);
	if (m_cur != m_buffer.end() && *m_cur != '#') {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected comment or newline after quoted string.", m_line));
		return false;
	}

	return process_escape_sequences(out, {m_cur, end});
}

std::optional<tr::localization_map::parser::parse_result> tr::localization_map::parser::parse_line()
{
	parse_result parsed_line;
	if (m_cur == m_buffer.end() || *m_cur == '#' || !parse_key(parsed_line.key) || !parse_delimiter() || !parse_value(parsed_line.value)) {
		return std::nullopt;
	}
	return std::move(parsed_line);
}

std::vector<std::string> tr::localization_map::parser::errors()
{
	return std::move(m_errors);
}

//

tr::localization_map::localization_map(const string_hash_map<std::string>& map)
	: m_map{map}
{
}

tr::localization_map::localization_map(string_hash_map<std::string>&& map)
	: m_map{std::move(map)}
{
}

void tr::localization_map::clear()
{
	m_map.clear();
}

std::vector<std::string> tr::localization_map::load(const std::filesystem::path& path)
{
	try {
		parser parser;
		std::ifstream file{open_file_r(path)};
		while (parser.read_line(file)) {
			std::optional<parser::parse_result> result{parser.parse_line()};
			if (result.has_value()) {
				if (m_map.contains(result->key)) {
					m_map.find(result->key)->second = std::move(result->value);
				}
				else {
					m_map.emplace(result->key, std::move(result->value));
				}
			}
		}
		return parser.errors();
	}
	catch (file_open_error& err) {
		throw localization_load_error{TR_FMT::format("{}: {}", err.name(), err.description())};
	}
	catch (file_not_found& err) {
		throw localization_load_error{TR_FMT::format("{}: {}", err.name(), err.description())};
	}
}

bool tr::localization_map::contains(std::string_view key) const
{
	return m_map.contains(key);
}

std::string_view tr::localization_map::operator[](std::string_view key) const
{
	const string_hash_map<std::string>::const_iterator it{m_map.find(key)};
	return it != m_map.end() ? it->second : key;
}