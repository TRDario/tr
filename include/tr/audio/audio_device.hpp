/// @file
/// @brief Provides an audio device class.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"

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
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Always empty.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}
	};

	//

	/// Representation of a physical audio device.
	class audio_device
	{
	  public:
		/// @name Constructors
		/// @{

		/// Opens an audio device.
		/// @details A list of valid device names can be gotten through available_audio_devices.
		/// @param name Name of the audio device to open. An empty string view (the default) will choose the default audio device.
		/// @exception audio_device_open_error If opening the audio device failed.
		audio_device(zstring_view name = {});

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the audio device.
		/// @return Name of the audio device.
		tr::zstring_view name() const;

		/// @}

	  private:
		/// Device closer.
		struct deleter
		{
			/// Closes the audio device.
			/// @param device OpenAL device.
			static void operator()(ALCdevice* device);
		};

		//

		/// Owning pointer to the OpenAL audio device.
		std::unique_ptr<ALCdevice, deleter> m_ptr;

		//

		// Accesses the raw OpenAL device pointer.
		friend class audio_context;
	};
} // namespace tr