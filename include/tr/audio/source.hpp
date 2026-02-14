///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio source class and related datatypes.                                                                                 //
//                                                                                                                                       //
// tr::audio::source represents a source of audio in the 3D world. The number of audio sources that may exist at one time is potentially //
// very limited (128 by default), so the only "constructor" of audio sources explicitly only tries to allocate one, culling lower        //
// priority sources without any remaining references if necessary. Audio sources have an associated priority level that is used when     //
// evaluating whether they can get culled; this is what is specified in teh allocation function, and also returned by .priority():       //
//     - tr::audio::source src{*tr::audio::try_allocating_source(1)}                                                                     //
//       -> tries to allocate a source with priority 1; if the limit of audio sources is reached, a lower priority source is sacrificed  //
//          to make space; if no such source exists, the allocation fails                                                                //
//     - src.priority() -> 1                                                                                                             //
// Leaving audio sources alive at the end of the program is erroneous and may trigger an assertion in debug builds.                      //
//                                                                                                                                       //
// Audio sources may play audio from audio buffers, or from audio streams. This may be set at any time with the .use() method, or        //
// cleared with the .clear() method:                                                                                                     //
//     - source.set(tr::audio::load_file("sound.ogg")) -> sets the source to use an audio buffer containing sound.ogg                    //
//     - source.set(tr::audio::open_file("music.ogg")) -> sets the source to use an audio stream of music.ogg                            //
//     - source.clear() -> unsets the previously set audio data source without providing a replacement                                   //
//                                                                                                                                       //
// Audio sources may be assigned to any combination of 32 available audio classes. The meaning of these classes is left up to the        //
// application to decide. Audio classes may be used as a convenient way of grouping related audio sources together for volume control,   //
// see tr::audio::set_class_gain in listener.hpp:                                                                                        //
//     - src.set_classes(0b11) -> associates 'src' with audio classes 0 and 1                                                            //
//     - src.classes() -> bitmask where the flags [0] and [1] are set                                                                    //
//                                                                                                                                       //
// A number of attributes (including gain, pitch, position...) of audio sources can be queried, set, or set to gradually change to a set //
// value. The exact meaning of these attributes can be found at https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf:       //
//     - src.pitch() -> 1.0f                                                                                                             //
//     - src.set_gain(0.5f) -> halves the gain of 'src'                                                                                  //
//     - src.set_position({10, 10, 0}, 5.0s) -> moves 'src' to (10, 10, 0) over the next 5 seconds                                       //
//                                                                                                                                       //
// Sources can be positioned in absolute coordinates within the world (tr::audio::origin::absolute) or relative to the listener's        //
// position (tr::audio::origin::listener). A source's origin can be queried or set, and by default is absolute:                          //
//     - src.origin() -> tr::audio::origin::absolute                                                                                     //
//     - src.set_origin(tr::audio::origin::listener); src.origin() -> tr::audio::origin::listener                                        //
//                                                                                                                                       //
// Sources can be played, paused (and unpaused) and stopped (unlike pausing, this rewinds the offset to the beginning). A source's       //
// current state can be checked with the .state() method:                                                                                //
//     - src.state() -> tr::audio::state::initial                                                                                        //
//     - src.play(); src.state() -> tr::audio::state::playing                                                                            //
//     - src.pause(); src.state() -> tr::audio::state::paused                                                                            //
//     - src.stop(); src.state() -> tr::audio::state::stopped                                                                            //
// Sources will stop on their own once they reach the end of their audio data.                                                           //
//                                                                                                                                       //
// The length of the audio data a source is playing, as well as the current offset within it, can be gotten through the .length() and    //
// .offset() methods respectively. The offset can be set with the .set_offset() method:                                                  //
//     - src.length() -> 50.0s                                                                                                           //
//     - src.offset() -> 2.0s                                                                                                            //
//     - src.set_offset(10.0s); src.offset() -> 10.0s                                                                                    //
//                                                                                                                                       //
// Much like audio streams, sources can be looped between arbitrary loop points (by default the beginning and end).                      //
// These parameters can also be queried:                                                                                                 //
//     - src.looping() -> false                                                                                                          //
//     - src.loop_start() -> 0.0s                                                                                                        //
//     - src.loop_end() -> 50.0s                                                                                                         //
//     - src.set_looping(true); stream.set_loop_start(2.0s) -> stream now loops in the interval [2.0s, 50.0s]                            //
//     - src.set_loop_points(0.5s, 10.0s) -> source now loops in the interval [0.5s, 10.0s]                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/angle.hpp"
#include "../utility/chrono.hpp"

