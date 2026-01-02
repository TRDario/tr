#pragma once
#include "../utility/angle.hpp"
#include "buffer.hpp"
#include "stream.hpp"

struct ALCdevice;
struct ALCcontext;

namespace tr::audio {
	enum class origin : bool;
	enum class state : u8;

	// Base audio buffer class.
	class buffer_base {
	  public:
		// Audio buffer ID.
		using id = buffer::id;
		// Hashes audio buffers by ID.
		struct hasher {
			using is_transparent = std::true_type;

			usize operator()(id v) const;
		};
		// Compares audio buffers by ID.
		struct equals {
			using is_transparent = std::true_type;

			bool operator()(id l, id r) const;
		};

		// Creates an audio buffer.
		buffer_base();

		// Gets the ID of the buffer.
		operator id() const;

	  private:
		// Destroys the buffer.
		struct deleter {
			void operator()(id id) const;
		};

		// The underlying buffer handle.
		handle<id, id::EMPTY, deleter> m_handle;
	};

	// Audio stream extended with buffers.
	struct buffered_stream {
		// Audio buffer used by the buffered stream.
		struct buffer : buffer_base {
			// Where the start offset of the audio data is within the stream.
			usize start_offset{0};

			// Refills the buffer with stream data.
			void refill_from(stream& source);
		};

		// The audio stream.
		std::unique_ptr<stream> stream;
		// Buffers associated with the stream.
		std::array<buffer, 4> buffers;
	};

	// Base audio source class.
	class source_base {
	  public:
		static constexpr fsecs START{fsecs::zero()};
		static constexpr fsecs END{fsecs::max()};

		// Creates an empty audio source.
		source_base(int priority);
		// Destroys the audio source.
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
		buffer::id buffer() const;

		// Locks the audio mutex.
		void lock_audio_mutex() const;
		// Unlocks the audio mutex.
		void unlock_audio_mutex() const;

	  private:
		// If the source is sourced from an audio stream, this is that stream.
		std::optional<buffered_stream> m_stream;
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

		friend class manager;
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
		union argument {
			float num;
			glm::vec2 vec2;
			glm::vec3 vec3;
		};

		// The source this command acts upon.
		std::shared_ptr<source_base> m_src;
		// The audio command type.
		type m_type;
		// The initial value.
		argument m_start;
		// The final value.
		argument m_end;
		// The length of the command.
		duration m_length;
		// When the last update was.
		time_point m_last_update;
		// How much time has elapsed for the command.
		duration m_elapsed;

		// Gets the current value of the command.
		argument value();
	};

	// Global audio manager class.
	inline class manager {
	  public:
		// Initializes the audio system and returns if initialization was successful.
		bool initialize();
		// Shuts down and cleans up the audio system.
		void shut_down();

		// Locks the audio mutex.
		void lock_mutex();
		// Unlocks the audio mutex.
		void unlock_mutex();

		// Gets an audio class (0-31)'s gain modifier.
		float class_gain(int id) const;
		// Calculates the gain multiplier based on a bitset of classes.
		float gain_multiplier(std::bitset<32> classes) const;
		// Sets an audio class (0-31)'s gain modifier.
		void set_class_gain(int id, float gain);

		// Allocates an audio buffer and returns its ID.
		buffer::id allocate_buffer();
		// Marks an audio buffer as cullable.
		void mark_buffer_as_cullable(buffer::id id);

		// Allocates an audio source and returns a pointer to it or nullptr if the allocation failed.
		std::shared_ptr<source_base> allocate_source(int priority);

		// Submits an audio command.
		template <class... Args>
			requires(std::constructible_from<command, Args...>)
		void submit_command(Args&&... args);

	  private:
		// Closes an audio device.
		struct device_closer {
			void operator()(ALCdevice* device) const;
		};
		// Destroys an audio context.
		struct context_destroyer {
			void operator()(ALCcontext* context) const;
		};

		// The audio device.
		std::unique_ptr<ALCdevice, device_closer> m_device;
		// The audio context.
		std::unique_ptr<ALCcontext, context_destroyer> m_context;
		// The audio thread.
		std::jthread m_thread;
		// The audio mutex.
		std::mutex m_mutex;
		// A list of active audio commands.
		std::list<command> m_commands;
		// Map holding the handles to extant audio buffer and whether they're cullable.
		std::unordered_map<buffer_base, bool, buffer_base::hasher, buffer_base::equals> m_buffers;
		// The maximum allowed number of audio sources.
		usize m_max_sources;
		// A list of active audio sources.
		std::list<std::shared_ptr<source_base>> m_sources;
		// The gain multipliers of audio classes.
		std::array<float, 32> m_gains;

		// Function used by the audio thread.
		void thread_fn(std::stop_token stoken);
	} g_manager; // Global audio manager.
} // namespace tr::audio

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class... Args>
	requires(std::constructible_from<tr::audio::command, Args...>)
void tr::audio::manager::submit_command(Args&&... args)
{
	std::lock_guard lock{m_mutex};
	m_commands.emplace_back(std::forward<Args>(args)...);
}