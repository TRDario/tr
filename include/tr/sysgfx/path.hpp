/// @file
/// @brief Provides functions for getting standard directories.

#pragma once
#include <tr/utility/exception.hpp>

//

namespace tr
{
	/// Error thrown when getting a path failed.
	class path_error : public exception
	{
	  public:
		/// Creates a path error.
		/// @param description Description of the error.
		[[nodiscard]] path_error(std::string_view description) noexcept;

		//

		/// Gets the name of the error.
		/// @return `"Path error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

	  private:
		/// Description of the error.
		std::string_view m_description;
	};

	/// @name Paths
	/// @{

	/// Gets a path to the executable directory.
	/// @return Path to the executable directory.
	[[nodiscard]] std::filesystem::path executable_directory();

	/// Gets a path to a safe user directory where data can be stored.
	/// @details
	/// On Windows this is in %appdata%, while on Linux this is in ~/.local/share.
	///
	/// @warning `tr::set_application_metadata()` must be called at least once prior to calling this function.
	/// @exception path_error If getting the path failed.
	/// @return Path to the user directory.
	[[nodiscard]] std::filesystem::path user_directory();

	/// @}
} // namespace tr