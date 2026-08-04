/// @file
/// @brief Provides stream utilities.

#pragma once
#include "exception.hpp"

//

namespace tr
{
	/// Exception thrown when a file was not found.
	class file_not_found : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the file that wasn't found.
		file_not_found(std::string_view path);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"File not found"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Path to the file that wasn't found.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		/// Path to the affected file.
		std::string m_path;
	};

	/// Exception thrown when a file opening fails.
	class file_open_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the file that failed to open.
		file_open_error(std::string_view path);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"File opening error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Path to the file whose opening failed.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		/// Path to the affected file.
		std::string m_path;
	};

	//

	/// Opens a file for writing with extra checks.
	/// @param path Path to the file to open.
	/// @param openmode Stream opening mode.
	/// @exception file_open_error If opening the file failed.
	/// @return Output stream to the file at `path`.
	std::ofstream open_file_w(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::in);

	/// Opens a file for reading with extra checks.
	/// @param path Path to the file to open.
	/// @param openmode Stream opening mode.
	/// @exception file_not_found If the file was not found.
	/// @exception file_open_error If opening the file failed.
	/// @return Input stream to the file at `path`.
	std::ifstream open_file_r(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::out);

	//

	/// Checks whether a stream has reached end-of-file.
	/// @param stream Stream to check for EOF.
	/// @return `true` if the stream reached EOF, `false` otherwise.
	bool reached_eof(std::istream& stream);
} // namespace tr