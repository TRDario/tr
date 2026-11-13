/// @file
/// @brief Provides the `tr::log_sink` interface and common implementations.

#pragma once
#include <tr/utility/common.hpp>

namespace tr
{
	struct log_message;
}

//

namespace tr
{
	/// @brief Interface for sinks recieving log messages.
	class log_sink
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Destroys the sink.
		virtual ~log_sink() noexcept = default;

		/// @}
		/// @name Logging sink interface
		/// @{

		/// @brief Handles a log message.
		/// @param message Log message to handle.
		virtual void log(const log_message& message) = 0;

		/// @brief Flushes the sink.
		virtual void flush() = 0;

		/// @}
	};

	//

	/// @brief Logging sink outputting to the standard output.
	class stdout_log_sink final : public log_sink
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Flushes the standard output.
		~stdout_log_sink() noexcept override;

		/// @}
		/// @name Logging sink interface
		/// @{

		/// @brief Outputs a logging message to the standard output.
		/// @param message Log message to output.
		void log(const log_message& message) override;

		/// @brief Flushes the standard output.
		void flush() override;

		/// @}
	};

	//

	/// @brief Logging sink outputting to log files in a directory.
	/// @details
	/// The directory sink places every source in a separate log file titled after it.
	///
	/// Logs from previous runs are rotated through the subdirectories `old`, `older` and `oldest` before they are discarded.
	class directory_log_sink final : public log_sink
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// Creates a directory logging sink.
		/// @details If the directory does not exist, it is created.
		/// @param directory_path Path to the directory containing the log files.
		directory_log_sink(std::filesystem::path directory_path);

		/// @}
		/// @name Logging sink interface
		/// @{

		/// @brief Outputs a logging message to a log file.
		/// @param message Log message to output.
		void log(const log_message& message) noexcept override;

		/// @brief Flushes the sink (no-op).
		void flush() noexcept override {}

		/// @}

	  private:
		/// @brief Path to the directory containing the log files.
		std::filesystem::path m_directory_path;
	};
} // namespace tr