///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides implementation details of the audio module.                                                                                  //
//                                                                                                                                       //
// tr::audio::owning_buffer is an RAII wrapper over an OpenAL buffer. tr::audio::buffer is in reality just a glorified counted reference //
// to a tr::audio::owning_buffer owned by tr::audio::g_manager and does not own the audio data. To be specific, it wraps around          //
// g_manager.allocate_buffer() and g_manager.mark_buffer_as_cullable().                                                                  //
//                                                                                                                                       //
// tr::audio::owning_source implements most of the interface of tr::audio::source, but actually owns its OpenAL source. tr::audio::source//
// is in reality just a shared pointer to an owning source also held by tr::audio::g_manager and does not own the OpenAL source.         //
// To be specific, tr::audio::try_allocating_source wraps around g_manager.allocate_source().                                            //
//                                                                                                                                       //
// Gradual changing of audio source attributes is handled by submitting commands via g_manager.submit_command(args...),                  //
// which then handles the commands internally until they are fully executed.                                                             //
//                                                                                                                                       //
// tr::audio::g_manager is the global audio manager and is an encapsulation of various audio state. It is automatically initialized and  //
// shut down by the library during execution.                                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/angle.hpp"
#include "buffer.hpp"
#include "stream.hpp"

struct ALCdevice;
struct ALCcontext;
namespace tr::audio {
	enum class origin : bool;
	enum class state : u8;
} // namespace tr::audio

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::audio {

	// Audio buffer class that owns the underlying OpenAL buffer.
	class owning_buffer {
	  public:
		// Audio buffer ID.
		using id = buffer::id;
		// Hashes audio buffers by ID.
		struct hasher {
			using is_transparent = std::true_type;

			usize operator()(id v) const;
		};

		// Creates an audio buffer.
		owning_buffer();

		// Gets the ID of the buffer.
		operator id() const;

	  private:
		// Destroys the buffer.
		struct deleter {
			void operator()(id id) const;
		};

		// The underlying buffer handle.
		handle<id, id::empty, deleter> m_handle;
	};

	// Audio stream extended with buffers.
	struct buffered_stream {
		// Audio buffer used by the buffered stream.
		struct buffer : owning_buffer {
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

	// Audio source class that owns the underlying OpenAL source.
	class owning_source {
	  public:
		// Sentinel value representing the beginning of the audio.
		static constexpr fsecs start{fsecs::zero()};
		// Sentinel value representing the end of the audio.
		static constexpr fsecs end{fsecs::max()};

		// Creates an empty audio source.
		owning_source(int priority);
		// Destroys the audio source.
		~owning_source();

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

		// Gets the gain of the source.
		float gain() const;
		// Sets the gain of the source.
		void set_gain(float gain);

		// Gets the distance where the source will no longer be attenuated any further.
		float max_dist() const;
		// Sets the distance where the source will no longer be attenuated any further.
		void set_max_dist(float max_dist);

		// Gets the distance rolloff factor of the source.
		float rolloff() const;
		// Sets the distance rolloff factor of the source.
		void set_rolloff(float rolloff);

		// Gets the reference distance of the source, where there is no attenuation.
		float ref_dist() const;
		// Sets the reference distance of the source, where there is no attenuation.
		void set_ref_dist(float ref_dist);

		// Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
		float out_cone_gain() const;
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
		void set_out_cone_gain(float out_gain);

		// Gets the width of the inner cone of the source (where no direction attenuation is done).
		angle in_cone_w() const;
		// Gets the width of the outer cone of the source (where direction attenuation is done).
		angle out_cone_w() const;
		// Sets the width of the inner and outer cones of the source.
		void set_cone_w(angle in_cone_w, angle out_cone_w);

		// Gets the position of the source.
		glm::vec3 pos() const;
		// Sets the position of the source.
		void set_pos(const glm::vec3& pos);

		// Gets the velocity of the source.
		glm::vec3 vel() const;
		// Sets the velocity of the source.
		void set_vel(const glm::vec3& vel);

		// Gets the direction of the source cone.
		glm::vec3 dir() const;
		// Sets the direction of the source cone.
		void set_dir(const glm::vec3& dir);

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
			pitch,
			gain,
			max_distance,
			rolloff,
			reference_distance,
			out_cone_gain,
			cone_width,
			position,
			velocity,
			direction
		};

		// Creates an audio command taking float arguments.
		command(std::shared_ptr<owning_source> source, type type, float start, float end, duration length);
		// Creates an audio command taking vec2 arguments.
		command(std::shared_ptr<owning_source> source, type type, glm::vec2 start, glm::vec2 end, duration length);
		// Creates an audio command taking vec3 arguments.
		command(std::shared_ptr<owning_source> source, type type, glm::vec3 start, glm::vec3 end, duration length);

		// Gets the audio source of the command.
		std::shared_ptr<owning_source> source() const;
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
		std::shared_ptr<owning_source> m_src;
		// The audio command type.
		type m_type;
		// The initial value.
		argument m_start;
		// The final value.
		argument m_end;
		// The length of the command.
		duration m_length;
		// When the last update was.
		std::chrono::steady_clock::time_point m_last_update;
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
		std::shared_ptr<owning_source> allocate_source(int priority);

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
		std::unordered_map<owning_buffer, bool, owning_buffer::hasher, std::equal_to<>> m_buffers;
		// The maximum allowed number of audio sources.
		usize m_max_sources;
		// A list of active audio sources.
		std::list<std::shared_ptr<owning_source>> m_sources;
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