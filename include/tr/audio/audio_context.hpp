/// @file
/// @brief Provides an audio context class.

#pragma once
#include <tr/audio/audio_class.hpp>
#include <tr/audio/internal/audio_command.hpp>
#include <tr/audio/internal/openal.hpp>

struct ALCcontext;
struct ALCdevice;
namespace tr
{
	class audio_buffer;
	class audio_device;
	class audio_stream;
	class audio_source;
	struct orientation;
} // namespace tr

//

namespace tr
{
	/// Object containing all audio context state.
	class audio_context
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an audio context on an audio device.
		/// @param device Reference to the device to create the context on.
		/// @exception audio_context_init_error If creating the audio context failed.
		[[nodiscard]] audio_context(audio_device& device);

		/// Audio contexts are not copyable.
		audio_context(const audio_context&) = delete;

		/// Audio contexts are not movable.
		audio_context(audio_context&&) = delete;

		/// @}
		/// @name Assignment operators.
		/// @{

		/// Audio contexts are not copyable.
		audio_context& operator=(const audio_context&) = delete;

		/// Audio contexts are not movable.
		audio_context& operator=(audio_context&&) = delete;

		/// @}
		/// @name Master gain
		/// @{

		/// Gets the master gain.
		/// @return Master gain factor.
		[[nodiscard]] float master_gain() const noexcept;

		/// Sets the master gain.
		/// @param gain Master gain factor.
		void set_master_gain(float gain) noexcept;

		/// @}
		/// @name Class gain
		/// @details Audio sources may be marked with any combination of tr::audio_class_count audio classes (passed in the form of the
		/// audio_class_mask bitset), each of which has its own gain multiplier which is multiplied with the base gain set on the source.
		/// @{

		/// Gets an audio class's gain modifier.
		/// @param id Class ID to get the gain modifier of.
		/// @return Gain modifier of class `id`.
		[[nodiscard]] float class_gain(audio_class_id id) const noexcept;

		/// Sets an audio class's gain modifier.
		/// @param id Class ID to set the gain modifier of.
		/// @param gain Gain modifier.
		void set_class_gain(audio_class_id id, float gain) noexcept;

		/// @}
		/// @name Listener position
		/// @{

		/// Gets the position of the listener.
		/// @return Position of the listener.
		[[nodiscard]] glm::vec3 listener_position() const noexcept;

		/// Sets the position of the listener.
		/// @param position Position of the listener.
		void set_listener_position(glm::vec3 position) noexcept;

		/// @}
		/// @name Listener velocity
		/// @{

		/// Gets the velocity of the listener.
		/// @return Velocity vector of the listener.
		[[nodiscard]] glm::vec3 listener_velocity() const noexcept;

		/// Sets the velocity of the listener.
		/// @param velocity Velocity vector of the listener.
		void set_listener_velocity(glm::vec3 velocity) noexcept;

		/// @}
		/// @name Listener orientation
		/// @{

		/// Gets the orientation of the listener.
		/// @return Orientation of the listener.
		[[nodiscard]] orientation listener_orientation() const noexcept;

		/// Sets the orientation of the listener.
		/// @param orientation Orientation of the listener.
		void set_listener_orientation(orientation orientation) noexcept;

		/// @}
		/// @name Object factories
		/// @{

		/// Creates a shared audio buffer pointer.
		/// @return Shared pointer to a new audio buffer.
		[[nodiscard]] std::shared_ptr<audio_buffer> create_audio_buffer();

		/// Creates a shared audio source pointer.
		/// @param priority Priority of the audio source.
		/// @return Shared pointer to a new audio source. Result may be null if the source could not be allocated.
		[[nodiscard]] std::shared_ptr<audio_source> create_audio_source(int priority);

		/// @}
		/// @cond al_interop
		/// @name OpenAL interoperability
		/// @{

		/// Unwraps the OpenAL context pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the OpenAL context.
		[[nodiscard]] ALCcontext* unwrap() const noexcept;

		/// @}
		/// @endcond
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Gets a reference to the the OpenAL API.
		/// @return Refernce to the OpenAL API functions.
		const internal::openal& al() const noexcept;

		//

		/// Locks the audio context mutex.
		/// @return Lock context mutex lock guard.
		[[nodiscard]] std::lock_guard<std::mutex> lock_mutex();

		//

		/// Creates an audio command on the context.
		/// @tparam T Argument type.
		/// @param source Audio source to command.
		/// @param method Method the command calls when setting the value.
		/// @param begin Initial value of the property being set.
		/// @param end Final value of the property being set.
		/// @param length Length of the command.
		template <typename T>
		void create_command(audio_source& source, internal::audio_command<T>::method_type method, T begin, T end, fsecs length)
		{
			const std::lock_guard lock{m_mutex};
			m_commands.emplace_back(std::in_place_type_t<internal::audio_command<T>>{}, source, method, begin, end, length);
		}

		/// Creates an audio command on the context.
		/// @tparam Ts Argument types.
		/// @param source Audio source to command.
		/// @param method Method the command calls when setting the value.
		/// @param begin Initial values of the property being set.
		/// @param end Final values of the property being set.
		/// @param length Length of the command.
		template <typename... Ts>
		void create_command(audio_source& source, internal::audio_command<Ts...>::method_type method, const std::tuple<Ts...>& begin,
							const std::tuple<Ts...>& end, fsecs length)
		{
			const std::lock_guard lock{m_mutex};
			m_commands.emplace_back(std::in_place_type_t<internal::audio_command<Ts...>>{}, source, method, begin, end, length);
		}

		/// @}
		/// @endcond

	  private:
		/// Audio context destroyer.
		struct deleter
		{
			/// Destroys the audio context.
			/// @param context OpenAL context.
			static void operator()(ALCcontext* context) noexcept;
		};

		//

		/// Owning pointer to the OpenAL audio context.
		std::unique_ptr<ALCcontext, deleter> m_ptr;

		/// OpenAL API.
		internal::openal m_al;

		/// Maximum allowed number of audio sources.
		usize m_max_sources;

		/// The gain multipliers of audio classes.
		std::array<float, audio_class_count> m_class_gains;

		/// List of audio buffers owned by the audio context.
		std::vector<std::shared_ptr<audio_buffer>> m_buffers;

		/// List of audio sources owned by the audio context.
		std::vector<std::shared_ptr<audio_source>> m_sources;

		/// List of active audio commands.
		std::vector<internal::generic_audio_command> m_commands;

		/// Audio context thread.
		std::jthread m_thread;

		/// Mutex protecting the audio context.
		std::mutex m_mutex;

		//

		/// Function used by the audio context thread.
		/// @param stoken Thread stop token.
		void thread_loop(std::stop_token stoken) noexcept;
	};
} // namespace tr