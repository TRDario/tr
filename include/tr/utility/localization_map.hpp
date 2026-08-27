/// @file
/// @brief Provides `tr::localization_map`.

#pragma once
#include "hash_map.hpp"

//

namespace tr
{
	/// Localization map class with support for reading custom localization files.
	/// @details
	/// The localization file consists of lines in the format: `[<KEY> = "<VALUE>"] [#COMMENT]`
	///
	/// Empty lines and lines consisting entirely of comments are ignored, as is any whitespace between the tokens.
	///
	/// `KEY` must consist entirely of ASCII alphanumeric characters and `_`.
	///
	/// `VALUE` can contain any Unicode characters, but a few have to be escaped: newlines with `\n`, backslashes with `\\`, quotes with
	/// `\"`.
	class localization_map
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an empty localization map.
		localization_map() = default;

		/// Copies a localization map.
		/// @param map Localization map to copy.
		localization_map(const string_flat_map<std::string>& map);

		/// Moves a localization map.
		/// @param map Localization map to move.
		localization_map(string_flat_map<std::string>&& map);

		/// @}
		/// @name Manipulation
		/// @{

		/// Clears the localization map.
		void clear();

		/// Loads a localization script, returning any non-fatal errors.
		/// @param script Script string in the format defined in the class description.
		/// @return Vector of non-fatal error message strings.
		std::vector<std::string> load_script(std::string_view script);

		/// Loads a localization script file, returning any non-fatal errors.
		/// @param path Path to a script file in the format defined in the class description.
		/// @exception file_not_found If the script file was not found.
		/// @exception file_open_error If opening the script file failed.
		/// @return Vector of non-fatal error message strings.
		std::vector<std::string> load_script_file(const std::filesystem::path& path);

		/// @}
		/// @name Access
		/// @{

		/// Gets whether a key has a corresponding localization string in the map.
		/// @param key Localization key to check.
		/// @return `true` if a string is associated with `key`, `false` otherwise.
		bool contains(std::string_view key) const;

		/// Gets a localization string associated with a key.
		/// @param key Localization key to get a localization string for.
		/// @post The string `key` is a view of must stay alive after the function returns in case it's returned.
		/// @return Localization string associated with a key, or `key` if one doesn't exist.
		std::string_view operator[](std::string_view key) const;

		/// @}

	  private:
		/// Localization script parser.
		class parser
		{
		  public:
			/// Result of a parse operation.
			struct parse_result
			{
				/// Localization key.
				std::string_view key;

				/// Localization value.
				std::string value;
			};

			//

			/// Tries to parse a line of script.
			std::optional<parse_result> parse_line(std::string_view line);

			//

			/// Returns the list of errors generated during parsing.
			std::vector<std::string> errors();

		  private:
			/// List of errors generated during parsing.
			std::vector<std::string> m_errors;

			/// Current line number.
			int m_line{0};

			//

			/// Tries to parse a key and write it to out.
			/// @param line Localization script line.
			/// @param out Output localization key.
			/// @return Remaining line or an empty string view on error.
			std::string_view parse_key(std::string_view line, std::string_view& out);

			/// Tries to parse an `=` delimiter.
			/// @param line Localization script line.
			/// @return Remaining line or an empty string view on error.
			std::string_view parse_delimiter(std::string_view line);

			/// Tries to parse a value and write it to `out`.
			/// @param line Localization script line.
			/// @param out Output localization value.
			/// @return `true` if the parsing was successful, `false` if an error occurred.
			bool parse_value(std::string_view line, std::string& out);

			/// Tries to process escape sequences in a raw value string and write the final value to `out`.
			/// @param raw Raw localization script value string.
			/// @param out Output localization value.
			/// @return `true` if the parsing was successful, `false` if an error occurred.
			bool process_escape_sequences(std::string_view raw, std::string& out);
		};

		//

		/// Base string map.
		string_flat_map<std::string> m_map;
	};
} // namespace tr