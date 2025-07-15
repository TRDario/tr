#pragma once
#include "../utility/common.hpp"

namespace tr {
	// Sets up the basic application info.
	void initialize_application(const char* developer, const char* name) noexcept;
	// Gets a path to the executable directory.
	std::filesystem::path executable_dir() noexcept;
	// Gets a path to a safe user directory where data can be stored.
	std::filesystem::path user_dir() noexcept;
} // namespace tr