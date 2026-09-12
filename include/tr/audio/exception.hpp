/// @file
/// @brief Provides exceptions thrown by the audio subsystem.

#pragma once
#include <tr/utility/exception.hpp>

struct ALCdevice;

//

namespace tr
{
	/// Exception thrown when audio device opening fails.
	struct audio_device_open_error final : public tr::exception
	{
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Audio device opening error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}
	};

	/// Error thrown when audio context creation fails.
	class audio_context_init_error final : public tr::exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an audio context initialization error.
		/// @param device Pointer to the audio device the error occured on.
		[[nodiscard]] audio_context_init_error(ALCdevice* device) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Audio context initialization error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always false.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Error description.
		std::string_view m_description;
	};

	/// Error thrown when opening an audio file failed.
	class audio_file_open_error final : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param description Description of the error.
		[[nodiscard]] audio_file_open_error(std::string&& description) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return "Audio file opening error".
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// The description of the error.
		std::string m_description;
	};
} // namespace tr