#pragma once
#include "../utility/angle.hpp"
#include "audio_buffer.hpp"
#include "audio_stream.hpp"

namespace tr {
	struct _buffer_stream;
	enum class audio_origin : bool;
	enum class audio_state : std::uint8_t;

	// Audio buffer used by the buffered stream.
	struct _buffer_stream_buffer {
		// The OpenAL ID of the buffer.
		unsigned int id;
		// Where the start offset of the audio data is within the stream.
		std::size_t start_offset;

		// Refills the buffer with stream data.
		void refill(_buffer_stream& stream) noexcept;
	};

	// Audio stream extrended with buffers.
	struct _buffer_stream {
		std::unique_ptr<audio_stream> stream;
		std::array<_buffer_stream_buffer, 4> buffers;
	};

	// Base audio source class.
	class _audio_source {
	  public:
		static constexpr fsecs START{fsecs::zero()};
		static constexpr fsecs END{fsecs::max()};

		// Creates an empty audio source.
		_audio_source(int priority);
		~_audio_source() noexcept;

		void use(const audio_buffer& buffer) noexcept;
		void use(std::unique_ptr<audio_stream>&& stream);
		void clear() noexcept;
		int priority() const noexcept;
		const std::bitset<32>& classes() const noexcept;
		void set_classes(const std::bitset<32>& classes) noexcept;
		float pitch() const noexcept;
		void set_pitch(float pitch) noexcept;
		float gain() const noexcept;
		void set_gain(float gain) noexcept;
		float max_dist() const noexcept;
		void set_max_dist(float max_dist) noexcept;
		float rolloff() const noexcept;
		void set_rolloff(float rolloff) noexcept;
		float ref_dist() const noexcept;
		void set_ref_dist(float ref_dist) noexcept;
		float out_cone_gain() const noexcept;
		void set_out_cone_gain(float out_gain) noexcept;
		fangle in_cone_w() const noexcept;
		fangle out_cone_w() const noexcept;
		void set_cone_w(fangle in_cone_w, fangle out_cone_w) noexcept;
		glm::vec3 pos() const noexcept;
		void set_pos(const glm::vec3& pos) noexcept;
		glm::vec3 vel() const noexcept;
		void set_vel(const glm::vec3& vel) noexcept;
		glm::vec3 dir() const noexcept;
		void set_dir(const glm::vec3& dir) noexcept;
		audio_origin origin() const noexcept;
		void set_origin(audio_origin type) noexcept;
		audio_state state() const noexcept;
		void play() noexcept;
		void pause() noexcept;
		void stop() noexcept;
		fsecs length() const noexcept;
		fsecs offset() const noexcept;
		void set_offset(fsecs offset) noexcept;
		bool looping() const noexcept;
		fsecs loop_start() const noexcept;
		fsecs loop_end() const noexcept;
		void set_looping(bool looping) noexcept;
		void set_loop_points(fsecs start, fsecs end) noexcept;

		// Gets the ID of the audio source buffer.
		unsigned int buffer() const noexcept;

		// Locks the audio mutex.
		void lock_audio_mutex() const noexcept;
		// Unlocks the audio mutex.
		void unlock_audio_mutex() const noexcept;

	  private:
		// If the source is sourced from an audio stream, this is that stream.
		std::optional<_buffer_stream> _stream;
		// The OpenAL ID of the source.
		unsigned int _id;
		// The gain muiltiplier of the source.
		float _gain;
		// The audio classes the source belongs to.
		std::bitset<32> _classes;
		// The priority of the source.
		int _priority;
		// Some functions that lock the audio mutex call other functions that also do that, so keep a ref counter.
		mutable std::uint32_t _audio_mutex_refc;

		friend void _audio_thread_loop(std::stop_token) noexcept;
	};

	// Audio command for gradual changing of an audio property.
	class _audio_command {
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
		_audio_command(std::shared_ptr<_audio_source> source, type type, float start, float end, duration length) noexcept;
		// Creates an audio command taking vec2 arguments.
		_audio_command(std::shared_ptr<_audio_source> source, type type, glm::vec2 start, glm::vec2 end, duration length) noexcept;
		// Creates an audio command taking vec3 arguments.
		_audio_command(std::shared_ptr<_audio_source> source, type type, glm::vec3 start, glm::vec3 end, duration length) noexcept;

		// Gets the audio source of the command.
		std::shared_ptr<_audio_source> source() const noexcept;
		// Executes the command.
		void execute() noexcept;
		// Reports whether the command is done.
		bool done() const noexcept;

	  private:
		// Type-erased argument storage.
		union _arg {
			float num;
			glm::vec2 vec2;
			glm::vec3 vec3;
		};

		// The source this command acts upon.
		std::shared_ptr<_audio_source> _src;
		// The audio command type.
		type _type;
		// The initial value.
		_arg _start;
		// The final value.
		_arg _end;
		// The length of the command.
		duration _length;
		// When the last update was.
		time_point _last_update;
		// How much time has elapsed for the command.
		duration _elapsed;

		// Gets the current value of the command.
		_arg _value() noexcept;
	};

	// Map holding the IDs of extant audio buffers and whether the're cullable.
	inline std::unordered_map<unsigned int, bool> _audio_buffers_cullable;
	// A list of active audio sources.
	inline std::list<std::shared_ptr<_audio_source>> _audio_sources;
	// The gain multipliers of audio classes.
	inline std::array<float, 32> _audio_gains;
	// A list of active audio commands.
	inline std::list<_audio_command> _audio_commands;
	// The audio mutex.
	inline std::mutex _audio_mutex;
	// The audio thread.
	inline std::jthread _audio_thread;

	// Function used by the audio thread.
	void _audio_thread_loop(std::stop_token stoken) noexcept;
} // namespace tr