///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio source class.                                                                                                       //
//                                                                                                                                       //
// tr::audio_source represents a source of audio in the 3D world. The number of audio sources that may exist on a context at one time is //
// potentially very limited (128 by default), so the only "constructor" (tr::create_audio_source) of audio sources explicitly only tries //
// to allocate one, culling lower priority sources without any remaining references if necessary. Audio sources have an associated       //
// priority level that is used when evaluating whether they can get culled; this is what is specified in teh allocation function, and    //
// also returned by .priority():                                                                                                         //
//     - std::shared_ptr<audio_source> src{tr::create_audio_source(context, 1)}                                                          //
//       -> tries to allocate a source with priority 1; if the limit of audio sources is reached, a lower priority source is sacrificed  //
//          to make space; if no such source exists, nullptr is returned                                                                 //
//     - src->priority() -> 1                                                                                                            //
// All audio source instances are passed in shared pointers as they may be kept alive by the context until culled.                       //
// Leaving audio sources alive after their context is erroneous.                                                                         //
//                                                                                                                                       //
// Audio sources may play audio from audio buffers, or from audio streams. This may be set at any time with the .use() method, or        //
// cleared with the .clear() method:                                                                                                     //
//     - src->set(tr::load_audio_file("sound.ogg")) -> sets the source to use an audio buffer containing sound.ogg                       //
//     - src->set(tr::open_audio_file("music.ogg")) -> sets the source to use an audio stream of music.ogg                               //
//     - src->clear() -> unsets the previously set audio data source without providing a replacement                                     //
//                                                                                                                                       //
// Audio sources may be assigned to any combination of available audio classes. The meaning of these classes is left up to the           //
// application to decide. Audio classes may be used as a convenient way of grouping related audio sources together for volume control,   //
// see .set_class_gain() in tr::audio_context:                                                                                           //
//     - src->set_class_mask(0b11) -> associates 'src' with audio classes 0 and 1                                                        //
//     - src->class_mask() -> bitmask where the flags [0] and [1] are set                                                                //
//                                                                                                                                       //
// A number of attributes (including gain, pitch, position...) of audio sources can be queried, set, or set to gradually change to a set //
// value. The exact meaning of these attributes can be found at https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf:       //
//     - src->pitch() -> 1.0f                                                                                                            //
//     - src->set_gain(0.5f) -> halves the gain of 'src'                                                                                 //
//     - src->set_position({10, 10, 0}, 5.0s) -> moves 'src' to (10, 10, 0) over the next 5 seconds                                      //
//                                                                                                                                       //
// Sources can be positioned in absolute coordinates within the world (tr::audio_source::origin::absolute) or relative to the listener's //
// position (tr::audio_source::origin::listener). A source's origin can be queried or set, and by default is absolute:                   //
//     - src->origin() -> tr::audio_source::origin::absolute                                                                             //
//     - src->set_origin(tr::audio_source::origin::listener); src->origin() -> tr::audio_source::origin::listener                        //
//                                                                                                                                       //
// Sources can be played, paused (and unpaused) and stopped (unlike pausing, this rewinds the offset to the beginning). A source's       //
// current state can be checked with the .state() method:                                                                                //
//     - src->state() -> tr::audio_source::state::initial                                                                                //
//     - src->play(); src->state() -> tr::audio_source::state::playing                                                                   //
//     - src->pause(); src->state() -> tr::audio_source::state::paused                                                                   //
//     - src->stop(); src->state() -> tr::audio_source::state::stopped                                                                   //
// Sources will stop on their own once they reach the end of their audio data.                                                           //
//                                                                                                                                       //
// The length of the audio data a source is playing, as well as the current offset within it, can be gotten through the .length() and    //
// .offset() methods respectively. The offset can be set with the .set_offset() method:                                                  //
//     - src->length() -> 50.0s                                                                                                          //
//     - src->offset() -> 2.0s                                                                                                           //
//     - src->set_offset(10.0s); src->offset() -> 10.0s                                                                                  //
//                                                                                                                                       //
// Much like audio streams, sources can be looped between arbitrary loop points (by default the beginning and end).                      //
// These parameters can also be queried:                                                                                                 //
//     - src->looping() -> false                                                                                                         //
//     - src->loop_start() -> 0.0s                                                                                                       //
//     - src->loop_end() -> 50.0s                                                                                                        //
//     - src->set_looping(true); src->set_loop_start(2.0s) -> source now loops in the interval [2.0s, 50.0s]                             //
//     - src->set_loop_points(0.5s, 10.0s) -> source now loops in the interval [0.5s, 10.0s]                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/static_vector.hpp"
#include "audio_buffer.hpp"
#include "audio_class.hpp"

namespace tr {
	class angle;
	class audio_stream;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 3-dimensional audio source.
	class audio_source {
	  public:
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
		static constexpr glm::vec3 omnidirectional{0, 0, 0};
		// Sentinel value representing the beginning of the audio.
		static constexpr fsecs start{fsecs::zero()};
		// Sentinel value representing the end of the audio.
		static constexpr fsecs end{fsecs::max()};

		// Audio sources are not copyable.
		audio_source(const audio_source&) = delete;
		// Audio sources are not movable.
		audio_source(audio_source&&) = delete;

		// Audio sources are not copyable.
		audio_source& operator=(const audio_source&) = delete;
		// Audio sources are not movable.
		audio_source& operator=(audio_source&&) = delete;

