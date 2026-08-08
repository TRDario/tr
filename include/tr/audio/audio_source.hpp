/// @file
/// @brief Provides an audio source class.

#pragma once
#include "../utility/static_vector.hpp"
#include "audio_buffer.hpp"
#include "audio_class.hpp"

namespace tr
{
	class angle;
	class audio_stream;
} // namespace tr

//

namespace tr
{
	/// 3-dimensional audio source.
	/// @details
	/// The number of audio sources that may exist on a context at one time is potentially very limited (128 by default), so the only
	/// "constructor" (create_audio_source) of audio sources explicitly only tries to allocate one, culling lower priority sources without
	/// any remaining references if necessary.
	///
	/// Audio sources have an associated priority level that is used when evaluating whether they can get culled; this is what is specified
	/// in the allocation function.
	class audio_source
	{
	  public:
		/// Audio source position origin types.
		enum class origin : bool
		{
			/// Absolute coordinates.
			absolute,
			/// Coordinates relative to the listener's position.
			listener
		};

		/// Audio source states.
		enum class state : u8
		{
			/// The source has not been played yet.
			initial,
			/// The source is playing.
			playing,
			/// The source is paused.
			paused,
			/// The source stopped playing.
			stopped
		};

		//

		/// 0-length direction vector representing an omnidirectional audio source.
		static constexpr glm::vec3 omnidirectional{0, 0, 0};

		/// Sentinel value representing the beginning of the audio.
		static constexpr fsecs start{fsecs::zero()};

		/// Sentinel value representing the end of the audio.
		static constexpr fsecs end{fsecs::max()};

		/// @name Constructors
		/// @{

		/// Audio sources are not copyable.
		audio_source(const audio_source&) = delete;

		/// Audio sources are not movable.
		audio_source(audio_source&&) = delete;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Audio sources are not copyable.
		audio_source& operator=(const audio_source&) = delete;

		/// Audio sources are not movable.
		audio_source& operator=(audio_source&&) = delete;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the audio context the source is on.
		/// @return Reference to the audio context the source is on.
		audio_context& context() const;

		/// @}
		/// @name Attachments
		/// @{

		/// Sets a buffer for the source to use.
		/// @param buffer Buffer to attach to the source.
		void use(std::shared_ptr<audio_buffer> buffer);

		/// Sets a audio stream for the source to use.
		/// @param stream Stream to attach to the source.
		void use(std::unique_ptr<audio_stream>&& stream);

		/// Unsets a buffer/stream attached to the source.
		void clear();

		/// @}
		/// @name Attributes
		/// @{

		/// Gets the priority of the audio source.
		/// @return Priority of the audio source.
		int priority() const;

		/// Gets the audio classes the source belongs to.
		/// @return Class mask of the source.
		const audio_class_mask& class_mask() const;

		/// Sets the audio classes the source belongs to.
		/// @param class_mask Class mask of the source.
		void set_class_mask(const audio_class_mask& class_mask);

		/// @}
		/// @name Pitch
		/// @{

		/// Gets the pitch of the source.
		/// @return Pitch of the source.
		float pitch() const;

		/// Sets the pitch (and speed) of the source.
		/// @param pitch Pitch of the source.
		void set_pitch(float pitch);

		/// Sets the pitch (and speed) of the source over time.
		/// @param pitch Target pitch of the source.
		/// @param time Transition time.
		void set_pitch(float pitch, fsecs time);

		/// @}
		/// @name Gain
		/// @{

		/// Gets the gain of the source.
		/// @return Gain of the source.
		float gain() const;

		/// Sets the gain of the source.
		/// @param gain Gain of the source.
		void set_gain(float gain);

		/// Sets the gain of the source over time.
		/// @param gain Target gain of the source.
		/// @param time Transition time.
		void set_gain(float gain, fsecs time);

		/// @}
		/// @name Max distance
		/// @{

		/// Gets the distance where the source will no longer be attenuated any further.
		/// @return Maximum distance of the source.
		float max_distance() const;

