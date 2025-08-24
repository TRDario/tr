#pragma once
#include "../utility/angle.hpp"
#include "buffer.hpp"
#include "stream.hpp"

namespace tr::audio {
	enum class origin : bool;
	enum class state : u8;

	// Audio buffer used by the buffered stream.
	struct buffer_stream_buffer {
		// The OpenAL ID of the buffer.
		unsigned int id;
		// Where the start offset of the audio data is within the stream.
		usize start_offset;

		buffer_stream_buffer();
		~buffer_stream_buffer();
	};
	// Audio stream extrended with buffers.
	struct buffer_stream {
		std::unique_ptr<stream> stream;
		std::array<buffer_stream_buffer, 4> buffers;
	};
	// Refills the buffer with stream data.
	void refill(stream& stream, buffer_stream_buffer& buffer);

	// Base audio source class.
	class source_base {
	  public:
		static constexpr fsecs START{fsecs::zero()};
		static constexpr fsecs END{fsecs::max()};

		// Creates an empty audio source.
		source_base(int priority);
		~source_base();

		void use(const buffer& buffer);
		void use(std::unique_ptr<stream>&& stream);
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
		origin origin() const;
		void set_origin(audio::origin type);
		state state() const;
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
		std::optional<buffer_stream> m_stream;
		// The OpenAL ID of the source.
		unsigned int m_id;
		// The gain muiltiplier of the source.
		float m_gain;
		// The audio classes the source belongs to.
		std::bitset<32> m_classes;
		// The priority of the source.
		int m_priority;
		// Some functions that lock the audio mutex call other functions that also do that, so keep a ref counter.
		mutable u32 m_mutex_refc;

		friend void thread_fn(std::stop_token);
	};

	// Audio command for gradual changing of an audio property.
	class command {
	  public:
		// Audio command opcodes.
		enum class type {
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
		command(std::shared_ptr<source_base> source, type type, float start, float end, duration length);
		// Creates an audio command taking vec2 arguments.
		command(std::shared_ptr<source_base> source, type type, glm::vec2 start, glm::vec2 end, duration length);
		// Creates an audio command taking vec3 arguments.
		command(std::shared_ptr<source_base> source, type type, glm::vec3 start, glm::vec3 end, duration length);

		// Gets the audio source of the command.
		std::shared_ptr<source_base> source() const;
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
		std::shared_ptr<source_base> m_src;
		// The audio command type.
		type m_type;
		// The initial value.
		arg m_start;
		// The final value.
		arg m_end;
		// The length of the command.
		duration m_length;
		// When the last update was.
		time_point m_last_update;
		// How much time has elapsed for the command.
		duration m_elapsed;

		// Gets the current value of the command.
		arg value();
	};

	// Map holding the IDs of extant audio buffers and whether the're cullable.
	inline std::unordered_map<unsigned int, bool> buffers;
	// The maximum allowed number of audio sources.
	inline usize max_sources;
	// A list of active audio sources.
	inline std::list<std::shared_ptr<source_base>> sources;
	// The gain multipliers of audio classes.
	inline std::array<float, 32> gains;
	// A list of active audio commands.
	inline std::list<command> commands;
	// The audio mutex.
	inline std::mutex mutex;
	// The audio thread.
	inline std::jthread thread;

	// Function used by the audio thread.
	void thread_fn(std::stop_token stoken);
} // namespace tr::audio