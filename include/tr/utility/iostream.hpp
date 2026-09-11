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
		[[nodiscard]] file_not_found(std::string_view path);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"File not found"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Path to the file that wasn't found.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

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
		[[nodiscard]] file_open_error(std::string_view path);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"File opening error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Path to the file whose opening failed.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

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
	[[nodiscard]] std::ofstream open_file_w(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::in);

	/// Opens a file for reading with extra checks.
	/// @param path Path to the file to open.
	/// @param openmode Stream opening mode.
	/// @exception file_not_found If the file was not found.
	/// @exception file_open_error If opening the file failed.
	/// @return Input stream to the file at `path`.
	[[nodiscard]] std::ifstream open_file_r(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::out);

	//

	/// Checks whether a stream has reached end-of-file.
	/// @param stream Stream to check for EOF.
	/// @return `true` if the stream reached EOF, `false` otherwise.
	[[nodiscard]] bool reached_eof(std::istream& stream);
} // namespace tr