		/// Sets the distance where the source will no longer be attenuated any further.
		/// @param distance Maximum distance of the source.
		void set_max_distance(float distance);

		/// Sets the distance where the source will no longer be attenuated any further over time.
		/// @param distance Target maximum distance of the source.
		/// @param time Transition time.
		void set_max_distance(float distance, fsecs time);

		/// @}
		/// @name Rolloff factor
		/// @{

		/// Gets the distance rolloff factor of the source.
		/// @return Rolloff factor of the source.
		float rolloff_factor() const;

		/// Sets the distance rolloff factor of the source.
		/// @param rolloff Rolloff factor of the source.
		void set_rolloff_factor(float rolloff);

		/// Sets the distance rolloff factor of the source over time.
		/// @param rolloff Target rolloff factor of the source.
		/// @param time Transition time.
		void set_rolloff_factor(float rolloff, fsecs time);

		/// @}
		/// @name Reference distance
		/// @{

		/// Gets the reference distance of the source, where there is no attenuation.
		/// @return Reference distance of the source.
		float reference_distance() const;

		/// Sets the reference distance of the source, where there is no attenuation.
		/// @param distance Reference distance of the source.
		void set_reference_distance(float distance);

		/// Sets the reference distance of the source, where there is no attenuation over time.
		/// @param distance Target reference distance of the source.
		/// @param time Transition time.
		void set_reference_distance(float distance, fsecs time);

		/// @}
		/// @name Gain outside cone
		/// @{

		/// Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
		/// @return Gain outside cone of the source.
		float gain_outside_cone() const;

		/// Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
		/// @param gain Gain outside cone of the source.
		void set_gain_outside_cone(float gain);

		/// Sets the gain multiplier applied when the listener is outside the source's outer cone angle over time.
		/// @param gain Target gain outside cone of the source.
		/// @param time Transition time.
		void set_gain_outside_cone(float gain, fsecs time);

		/// @}
		/// @name Cone widths
		/// @{

		/// Gets the width of the inner cone of the source (where no direction attenuation is done).
		/// @return Inner cone width of the source.
		angle inner_cone_width() const;

		/// Gets the width of the outer cone of the source (where direction attenuation is done).
		/// @return Outer cone width of the source.
		angle outer_cone_width() const;

		/// Sets the width of the inner and outer cones of the source.
		/// @param inner Inner cone width of the source.
		/// @param outer Outer cone width of the source.
		void set_cone_widths(angle inner, angle outer);

		/// Sets the width of the inner and outer cones of the source over time.
		/// @param inner Target inner cone width of the source.
		/// @param outer Target outer cone width of the source.
		/// @param time Transition time.
		void set_cone_widths(angle inner, angle outer, fsecs time);

		/// @}
		/// @name Position
		/// @{

		/// Gets the position of the source.
		/// @return Position of the source.
		glm::vec3 position() const;

		/// Sets the position of the source.
		/// @param position Position of the source.
		void set_position(glm::vec3 position);

		/// Sets the position of the source over time.
		/// @param position Target position of the source.
		/// @param time Transition time.
		void set_position(glm::vec3 position, fsecs time);

		/// @}
		/// @name Velocity
		/// @{

		/// Gets the velocity of the source.
		/// @return Velocity of the source.
		glm::vec3 velocity() const;

		/// Sets the velocity of the source.
		/// @param velocity Velocity vector of the source.
		void set_velocity(glm::vec3 velocity);

		/// Sets the velocity of the source over time.
		/// @param velocity Target velocity vector of the source.
		/// @param time Transition time.
		void set_velocity(glm::vec3 velocity, fsecs time);

		/// @}
		/// @name Direction
		/// @{

		/// Gets the direction of the source cone.
		/// @return Direction vector of the source cone.
		glm::vec3 direction() const;

		/// Sets the direction of the source cone.
		/// @param direction Direction vector of the source cone.
		void set_direction(glm::vec3 direction);

