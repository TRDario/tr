#pragma once
#include "../utility/exception.hpp"

namespace tr::audio {
	// Error thrown when initializing the audio system fails.
	class init_error : public exception {
	  public:
		// Constructs an exception.
		init_error(std::string_view description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string_view m_description;
	};

	// Listener orientation.
	struct orientation {
		// View vector.
		glm::vec3 view;
		// Up vector.
		glm::vec3 up;
	};

	// Initializes the audio system.
	// May throw: audio_init_error.
	void initialize();
	// Reports whether the audio system is active.
	bool active();
	// Shuts the audio system down.
	// Any handles to audio buffers or audio sources must have been destroyed before calling this function.
	void shut_down();

	// Gets the master gain.
	float master_gain();
	// Sets the master gain.
	void set_master_gain(float gain);

	// Gets an audio class (0-31)'s gain modifier.
	float class_gain(int id);
	// Sets an audio class (0-31)'s gain modifier.
	void set_class_gain(int id, float gain);

	// Gets the position of the listener.
	glm::vec3 listener_pos();
	// Sets the position of the listener.
	void set_listener_pos(const glm::vec3& pos);

	// Gets the velocity of the listener.
	glm::vec3 listener_vel();
	// Sets the velocity of the listener.
	void set_listener_vel(const glm::vec3& vel);

	// Gets the orientation of the listener.
	orientation listener_orientation();
	// Sets the orientation of the listener.
	void set_listener_orientation(orientation orientation);
} // namespace tr::audio