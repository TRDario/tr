///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a localization map class.                                                                                                    //
//                                                                                                                                       //
// A localization map is a fairly thin wrapper around a string -> string map meant to serve as a map between localization keys and       //
// localization strings. As such, it can be manually constructed from one. The only other constructor is the default one, which creates  //
// an empty map.                                                                                                                         //
//     - tr::localization_map loc -> empty localization map                                                                              //
//     - tr::localization_map loc{{"key", "value"}, {"key2", "value2"}} -> localization map with values for keys "key" and "key2"        //
//                                                                                                                                       //
// The primary benefit of the class, however, is being able to load key-value pairs from a custom localization text file using the       //
// .load() method. The parser silently handles lines with syntax errors; a vector of error messages is returned by .load() if you wish   //
// to display them to the user. Loading multiple files is allowed: any duplicate keys are silently overwritten; this is not an error.    //
// Besides loading files, clearing the map is the only other allowed operation.                                                          //
//     - loc.load("loc.txt") -> {"line 5: Unterminated quoted string."}; map now contains key-value pairs from loc.txt                   //
//     - loc.load("loc2.txt") -> map now contains key-value pairs from loc.txt AND loc2.txt                                              //
//     - loc.clear() -> map is now empty again                                                                                           //
//                                                                                                                                       //
// The localization file consists of lines in the format: [<KEY> = "<VALUE>"] [#COMMENT]                                                 //
// Empty lines and lines consisting entirely of comments are ignored, as is any whitespace between the tokens.                           //
// KEY must consist entirely of ASCII alphanumeric characters and '_'.                                                                   //
// VALUE can contain any Unicode characters, but a few have to be escaped: newlines with '\n', backslashes with '\\', quotes with '\"'.  //
//     - example_key = "Example Message" # This is an example line                                                                       //
//     - example_key2 = "Value with \"escaped\" characters: /\\_/\\" # Value with "escaped" characters: /\_/\                            //
//                                                                                                                                       //
// The presence of a key in the map can be checked with .contains(), and a view to the value can be gotten through operator[].           //
// If there is no corresponding value for a queried key, operator[] returns a view to the key itself, so passing temporaries to          //
// operator[] is not recommended to avoid dangling references.                                                                           //
//     - loc.contains("example_key") -> true                                                                                             //
//     - loc["example_key"] -> "Example Message"                                                                                         //
//     - loc.contains("nonexistant_key") -> false                                                                                        //
//     - loc["nonexistant_key"] -> "nonexistant_key"                                                                                     //
//     - loc[std::string{"dangling"}] -> DON'T DO THIS                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "hashmap.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Error thrown when localization loading fails.
	class localization_load_error : public exception {
	  public:
		// Constructs an exception.
		localization_load_error(std::string&& description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string m_description;
	};

	// Localization map class with support for reading custom localization files.
	class localization_map {
	  public:
		// Constructs an empty localization map.
		localization_map() = default;
		// Copies a localization map.
		localization_map(const string_hash_map<std::string>& map);
		// Moves a localization map.
		localization_map(string_hash_map<std::string>&& map);

		// Clears the localization map.
		void clear();
		// Loads a localization file, returning any non-fatal errors.
		// May throw: tr::localization_load_error.
		std::vector<std::string> load(const std::filesystem::path& path);

		// Gets whether a key has a corresponding localization string in the map.
		bool contains(std::string_view key) const;
		// Gets a localization string associated with a key, or returns the key if one doesn't exist.
		std::string_view operator[](std::string_view key) const;

	  private:
		// Localization file parser.
		struct parser {
		  public:
			// Result of a parse operation.
			struct parse_result {
				// A localization key.
				std::string_view key;
				// A localization value.
				std::string value;
			};

			// Tries to read a line from a stream. Returns false on EOF.
			bool read_line(std::istream& is);
			// Tries to parse a previously read line.
			std::optional<parse_result> parse_line();

			// Returns the list of errors generated during parsing.
			std::vector<std::string> errors();

		  private:
			// List of errors generated during parsing.
			std::vector<std::string> m_errors;
			// The current line number.
			int m_line{0};
			// Buffer containing the last read line.
			std::string m_buffer;
			// Current cursor position within the buffer.
			std::string::iterator m_cur;

			// Returns an iterator to the first character at or after 'it' that isn't whitespace.
			std::string::iterator skip_whitespace(std::string::iterator it);

			// Tries to parse a key and write it to out. Returns false on error.
			bool parse_key(std::string_view& out);
			// Tries to parse an '=' delimiter. Returns false on error.
			bool parse_delimiter();
			// Tries to parse a value and write it to out. Returns false on error.
			bool parse_value(std::string& out);
			// Tries to process escape sequences in a raw value string and write the final value to out. Returns false on error.
			bool process_escape_sequences(std::string& out, std::string_view raw);
		};

		// The base string map.
		string_hash_map<std::string> m_map;
	};
} // namespace tr