		/// Sets the direction of the source cone over time.
		/// @param direction Target direction vector of the source cone.
		/// @param time Transition time.
		void set_direction(glm::vec3 direction, fsecs time);

		/// @}
		/// @name Origin
		/// @{

		/// Gets the origin of the source's position.
		/// @return Origin of the source's position.
		origin origin() const;

		/// Sets the origin of the source's position.
		/// @param type Origin of the source's position.
		void set_origin(enum origin type);

		/// @}
		/// @name State
		/// @{

		/// Gets the state of the audio source.
		/// @return State of the audio source.
		state state() const;

		/// Plays the source.
		void play();

		/// Pauses the source.
		void pause();

		/// Stops the source and rewinds it to the beginning.
		void stop();

		/// @}
		/// @name Length & offset
		/// @{

		/// Gets the length of the source audio.
		/// @return Length of the source audio.
		fsecs length() const;

		/// Gets the source's playback position within the current buffer.
		/// @return Playback position within the current buffer.
		fsecs offset() const;

		/// Sets the source's playback position within the current buffer.
		/// @param offset Playback position within the current buffer.
		void set_offset(fsecs offset);

		/// @}
		/// @name Looping
		/// @{

		/// Gets whether the source is looping.
		/// @return `true` if the source is looping, `false` otherwise.
		bool looping() const;

		/// Gets a source's starting loop point.
		/// @return Starting loop timestamp of the source.
		fsecs loop_start() const;

		/// Gets a source's ending loop point.
		/// @return Ending loop timestamp of the source.
		fsecs loop_end() const;

		/// Sets whether the source is looping.
		/// @param looping Whether the source should loop.
		void set_looping(bool looping);

		/// Sets a source's loop points.
		/// @param start Starting loop point in seconds.
		/// @param end Ending loop point in seconds.
		void set_loop_points(fsecs start, fsecs end);

		/// @}

	  private:
		/// Source destroyer.
		struct deleter
		{
			/// Reference to the context the source is on.
			audio_context& context;

			//

			/// Destroys the audio source.
			/// @param id OpenAL source ID.
			void operator()(unsigned int id) const;
		};

		/// Buffered audio stream.
		class buffered_stream
		{
		  public:
			/// Constructs an audio stream backed by audio buffers.
			/// @param context Audio context to create the stream on.
			/// @param stream Base audio stream.
			buffered_stream(audio_context& context, std::unique_ptr<audio_stream>&& stream);

			//

			/// Gets the length of the stream.
			/// @return Length of the stream in seconds.
			fsecs length() const;

			//

			/// Gets the current offset within the stream.
			/// @return Offset sample within the stream.
			fsecs tell() const;

			/// Seeks to an offset relative to the beginning.
			/// @param where Offset sample within the stream.
			void seek(fsecs where);

			//

			/// Gets whether the stream is looping.
			/// @return `true` if the stream is looping, `false` otherwise.
			bool looping() const;

			/// Sets whether the stream is looping.
			/// @param looping `true` if the stream should loop, `false` otherwise.
			void set_looping(bool looping);

			/// Gets the loop start timestamp of the stream.
			/// @return Starting loop timestamp of the stream.
			fsecs loop_start() const;

			/// Gets the loop end timestamp of the stream.
			/// @return Ending loop timestamp of the stream.
			fsecs loop_end() const;

			/// Sets the loop points of the stream.
			/// @param start_point Starting loop timestamp of the stream.
			/// @param end_point Ending loop timestamp of the stream.
			void set_loop_points(fsecs start_point, fsecs end_point);

			//

			/// Gets the start offset of a buffer belonging to the stream.
			/// @param id ID of a buffer belonging to the stream.
			/// @return Starting offset of buffer `id`.
			fsecs buffer_start_offset(unsigned int id) const;

			//

			/// Tries to refill all buffers.
			/// @return List of buffers that can be requeued.
			static_vector<unsigned int, 4> try_refill_all();

