///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functions for manipulating audio listener state.                                                                             //
//                                                                                                                                       //
// The master gain, as well as the gains of any of the 32 audio classes can be gotten and set:                                           //
//     - tr::audio::set_master_gain(tr::audio::master_gain() / 2) -> halve the master gain                                               //
//     - tr::audio::set_class_gain(1, tr::audio::class_gain(1) * 2) -> double the gain of audio class 1                                  //
//                                                                                                                                       //
// The position, velocity, and orientation of the listener can be gotten and set:                                                        //
//     - tr::audio::listener_pos() -> {0, 0, 0}                                                                                          //
//     - tr::audio::set_listener_pos({5, 0, 5}) -> listener is now at {5, 0, 5}                                                          //
//     - tr::audio::listener_vel() -> {0, 0, 0}                                                                                          //
//     - tr::audio::set_listener_vel({5, 0, 5}) -> listener is now treated as if it has a velocity of {5, 0, 5}                          //
//     - tr::audio::listener_orientation() -> gets the listener's orientation                                                            //
//     - tr::audio::set_listener_orientation(view, up) -> sets the listener's orientation                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

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