/// @file
/// @brief Provides `tr::internal::opengl_debug_callback()`.

#pragma once
#include <tr/sysgfx/log_level.hpp>

namespace tr::internal
{
	/// Gets a readable string for an OpenGL debug log message type.
	/// @param value OpenGL debug type.
	/// @return String representation of the debug type.
	[[nodiscard]] std::string_view opengl_debug_type_string(unsigned int value) noexcept;

	/// Converts OpenGL debug severity to a tr log level.
	/// @param value OpenGL debug severity.
	/// @return tr log level equivalent.
	[[nodiscard]] log_level opengl_debug_severity_log_level(unsigned int value) noexcept;

	/// Gets a readable string for an OpenGL debug log source.
	/// @param value OpenGL debug source.
	/// @return String representation of the debug source.
	[[nodiscard]] std::string_view opengl_debug_source_string(unsigned int value) noexcept;

	/// OpenGL debug callback.
	/// @param source Message source.
	/// @param type Message type.
	/// @param severity Message severity.
	/// @param length Length of the debug message.
	/// @param message Pointer to the debug message string.
	void opengl_debug_callback(unsigned int source, unsigned int type, unsigned int, unsigned int severity, int length, const char* message,
							   const void*) noexcept;
} // namespace tr::internal