namespace tr::audio {
	class owning_source;
	class buffer;
	class stream;
} // namespace tr::audio

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::audio {
	// Audio source position origin types.
	enum class origin : bool {
		absolute, // Absolute coordinates.
		listener  // Coordinates relative to the listener's position.
	};

	// Audio source states.
	enum class state : u8 {
		initial, // The source has not been played yet.
		playing, // The source is playing.
		paused,  // The source is paused.
		stopped  // The source stopped playing.
	};

	// 0-length direction vector representing an omnidirectional audio source.
	inline constexpr glm::vec3 omnidirectional{0, 0, 0};

	// 3D Audio source.
	class source {
	  public:
		// Sentinel value representing the beginning of the audio.
		static constexpr fsecs start{fsecs::zero()};
		// Sentinel value representing the end of the audio.
		static constexpr fsecs end{fsecs::max()};

		source(const source&) = delete;
		source(source&&) noexcept = default;
		source& operator=(const source&) = delete;
		source& operator=(source&&) noexcept = default;

		// Sets a buffer for the source to use.
		void use(const buffer& buffer);
		// Sets a audio stream for the source to use.
		void use(std::unique_ptr<stream>&& stream);
		// Unsets a buffer/stream attached to the source.
		void clear();

		// Gets the priority of the audio source.
		int priority() const;
		// Gets the audio classes the source belongs to.
		const std::bitset<32>& classes() const;
		// Sets the audio classes the source belongs to.
		void set_classes(const std::bitset<32>& classes);

		// Gets the pitch of the source.
		float pitch() const;
		// Sets the pitch (and speed) of the source.
		void set_pitch(float pitch);
		// Sets the pitch (and speed) of the source over time.
		void set_pitch(float pitch, fsecs time);

		// Gets the gain of the source.
		float gain() const;
		// Sets the gain of the source.
		void set_gain(float gain);
		// Sets the gain of the source over time.
		void set_gain(float gain, fsecs time);

		// Gets the distance where the source will no longer be attenuated any further.
		float max_distance() const;
		// Sets the distance where the source will no longer be attenuated any further.
		void set_max_distance(float distance);
		// Sets the distance where the source will no longer be attenuated any further over time.
		void set_max_distance(float distance, fsecs time);

		// Gets the distance rolloff factor of the source.
		float rolloff_factor() const;
		// Sets the distance rolloff factor of the source.
		void set_rolloff_factor(float rolloff);
		// Sets the distance rolloff factor of the source over time.
		void set_rolloff_factor(float rolloff, fsecs time);

		// Gets the reference distance of the source, where there is no attenuation.
		float reference_distance() const;
		// Sets the reference distance of the source, where there is no attenuation.
		void set_reference_distance(float distance);
		// Sets the reference distance of the source, where there is no attenuation over time.
		void set_reference_distance(float distance, fsecs time);

		// Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
		float gain_outside_cone() const;
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
		void set_gain_outside_cone(float gain);
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle over time.
		void set_gain_outside_cone(float gain, fsecs time);

		// Gets the width of the inner cone of the source (where no direction attenuation is done).
		angle inner_cone_width() const;
		// Gets the width of the outer cone of the source (where direction attenuation is done).
		angle outer_cone_width() const;
		// Sets the width of the inner and outer cones of the source.
		void set_cone_widths(angle inner, angle outer);
		// Sets the width of the inner and outer cones of the source over time.
		void set_cone_widths(angle inner, angle outer, fsecs time);

		// Gets the position of the source.
		glm::vec3 position() const;
		// Sets the position of the source.
		void set_position(const glm::vec3& pos);
		// Sets the position of the source over time.
		void set_position(const glm::vec3& pos, fsecs time);

		// Gets the velocity of the source.
		glm::vec3 velocity() const;
		// Sets the velocity of the source.
		void set_velocity(const glm::vec3& vel);
		// Sets the velocity of the source over time.
		void set_velocity(const glm::vec3& vel, fsecs time);

		// Gets the direction of the source cone.
		glm::vec3 direction() const;
		// Sets the direction of the source cone.
		void set_direction(const glm::vec3& dir);
		// Sets the direction of the source cone over time.
		void set_direction(const glm::vec3& dir, fsecs time);

		// Gets the origin of the source's position.
		origin origin() const;
		// Sets the origin of the source's position.
		void set_origin(audio::origin type);

		// Gets the state of the audio source.
		state state() const;
		// Plays the source.
		void play();
		// Pauses the source.
		void pause();
		// Stops the source and rewinds it to the beginning.
		void stop();

		// Gets the length of the source audio.
		fsecs length() const;
		// Gets the source's playback position within the current buffer.
		fsecs offset() const;
		// Sets the source's playback position within the current buffer.
		void set_offset(fsecs offset);

		// Gets whether the source is looping.
		bool looping() const;
		// Gets a source's starting loop point.
		fsecs loop_start() const;
		// Gets a source's ending loop point.
		fsecs loop_end() const;
		// Sets whether the source is looping.
		void set_looping(bool looping);
		// Sets a source's loop points.
		void set_loop_points(fsecs start, fsecs end);

	  private:
		// Shared pointer to an owning audio source in the audio manager.
		std::shared_ptr<owning_source> m_base;

		// Wraps a pointer to the owning audio source.
		source(std::shared_ptr<owning_source> base);

		friend std::optional<source> try_allocating_source(int priority);
	};
	// Tries to allocate an audio source.
	std::optional<source> try_allocating_source(int priority);
} // namespace tr::audio