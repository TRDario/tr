#pragma once
#include "../utility/exception.hpp"

namespace tr {
	// Error thrown when initializing the audio system fails.
	class audio_system_init_error : public exception {
	  public:
		// Constructs an exception.
		audio_system_init_error(std::string_view description) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The description of the error.
		std::string_view _description;
	};

	// Listener orientation.
	struct orientation {
		// View vector.
		glm::vec3 view;
		// Up vector.
		glm::vec3 up;
	};

	// The global audio system.
	// All methods barring initialize() and active() require the audio system to be initialized, as do the constructors of audio_buffer and
	// audio_source. Additionally, any instances of those classes cannot outlive the audio system.
	struct audio_system {
		// Initializes the audio system.
		// May throw: audio_system_init_error.
		static void initialize();
		// Reports whether the audio system is active.
		static bool active() noexcept;
		// Shuts the audio system down.
		// Any handles to audio buffers or audio sources must have been destroyed before calling this function.
		static void shut_down() noexcept;

		// Gets the master gain.
		static float master_gain() noexcept;
		// Sets the master gain.
		static void set_master_gain(float gain) noexcept;

		// Gets an audio class (0-31)'s gain modifier.
		static float class_gain(int id) noexcept;
		// Sets an audio class (0-31)'s gain modifier.
		static void set_class_gain(int id, float gain) noexcept;

		// Gets the position of the listener.
		static glm::vec3 listener_pos() noexcept;
		// Sets the position of the listener.
		static void set_listener_pos(const glm::vec3& pos) noexcept;

		// Gets the velocity of the listener.
		static glm::vec3 listener_vel() noexcept;
		// Sets the velocity of the listener.
		static void set_listener_vel(const glm::vec3& vel) noexcept;

		// Gets the orientation of the listener.
		static orientation listener_orientation() noexcept;
		// Sets the orientation of the listener.
		static void set_listener_orientation(orientation orientation) noexcept;

		// Reports whether an audio source with a given priority can be allocated.
		static bool can_allocate_audio_source(int priority) noexcept;
	};
} // namespace tr