/// @file
/// @brief Provides functions for getting standard directories.

#pragma once

//

namespace tr
{
	/// @name Paths
	/// @{

	/// Gets a path to the executable directory.
	/// @return Path to the executable directory.
	std::filesystem::path executable_directory();

	/// Gets a path to a safe user directory where data can be stored.
	/// @details On Windows this is in %appdata%, while on Linux this is in ~/.local/share.
	/// @exception init_error If getting the path failed.
	/// @return Path to the user directory.
	std::filesystem::path user_directory();

	/// @}
} // namespace tr