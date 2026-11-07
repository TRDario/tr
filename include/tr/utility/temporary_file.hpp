/// @file
/// @brief Provides `tr::temporary_file`.

#pragma once
#include <tr/utility/common.hpp>

namespace tr
{
	/// @ingroup utility
	/// @brief Handle to a temporary file that gets automatically erased at the end of the lifetime of the handle.
	/// @details This file is primarily intended for use in tests.
	class temporary_file
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Registers a temporary file.
		/// @note This does not create the file, it merely registers it for deletion when the temporary file handle goes out of scope.
		/// @param filename_base Base filename of the file.
		/// @param extension Extension of the file (must include the dot if not empty).
		[[nodiscard]] temporary_file(std::string_view filename_base, std::string_view extension = {});

		/// @brief Temporary files are not copyable.
		temporary_file(const temporary_file&) = delete;

		/// @brief Temporary files are not copyable.
		temporary_file& operator=(const temporary_file&) = delete;

		/// @brief Transfers ownership over a temporary file.
		/// @param rhs Temporary file handle to move from.
		[[nodiscard]] temporary_file(temporary_file&& rhs) noexcept;

		/// @brief Transfers ownership over a temporary file.
		/// @param rhs Temporary file handle to move from.
		temporary_file& operator=(temporary_file&& rhs) noexcept;

		/// @brief Deletes the temporary file.
		~temporary_file() noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the path to the temporary file.
		/// @return Path to the temporary file.
		[[nodiscard]] const std::filesystem::path& path() const noexcept;

		/// @}

	  private:
		/// @brief Path to the temporary file.
		std::filesystem::path m_path;
	};
} // namespace tr