#pragma once

namespace tr
{
	class application;
	struct application_metadata;
} // namespace tr

//

namespace tr::internal
{
	/// Sets application metadata.
	/// @note This function must be called at least once before you can use `tr::user_directory()`.
	/// @param metadata Application metadata to set.
	void set_application_metadata(const application_metadata& metadata);

	/// Executes an application.
	/// @param application Application to run.
	/// @param argc Command-line argument count.
	/// @param argv Command-line argument values.
	/// @return Exit code of the application.
	[[nodiscard]] int run_application(std::unique_ptr<application> application, int argc, const char** argv);
} // namespace tr::internal