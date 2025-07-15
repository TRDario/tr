#pragma once
#include "../utility/angle.hpp"
#include "../utility/chrono.hpp"

namespace tr {
	class _audio_source;
	class audio_buffer;
	class audio_stream;

	// Audio source position origin types.
	enum class audio_origin : bool {
		ABSOLUTE, // Absolute coordinates.
		LISTENER  // Coordinates relative to the listener's position.
	};

	// Audio source states.
	enum class audio_state : std::uint8_t {
		INITIAL, // The source has not been played yet.
		PLAYING, // The source is playing.
		PAUSED,  // The source is paused.
		STOPPED  // The source stopped playing.
	};

	// 0-length direction vector representing an omnidirectional audio source.
	inline constexpr glm::vec3 OMNIDIRECTIONAL{0, 0, 0};

	// 3D Audio source.
	class audio_source {
	  public:
		// Sentinel value representing the beginning of the audio.
		static constexpr fsecs START{fsecs::zero()};
		// Sentinel value representing the end of the audio.
		static constexpr fsecs END{fsecs::max()};

		// Constructs an audio source.
		audio_source(int priority) noexcept;
		audio_source(const audio_source&) = delete;
		audio_source(audio_source&&) noexcept = default;
		audio_source& operator=(const audio_source&) = delete;
		audio_source& operator=(audio_source&&) noexcept = default;

		// Sets a buffer for the source to use.
		void use(const audio_buffer& buffer) noexcept;
		// Sets a audio stream for the source to use.
		void use(std::unique_ptr<audio_stream>&& stream) noexcept;
		// Unsets a buffer/stream attached to the source.
		void clear() noexcept;

		// Gets the priority of the audio source.
		int priority() const noexcept;
		// Gets the audio classes the source belongs to.
		const std::bitset<32>& classes() const noexcept;
		// Sets the audio classes the source belongs to.
		void set_classes(const std::bitset<32>& classes) noexcept;

		// Gets the pitch of the source.
		float pitch() const noexcept;
		// Sets the pitch (and speed) of the source.
		void set_pitch(float pitch) noexcept;
		// Sets the pitch (and speed) of the source over time.
		void set_pitch(float pitch, fsecs time) noexcept;

		// Gets the gain of the source.
		float gain() const noexcept;
		// Sets the gain of the source.
		void set_gain(float gain) noexcept;
		// Sets the gain of the source over time.
		void set_gain(float gain, fsecs time) noexcept;

		// Gets the distance where the source will no longer be attenuated any further.
		float max_dist() const noexcept;
		// Sets the distance where the source will no longer be attenuated any further.
		void set_max_dist(float max_dist) noexcept;
		// Sets the distance where the source will no longer be attenuated any further over time.
		void set_max_dist(float max_dist, fsecs time) noexcept;

		// Gets the distance rolloff factor of the source.
		float rolloff() const noexcept;
		// Sets the distance rolloff factor of the source.
		void set_rolloff(float rolloff) noexcept;
		// Sets the distance rolloff factor of the source over time.
		void set_rolloff(float rolloff, fsecs time) noexcept;

		// Gets the reference distance of the source, where there is no attenuation.
		float ref_dist() const noexcept;
		// Sets the reference distance of the source, where there is no attenuation.
		void set_ref_dist(float ref_dist) noexcept;
		// Sets the reference distance of the source, where there is no attenuation over time.
		void set_ref_dist(float ref_dist, fsecs time) noexcept;

		// Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
		float out_cone_gain() const noexcept;
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
		void set_out_cone_gain(float out_gain) noexcept;
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle over time.
		void set_out_cone_gain(float out_gain, fsecs time) noexcept;

		// Gets the width of the inner cone of the source (where no direction attenuation is done).
		fangle in_cone_w() const noexcept;
		// Gets the width of the outer cone of the source (where direction attenuation is done).
		fangle out_cone_w() const noexcept;
		// Sets the width of the inner and outer cones of the source.
		void set_cone_w(fangle in_cone_w, fangle out_cone_w) noexcept;
		// Sets the width of the inner and outer cones of the source over time.
		void set_cone_w(fangle in_cone_w, fangle out_cone_w, fsecs time) noexcept;

		// Gets the position of the source.
		glm::vec3 pos() const noexcept;
		// Sets the position of the source.
		void set_pos(const glm::vec3& pos) noexcept;
		// Sets the position of the source over time.
		void set_pos(const glm::vec3& pos, fsecs time) noexcept;

		// Gets the velocity of the source.
		glm::vec3 vel() const noexcept;
		// Sets the velocity of the source.
		void set_vel(const glm::vec3& vel) noexcept;
		// Sets the velocity of the source over time.
		void set_vel(const glm::vec3& vel, fsecs time) noexcept;

		// Gets the direction of the source cone.
		glm::vec3 dir() const noexcept;
		// Sets the direction of the source cone.
		void set_dir(const glm::vec3& dir) noexcept;
		// Sets the direction of the source cone over time.
		void set_dir(const glm::vec3& dir, fsecs time) noexcept;

		// Gets the origin of the source's position.
		audio_origin origin() const noexcept;
		// Sets the origin of the source's position.
		void set_origin(audio_origin type) noexcept;

		// Gets the state of the audio source.
		audio_state state() const noexcept;
		// Plays the source.
		void play() noexcept;
		// Pauses the source.
		void pause() noexcept;
		// Stops the source and rewinds it to the beginning.
		void stop() noexcept;

		// Gets the length of the source audio.
		fsecs length() const noexcept;
		// Gets the source's playback position within the current buffer.
		fsecs offset() const noexcept;
		// Sets the source's playback position within the current buffer.
		void set_offset(fsecs offset) noexcept;

		// Gets whether the source is looping.
		bool looping() const noexcept;
		// Gets a source's starting loop point.
		fsecs loop_start() const noexcept;
		// Gets a source's ending loop point.
		fsecs loop_end() const noexcept;
		// Sets whether the source is looping.
		void set_looping(bool looping) noexcept;
		// Sets a source's loop points.
		void set_loop_points(fsecs start, fsecs end) noexcept;

	  private:
		// Shared pointer to the actual audio source implementation.
		std::shared_ptr<_audio_source> _impl;
	};
} // namespace tr