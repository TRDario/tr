#pragma once
#include "../utility/angle.hpp"
#include "../utility/chrono.hpp"

namespace tr::audio {
	class source_base;
	class buffer;
	class stream;

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
		float max_dist() const;
		// Sets the distance where the source will no longer be attenuated any further.
		void set_max_dist(float max_dist);
		// Sets the distance where the source will no longer be attenuated any further over time.
		void set_max_dist(float max_dist, fsecs time);

		// Gets the distance rolloff factor of the source.
		float rolloff() const;
		// Sets the distance rolloff factor of the source.
		void set_rolloff(float rolloff);
		// Sets the distance rolloff factor of the source over time.
		void set_rolloff(float rolloff, fsecs time);

		// Gets the reference distance of the source, where there is no attenuation.
		float ref_dist() const;
		// Sets the reference distance of the source, where there is no attenuation.
		void set_ref_dist(float ref_dist);
		// Sets the reference distance of the source, where there is no attenuation over time.
		void set_ref_dist(float ref_dist, fsecs time);

		// Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
		float out_cone_gain() const;
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
		void set_out_cone_gain(float out_gain);
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle over time.
		void set_out_cone_gain(float out_gain, fsecs time);

		// Gets the width of the inner cone of the source (where no direction attenuation is done).
		angle in_cone_w() const;
		// Gets the width of the outer cone of the source (where direction attenuation is done).
		angle out_cone_w() const;
		// Sets the width of the inner and outer cones of the source.
		void set_cone_w(angle in_cone_w, angle out_cone_w);
		// Sets the width of the inner and outer cones of the source over time.
		void set_cone_w(angle in_cone_w, angle out_cone_w, fsecs time);

		// Gets the position of the source.
		glm::vec3 pos() const;
		// Sets the position of the source.
		void set_pos(const glm::vec3& pos);
		// Sets the position of the source over time.
		void set_pos(const glm::vec3& pos, fsecs time);

		// Gets the velocity of the source.
		glm::vec3 vel() const;
		// Sets the velocity of the source.
		void set_vel(const glm::vec3& vel);
		// Sets the velocity of the source over time.
		void set_vel(const glm::vec3& vel, fsecs time);

		// Gets the direction of the source cone.
		glm::vec3 dir() const;
		// Sets the direction of the source cone.
		void set_dir(const glm::vec3& dir);
		// Sets the direction of the source cone over time.
		void set_dir(const glm::vec3& dir, fsecs time);

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
		// Shared pointer to the actual audio source implementation.
		std::shared_ptr<source_base> m_base;

		// Allocates an audio source.
		source(std::shared_ptr<source_base> base);

		friend std::optional<source> try_allocating_source(int priority);
	};
	// Tries to allocate an audio source.
	std::optional<source> try_allocating_source(int priority);
} // namespace tr::audio