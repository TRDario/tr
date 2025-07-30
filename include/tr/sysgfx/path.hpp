#pragma once
#include "../utility/common.hpp"

namespace tr {
	// Gets a path to the executable directory.
	std::filesystem::path executable_dir();
	// Gets a path to a safe user directory where data can be stored.
	std::filesystem::path user_dir();
} // namespace tr