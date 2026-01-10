///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous iostream functionality.                                                                                        //
//                                                                                                                                       //
// tr::open_file_r and tr::open_file_w act as wrappers over the iostream constructor primarily to provide better error messages:         //
//     - tr::open_file_w("input.txt") -> std::ofstream to "input.txt"                                                                    //
//     - tr::open_file_r("nonexistant.txt") -> File not found: nonexistant.txt                                                           //
//     - tr::open_file_w("restricted.txt") -> File opening error: restricted.txt                                                         //
//                                                                                                                                       //
// tr::reached_eof provides a way of checking for end-of-file without having to trigger eofbit or badbit on operations:                  //
//     - tr::reached_eof(file) -> true, any further operations will set eofbit.                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "exception.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Exception thrown when a file was not found.
	class file_not_found : public exception {
	  public:
		// Constructs an exception.
		file_not_found(std::string_view path);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Path to the affected file.
		std::string m_path;
	};
	// Exception thrown when a file opening fails.
	class file_open_error : public exception {
	  public:
		// Constructs an exception.
		file_open_error(std::string_view path);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Path to the affected file.
		std::string m_path;
	};

	// Opens a file for writing with extra checks.
	// May throw: tr::file_open_error.
	std::ofstream open_file_w(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::in);
	// Opens a file for reading with extra checks.
	// May throw: tr::file_not_found, tr::file_open_error.
	std::ifstream open_file_r(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::out);

	// Checks whether a stream has reached end-of-file.
	bool reached_eof(std::istream& stream);
} // namespace tr