			/// Tries to refill buffers.
			/// @param buffers List of buffers to try to refill.
			/// @return List of buffers that can be requeued.
			static_vector<unsigned int, 4> try_refill(static_vector<unsigned int, 4> buffers);

		  private:
			/// Audio buffer used by the buffered stream.
			struct buffer : audio_buffer
			{
			  public:
				/// Constructs a buffer.
				/// @param context Audio context to create the buffer on.
				buffer(audio_context& context);

				//

				/// Gets the starting offset of the audio data within the stream.
				/// @return Starting offset of the audio data within the stream.
				usize start_offset() const;

				//

				/// Refills the buffer with stream data.
				/// @param source Stream to read the data from.
				void refill_from(audio_stream& source);

			  private:
				/// Where the start offset of the audio data is within the stream.
				usize m_start_offset{0};
			};

			//

			/// The audio stream.
			std::unique_ptr<audio_stream> m_stream;

			/// Buffers associated with the stream.
			std::array<buffer, 4> m_buffers;
		};

		//

		/// Handle to the OpenAL source.
		handle<unsigned int, 0, deleter> m_handle;

		/// Data source of the source.
		std::variant<std::monostate, std::shared_ptr<audio_buffer>, buffered_stream> m_data_source;

		/// Priority of the source.
		int m_priority;

		/// Class mask of the audio source.
		audio_class_mask m_class_mask;

		/// Gain of the source.
		float m_gain;

		//

		/// Creates an empty audio source.
		/// @param context Audio context to create the source on.
		/// @param priority Priority of the source.
		audio_source(audio_context& context, int priority);

		//

		/// Attaches an audio buffer to the source.
		/// @param buffer Buffer to attach to the source.
		void attach_buffer(audio_buffer& buffer);

		/// Detaches the attached buffer from the source.
		void detach_buffer();

		//

		/// Sets a buffer for the source to use.
		/// @param lock Context lock.
		/// @param buffer Buffer to use.
		void use(const std::lock_guard<std::mutex>& lock, std::shared_ptr<audio_buffer> buffer);

		/// Sets a audio stream for the source to use.
		/// @param lock Context lock.
		/// @param stream Stream to use.
		void use(const std::lock_guard<std::mutex>& lock, std::unique_ptr<audio_stream>&& stream);

		/// Unsets a buffer/stream attached to the source.
		/// @param lock Context lock.
		void clear(const std::lock_guard<std::mutex>& lock);

		/// Plays the source.
		/// @param lock Context lock.
		void play(const std::lock_guard<std::mutex>& lock);

		/// Stops the source and rewinds it to the beginning.
		/// @param lock Context lock.
		void stop(const std::lock_guard<std::mutex>& lock);

		/// Sets the source's playback position within the current buffer.
		/// @param lock Context lock.
		/// @param offset Playback position within the current buffer.
		void set_offset(const std::lock_guard<std::mutex>& lock, fsecs offset);

		/// Sets whether the source is looping.
		/// @param lock Context lock.
		/// @param looping Whether the source should loop.
		void set_looping(const std::lock_guard<std::mutex>& lock, bool looping);

		/// Sets a source's loop points.
		/// @param lock Context lock.
		/// @param start Starting loop point in seconds.
		/// @param end Ending loop point in seconds.
		void set_loop_points(const std::lock_guard<std::mutex>& lock, fsecs start, fsecs end);

		//

		/// Refills the audio source if it is streamed.
		void refill_if_needed();

		//

		// Accesses the raw OpenAL source ID.
		friend class audio_context;

		// Uses the private constructor.
		friend std::shared_ptr<audio_source> create_audio_source(audio_context& context, int priority);
	};

	/// @name Factories
	/// @{

	/// Creates a shared audio source pointer.
	/// @param context Audio context to create the audio source on.
	/// @param priority Priority of the audio source.
	/// @return Shared pointer to a new audio source. Result may be null if the source could not be allocated.
	std::shared_ptr<audio_source> create_audio_source(audio_context& context, int priority);

	/// @}
} // namespace tr