#pragma once
#include "../utility/angle.hpp"
#include "audio_buffer.hpp"
#include "audio_stream.hpp"

namespace tr {
	struct buffer_stream;
	enum class audio_origin : bool;
	enum class audio_state : std::uint8_t;

	// Audio buffer used by the buffered stream.
	struct buffer_stream_buffer {
		// The OpenAL ID of the buffer.
		unsigned int id;
		// Where the start offset of the audio data is within the stream.
		std::size_t start_offset;

		buffer_stream_buffer();
		~buffer_stream_buffer();

		// Refills the buffer with stream data.
		void refill(buffer_stream& stream);
	};

	// Audio stream extrended with buffers.
	struct buffer_stream {
		std::unique_ptr<audio_stream> stream;
		std::array<buffer_stream_buffer, 4> buffers;
	};

	// Base audio source class.
	class base_audio_source {
	  public:
		static constexpr fsecs START{fsecs::zero()};
		static constexpr fsecs END{fsecs::max()};

		// Creates an empty audio source.
		base_audio_source(int priority);
		~base_audio_source();

		void use(const audio_buffer& buffer);
		void use(std::unique_ptr<audio_stream>&& stream);
		void clear();
		int priority() const;
		const std::bitset<32>& classes() const;
		void set_classes(const std::bitset<32>& classes);
		float pitch() const;
		void set_pitch(float pitch);
		float gain() const;
		void set_gain(float gain);
		float max_dist() const;
		void set_max_dist(float max_dist);
		float rolloff() const;
		void set_rolloff(float rolloff);
		float ref_dist() const;
		void set_ref_dist(float ref_dist);
		float out_cone_gain() const;
		void set_out_cone_gain(float out_gain);
		angle in_cone_w() const;
		angle out_cone_w() const;
		void set_cone_w(angle in_cone_w, angle out_cone_w);
		glm::vec3 pos() const;
		void set_pos(const glm::vec3& pos);
		glm::vec3 vel() const;
		void set_vel(const glm::vec3& vel);
		glm::vec3 dir() const;
		void set_dir(const glm::vec3& dir);
		audio_origin origin() const;
		void set_origin(audio_origin type);
		audio_state state() const;
		void play();
		void pause();
		void stop();
		fsecs length() const;
		fsecs offset() const;
		void set_offset(fsecs offset);
		bool looping() const;
		fsecs loop_start() const;
		fsecs loop_end() const;
		void set_looping(bool looping);
		void set_loop_points(fsecs start, fsecs end);

		// Gets the ID of the audio source buffer.
		unsigned int buffer() const;

		// Locks the audio mutex.
		void lock_audio_mutex() const;
		// Unlocks the audio mutex.
		void unlock_audio_mutex() const;

	  private:
		// If the source is sourced from an audio stream, this is that stream.
		std::optional<buffer_stream> stream;
		// The OpenAL ID of the source.
		unsigned int id;
		// The gain muiltiplier of the source.
		float base_gain;
		// The audio classes the source belongs to.
		std::bitset<32> class_flags;
		// The priority of the source.
		int priority_;
		// Some functions that lock the audio mutex call other functions that also do that, so keep a ref counter.
		mutable std::uint32_t audio_mutex_refc;

		friend void audio_thread_loop(std::stop_token);
	};

	// Audio command for gradual changing of an audio property.
	class audio_command {
	  public:
		// Audio command opcodes.
		enum class command_type {
			PITCH,
			GAIN,
			MAX_DIST,
			ROLLOFF,
			REF_DIST,
			OUT_CONE_GAIN,
			CONE_W,
			POS,
			VEL,
			DIR
		};

		// Creates an audio command taking float arguments.
		audio_command(std::shared_ptr<base_audio_source> source, command_type type, float start, float end, duration length);
		// Creates an audio command taking vec2 arguments.
		audio_command(std::shared_ptr<base_audio_source> source, command_type type, glm::vec2 start, glm::vec2 end, duration length);
		// Creates an audio command taking vec3 arguments.
		audio_command(std::shared_ptr<base_audio_source> source, command_type type, glm::vec3 start, glm::vec3 end, duration length);

		// Gets the audio source of the command.
		std::shared_ptr<base_audio_source> source() const;
		// Executes the command.
		void execute();
		// Reports whether the command is done.
		bool done() const;

	  private:
		// Type-erased argument storage.
		union arg {
			float num;
			glm::vec2 vec2;
			glm::vec3 vec3;
		};

		// The source this command acts upon.
		std::shared_ptr<base_audio_source> src;
		// The audio command type.
		command_type type;
		// The initial value.
		arg start;
		// The final value.
		arg end;
		// The length of the command.
		duration length;
		// When the last update was.
		time_point last_update;
		// How much time has elapsed for the command.
		duration elapsed;

		// Gets the current value of the command.
		arg value();
	};

	// Map holding the IDs of extant audio buffers and whether the're cullable.
	inline std::unordered_map<unsigned int, bool> audio_buffers_cullable;
	// The maximum allowed number of audio sources.
	inline std::size_t max_audio_sources;
	// A list of active audio sources.
	inline std::list<std::shared_ptr<base_audio_source>> audio_sources;
	// The gain multipliers of audio classes.
	inline std::array<float, 32> audio_gains;
	// A list of active audio commands.
	inline std::list<audio_command> audio_commands;
	// The audio mutex.
	inline std::mutex audio_mutex;
	// The audio thread.
	inline std::jthread audio_thread;

	// Function used by the audio thread.
	void audio_thread_loop(std::stop_token stoken);
} // namespace tr