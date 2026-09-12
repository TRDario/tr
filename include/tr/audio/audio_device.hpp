/// @file
/// @brief Provides an audio device class.

#pragma once
#include "../utility/zstring_view.hpp"

struct ALCdevice;

//

namespace tr
{
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
		[[nodiscard]] audio_device(zstring_view name = {});

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the audio device.
		/// @return Name of the audio device.
		[[nodiscard]] tr::zstring_view name() const noexcept;

		/// @}
		/// @cond al_interop
		/// @name OpenAL interoperability
		/// @{

		/// Unwraps the OpenAL device pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the OpenAL device.
		[[nodiscard]] ALCdevice* unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Device closer.
		struct deleter
		{
			/// Closes the audio device.
			/// @param device OpenAL device.
			static void operator()(ALCdevice* device) noexcept;
		};

		//

		/// Owning pointer to the OpenAL audio device.
		std::unique_ptr<ALCdevice, deleter> m_ptr;
	};
} // namespace tr