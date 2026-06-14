///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a localization map class.                                                                                                    //
//                                                                                                                                       //
// A localization map is a fairly thin wrapper around a string -> string map meant to serve as a map between localization keys and       //
// localization strings. As such, it can be manually constructed from one. The only other constructor is the default one, which creates  //
// an empty map:                                                                                                                         //
//     - tr::localization_map loc -> empty localization map                                                                              //
//     - tr::localization_map loc{{"key", "value"}, {"key2", "value2"}} -> localization map with values for keys "key" and "key2"        //
//                                                                                                                                       //
// The primary benefit of the class, however, is being able to load key-value pairs from a custom localization script using the          //
// .load_script() and .load_script_file() methods. The parser silently handles lines with syntax errors; a vector of error messages is   //
// returned by .load_script() and .load_script_file() if you wish to display them to the user. Loading multiple files is allowed: any    //
// duplicate keys are silently overwritten; this is not an error. Besides loading files, clearing the map is the only other allowed      //
// operation:                                                                                                                            //
//     - loc.load_script_file("loc.txt") -> {"line 5: Unterminated quoted string."}; map now contains key-value pairs from loc.txt       //
//     - loc.load_script_file("loc2.txt") -> map now contains key-value pairs from loc.txt AND loc2.txt                                  //
//     - loc.clear() -> map is now empty again                                                                                           //
//     - loc.load_script("a="b"\nb="c"") -> map now contains the pairs {"a", "b"} and {"b", "c"}                                         //
//                                                                                                                                       //
// The localization file consists of lines in the format: [<KEY> = "<VALUE>"] [#COMMENT]                                                 //
// Empty lines and lines consisting entirely of comments are ignored, as is any whitespace between the tokens.                           //
// KEY must consist entirely of ASCII alphanumeric characters and '_'.                                                                   //
// VALUE can contain any Unicode characters, but a few have to be escaped: newlines with '\n', backslashes with '\\', quotes with '\"':  //
//     - example_key = "Example Message" # This is an example line                                                                       //
//     - example_key2 = "Value with \"escaped\" characters: /\\_/\\" # Value with "escaped" characters: /\_/\                            //
//                                                                                                                                       //
// The presence of a key in the map can be checked with .contains(), and a view to the value can be gotten through operator[].           //
// If there is no corresponding value for a queried key, operator[] returns a view to the key itself, so passing temporaries to          //
// operator[] is not recommended to avoid dangling references:                                                                           //
//     - loc.contains("example_key") -> true                                                                                             //
//     - loc["example_key"] -> "Example Message"                                                                                         //
//     - loc.contains("nonexistant_key") -> false                                                                                        //
//     - loc["nonexistant_key"] -> "nonexistant_key"                                                                                     //
//     - loc[std::string{"dangling"}] -> DON'T DO THIS                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "hash_map.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Localization map class with support for reading custom localization files.
	class localization_map {
	  public:
		// Constructs an empty localization map.
		localization_map() = default;
		// Copies a localization map.
		localization_map(const string_flat_map<std::string>& map);
		// Moves a localization map.
		localization_map(string_flat_map<std::string>&& map);

		// Clears the localization map.
		void clear();
		// Loads a localization script, returning any non-fatal errors.
		std::vector<std::string> load_script(std::string_view script);
		// Loads a localization script file, returning any non-fatal errors.
		// May throw: file_not_found, file_open_error.
		std::vector<std::string> load_script_file(const std::filesystem::path& path);

		// Gets whether a key has a corresponding localization string in the map.
		bool contains(std::string_view key) const;
		// Gets a localization string associated with a key, or returns the key if one doesn't exist.
		std::string_view operator[](std::string_view key) const;

	  private:
		class parser {
		  public:
			// Result of a parse operation.
			struct parse_result {
				// A localization key.
				std::string_view key;
				// A localization value.
				std::string value;
			};

			// Tries to parse a line of script.
			std::optional<parse_result> parse_line(std::string_view line);

			// Returns the list of errors generated during parsing.
			std::vector<std::string> errors();

		  private:
			// List of errors generated during parsing.
			std::vector<std::string> m_errors;
			// The current line number.
			int m_line{0};

			// Tries to parse a key and write it to out. Returns the remaining line or an empty string view on error.
			std::string_view parse_key(std::string_view line, std::string_view& out);
			// Tries to parse an '=' delimiter. Returns the remaining line or an empty string view on error.
			std::string_view parse_delimiter(std::string_view line);
			// Tries to parse a value and write it to out. Returns false on error.
			bool parse_value(std::string_view line, std::string& out);
			// Tries to process escape sequences in a raw value string and write the final value to out. Returns false on error.
			bool process_escape_sequences(std::string_view raw, std::string& out);
		};

		// The base string map.
		string_flat_map<std::string> m_map;
	};
} // namespace tr