		// Gets a reference to the audio context the source is on.
		audio_context& context() const;

		// Sets a buffer for the source to use.
		void use(std::shared_ptr<audio_buffer> buffer);
		// Sets a audio stream for the source to use.
		void use(std::unique_ptr<audio_stream>&& stream);
		// Unsets a buffer/stream attached to the source.
		void clear();

		// Gets the priority of the audio source.
		int priority() const;
		// Gets the audio classes the source belongs to.
		const audio_class_mask& class_mask() const;
		// Sets the audio classes the source belongs to.
		void set_class_mask(const audio_class_mask& class_mask);

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
		void set_position(const glm::vec3& position);
		// Sets the position of the source over time.
		void set_position(const glm::vec3& position, fsecs time);

		// Gets the velocity of the source.
		glm::vec3 velocity() const;
		// Sets the velocity of the source.
		void set_velocity(const glm::vec3& velocity);
		// Sets the velocity of the source over time.
		void set_velocity(const glm::vec3& velocity, fsecs time);

		// Gets the direction of the source cone.
		glm::vec3 direction() const;
		// Sets the direction of the source cone.
		void set_direction(const glm::vec3& direction);
		// Sets the direction of the source cone over time.
		void set_direction(const glm::vec3& direction, fsecs time);

		// Gets the origin of the source's position.
		origin origin() const;
		// Sets the origin of the source's position.
		void set_origin(enum origin type);

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
		// Source destroyer.
		struct deleter {
			// Reference to the context the source is on.
			audio_context& context;

			// Destroys the audio source.
			void operator()(unsigned int id) const;
		};

		// Buffered audio stream.
		class buffered_stream {
		  public:
			// Constructs an audio stream backed by audio buffers.
			buffered_stream(audio_context& context, std::unique_ptr<audio_stream>&& stream);

			// Gets the length of the stream.
			fsecs length() const;

			// Gets the current offset within the stream.
			fsecs tell() const;
			// Seeks to an offset relative to the beginning.
			void seek(fsecs where);

			// Gets whether the stream is looping.
			bool looping() const;
			// Gets the loop start of the stream.
			fsecs loop_start() const;
			// Gets the loop end of the stream.
			fsecs loop_end() const;
			// Sets whether the stream is looping.
			void set_looping(bool looping);
			// Sets the loop points of the stream.
			void set_loop_points(fsecs start_point, fsecs end_point);

			// Gets the start offset of a buffer belonging to the stream.
			fsecs buffer_start_offset(unsigned int id) const;

			// Tries to refill buffers and returns a list of those that can be requeued.
			static_vector<unsigned int, 4> try_refill_all();
			// Tries to refill buffers and returns a list of those that can be requeued.
			static_vector<unsigned int, 4> try_refill(static_vector<unsigned int, 4> buffers);

		  private:
			// Audio buffer used by the buffered stream.
			struct buffer : audio_buffer {
			  public:
				// Constructs a buffer.
				buffer(audio_context& context);

				// Gets the starting offset of the audio data is within the stream.
				usize start_offset() const;

				// Refills the buffer with stream data.
				void refill_from(audio_stream& source);

			  private:
				// Where the start offset of the audio data is within the stream.
				usize m_start_offset{0};
			};

			// The audio stream.
			std::unique_ptr<audio_stream> m_stream;
			// Buffers associated with the stream.
			std::array<buffer, 4> m_buffers;
		};

		// Handle to the OpenAL source.
		handle<unsigned int, 0, deleter> m_handle;
		// Data source of the source.
		std::variant<std::monostate, std::shared_ptr<audio_buffer>, buffered_stream> m_data_source;
		// Priority of the source.
		int m_priority;
		// Class mask of the audio source.
		audio_class_mask m_class_mask;
		// Gain of the source.
		float m_gain;

		// Creates an empty audio source.
		audio_source(audio_context& context, int priority);

		// Attached an audio buffer to the source.
		void attach_buffer(audio_buffer& buffer);
		// Detaches the attached buffer from the source.
		void detach_buffer();

		// Sets a buffer for the source to use.
		void use(const std::lock_guard<std::mutex>& lock, std::shared_ptr<audio_buffer> buffer);
		// Sets a audio stream for the source to use.
		void use(const std::lock_guard<std::mutex>& lock, std::unique_ptr<audio_stream>&& stream);
		// Unsets a buffer/stream attached to the source.
		void clear(const std::lock_guard<std::mutex>& lock);
		// Plays the source.
		void play(const std::lock_guard<std::mutex>& lock);
		// Stops the source and rewinds it to the beginning.
		void stop(const std::lock_guard<std::mutex>& lock);
		// Sets the source's playback position within the current buffer.
		void set_offset(const std::lock_guard<std::mutex>& lock, fsecs offset);
		// Sets whether the source is looping.
		void set_looping(const std::lock_guard<std::mutex>& lock, bool looping);
		// Sets a source's loop points.
		void set_loop_points(const std::lock_guard<std::mutex>& lock, fsecs start, fsecs end);

		// Refills the audio source if it is streamed.
		void refill_if_needed();

		friend class audio_context;
		friend std::shared_ptr<audio_source> create_audio_source(audio_context& context, int priority);
	};
	// Creates a shared audio source pointer. Result may be null if the source could not be allocated.
	std::shared_ptr<audio_source> create_audio_source(audio_context& context, int priority);
} // namespace tr