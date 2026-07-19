///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements localization_map.hpp.                                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/localization_map.hpp"
#include "../../include/tr/utility/iostream.hpp"

///////////////////////////////////////////////////////// LOCALIZATION MAP PARSER /////////////////////////////////////////////////////////

namespace tr {
	namespace {
		// Trims whitespace from the line.
		std::string_view trim_whitespace(std::string_view line)
		{
			return {std::ranges::find_if_not(line, [](char c) { return std::isspace(c); }), line.end()};
		}
	} // namespace
} // namespace tr

std::string_view tr::localization_map::parser::parse_key(std::string_view line, std::string_view& out)
{
	std::string_view::iterator end{std::ranges::find_if(line, [](char c) { return !std::isalnum(c) && c != '_'; })};
	if (line.begin() == end) {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected a key.", m_line));
		return {};
	}
	out = {line.begin(), end};
	return trim_whitespace({end, line.end()});
}

std::string_view tr::localization_map::parser::parse_delimiter(std::string_view line)
{
	if (line[0] != '=') {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected '=' after key.", m_line));
		return {};
	}
	return trim_whitespace(line.substr(1));
}

bool tr::localization_map::parser::process_escape_sequences(std::string_view raw, std::string& out)
{
	out.reserve(raw.size());
	for (std::string_view::iterator chr_it = raw.begin(); chr_it != raw.end(); ++chr_it) {
		if (*chr_it == '\\') {
			if (++chr_it == raw.end()) {
				m_errors.emplace_back(TR_FMT::format("line {}: Unterminated escape sequence in value string.", m_line));
				return false;
			}

			if (*chr_it == 'n') {
				out.push_back('\n');
			}
			else if (*chr_it == '\\') {
				out.push_back('\\');
			}
			else if (*chr_it == '"') {
				out.push_back('"');
			}
			else {
				m_errors.emplace_back(TR_FMT::format("line {}: Unknown escape sequence \\{} in value string.", m_line, *chr_it));
				return false;
			}
		}
		else {
			out.push_back(*chr_it);
		}
	}
	out.shrink_to_fit();
	return true;
}

bool tr::localization_map::parser::parse_value(std::string_view line, std::string& out)
{
	if (line[0] != '"') {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected quoted string after '<key> = '.", m_line));
		return false;
	}

	std::string_view::iterator begin{line.begin() + 1};
	std::string_view::iterator end{line.begin()};
	while ((end = std::find(end + 1, line.end(), '"')) != line.end()) {
		int leading_backslashes{0};
		for (std::string_view::iterator it = std::prev(end); *it == '\\' && it != begin; ++leading_backslashes, --it) {}
		if (leading_backslashes % 2 == 0) {
			break;
		}
	}

	if (end == line.end()) {
		m_errors.emplace_back(TR_FMT::format("line {}: Unterminated quoted string.", m_line));
		return false;
	}

	line = trim_whitespace({end + 1, line.end()});
	if (!line.empty() && line[0] != '#') {
		m_errors.emplace_back(TR_FMT::format("line {}: Expected comment or newline after quoted string.", m_line));
		return false;
	}

	return process_escape_sequences({begin, end}, out);
}

std::optional<tr::localization_map::parser::parse_result> tr::localization_map::parser::parse_line(std::string_view line)
{
	++m_line;
	parse_result parsed_line;
	line = trim_whitespace(line);
	if (line.empty() || line[0] == '#') {
		return std::nullopt;
	}
	line = parse_key(line, parsed_line.key);
	if (line.empty()) {
		return std::nullopt;
	}
	line = parse_delimiter(line);
	if (line.empty()) {
		return std::nullopt;
	}
	if (!parse_value(line, parsed_line.value)) {
		return std::nullopt;
	}
	return std::move(parsed_line);
}

std::vector<std::string> tr::localization_map::parser::errors()
{
	return std::move(m_errors);
}

///////////////////////////////////////////////////////////// LOCALIZATION MAP ////////////////////////////////////////////////////////////

tr::localization_map::localization_map(const string_flat_map<std::string>& map)
	: m_map{map}
{
}

tr::localization_map::localization_map(string_flat_map<std::string>&& map)
	: m_map{std::move(map)}
{
}

//

void tr::localization_map::clear()
{
	m_map.clear();
}

std::vector<std::string> tr::localization_map::load_script(std::string_view script)
{
	parser parser;
	while (!script.empty()) {
		const std::string_view::iterator line_end{std::ranges::find(script, '\n')};

		std::optional<parser::parse_result> result{parser.parse_line({script.begin(), line_end})};
		if (result.has_value()) {
			const opt_ref<std::string> value{try_get(m_map, result->key)};
			if (value.has_ref()) {
				*value = std::move(result->value);
			}
			else {
				m_map.emplace(result->key, std::move(result->value));
			}
		}

		script = line_end == script.end() ? std::string_view{} : std::string_view{line_end + 1, script.end()};
	}
	return parser.errors();
}

std::vector<std::string> tr::localization_map::load_script_file(const std::filesystem::path& path)
{
	std::ostringstream ss;
	ss << tr::open_file_r(path).rdbuf();
	return load_script(ss.view());
}

//

bool tr::localization_map::contains(std::string_view key) const
{
	return m_map.contains(key);
}

std::string_view tr::localization_map::operator[](std::string_view key) const
{
	const opt_ref<const std::string> value{try_get(m_map, key)};
	return value.has_ref() ? *value : key;
}