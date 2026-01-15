///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functions for getting standard directories.                                                                                  //
//                                                                                                                                       //
// The path to the directory the executable is in can be gotten with tr::sys::executable_dir:                                            //
//     - tr::sys::executable_dir() -> std::filesystem::path{"/usr/bin"}                                                                  //
//                                                                                                                                       //
// The path to a user directory where data can be reliably stored can be gotten with tr::sys::user_dir. On Windows this is in %appdata%, //
// while on Linux this is in ~/.local/share:                                                                                             //
//     - tr::sys::user_dir() -> std::filesystem::path{"~/.local/share/TRDario/Bodge"}                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::sys {
	// Gets a path to the executable directory.
	std::filesystem::path executable_dir();
	// Gets a path to a safe user directory where data can be stored.
	std::filesystem::path user_dir();
} // namespace tr::sys