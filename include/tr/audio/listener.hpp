#pragma once
#include "../utility/common.hpp"

namespace tr::audio {
	// Listener orientation.
	struct orientation {
		// View vector.
		glm::vec3 view;
		// Up vector.
		glm::vec3 up;
	};

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