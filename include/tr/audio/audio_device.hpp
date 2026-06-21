///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio device class.                                                                                                       //
//                                                                                                                                       //
// Audio devices are identified and opened through names gotten with tr::available_audio_devices(). Opening an audio device with no name //
// will open the default audio device:                                                                                                   //
//     - tr::audio_device device{} -> opens the default audio device                                                                     //
//     - tr::audio_device device{"name"} -> opens the audio device named "name"                                                          //
//                                                                                                                                       //
// The name of an audio device can be obtained through .name():                                                                          //
//     - tr::audio_device device{"name"}; device.name() -> "name"                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"

struct ALCdevice;

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Exception thrown when audio device opening fails.
	struct audio_device_open_error final : public tr::exception {
		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;
	};

	// Representation of a physical audio device.
	class audio_device {
	  public:
		// Opens an audio device with a given name. An empty string view (the default) will choose the default audio device.
		// May throw: audio_device_open_error.
		audio_device(zstring_view name = {});

		// Gets the name of the audio device.
		tr::zstring_view name() const;

	  private:
		// Closes the audio device.
		struct deleter {
			void operator()(ALCdevice* device) const;
		};

		// Owning pointer to the OpenAL audio device.
		std::unique_ptr<ALCdevice, deleter> m_ptr;

		friend class audio_context;
	};
} // namespace tr