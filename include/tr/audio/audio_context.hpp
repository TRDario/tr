/// @file
/// @brief Provides an audio context class.

#pragma once
#include "../utility/exception.hpp"
#include "al_api.hpp"
#include "audio_buffer.hpp"
#include "audio_class.hpp"
#include "audio_command.hpp"

struct ALCcontext;
struct ALCdevice;
namespace tr
{
	class audio_device;
	class audio_stream;
	class audio_source;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr
{
	/// Listener orientation.
	struct orientation
	{
		/// View vector.
		glm::vec3 view;

		/// Up vector.
		glm::vec3 up;

		//

		/// Compares orientations for equality.
		/// @return Whether the orientations are identical.
		friend bool operator==(const orientation&, const orientation&) = default;
	};

	//

	/// Error thrown when audio context creation fails.
	class audio_context_init_error final : public tr::exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an audio context initialization error.
		/// @param device Pointer to the audio device the error occured on.
		audio_context_init_error(ALCdevice* device);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Audio context initialization error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always false.
		std::string_view details() const override;

		/// @}

	  private:
		/// Error description.
		std::string_view m_description;
	};

	//

	/// Object containing all audio context state.
	class audio_context
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an audio context on an audio device.
		/// @param device Reference to the device to create the context on.
		/// @exception audio_context_init_error If creating the audio context failed.
		audio_context(audio_device& device);

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
		float master_gain() const;

		/// Sets the master gain.
		/// @param gain Master gain factor.
		void set_master_gain(float gain);

		/// @}
		/// @name Class gain
		/// @details Audio sources may be marked with any combination of tr::audio_class_count audio classes (passed in the form of the
		/// audio_class_mask bitset), each of which has its own gain multiplier which is multiplied with the base gain set on the source.
		/// @{

		/// Gets an audio class's gain modifier.
		/// @param id Class ID to get the gain modifier of.
		/// @return Gain modifier of class `id`.
		float class_gain(audio_class_id id) const;

		/// Sets an audio class's gain modifier.
		/// @param id Class ID to set the gain modifier of.
		/// @param gain Gain modifier.
		void set_class_gain(audio_class_id id, float gain);

		/// @}
		/// @name Listener position
		/// @{

		/// Gets the position of the listener.
		/// @return Position of the listener.
		glm::vec3 listener_position() const;

		/// Sets the position of the listener.
		/// @param position Position of the listener.
		void set_listener_position(glm::vec3 position);

		/// @}
		/// @name Listener velocity
		/// @{

		/// Gets the velocity of the listener.
		/// @return Velocity vector of the listener.
		glm::vec3 listener_velocity() const;

		/// Sets the velocity of the listener.
		/// @param velocity Velocity vector of the listener.
		void set_listener_velocity(glm::vec3 velocity);

		/// @}
		/// @name Listener orientation
		/// @{

		/// Gets the orientation of the listener.
		/// @return Orientation of the listener.
		orientation listener_orientation() const;

		/// Sets the orientation of the listener.
		/// @param orientation Orientation of the listener.
		void set_listener_orientation(orientation orientation);

		/// @}

	  private:
		/// Audio context destroyer.
		struct deleter
		{
			/// Destroys the audio context.
			/// @param context OpenAL context.
			static void operator()(ALCcontext* context);
		};

		//

		/// Owning pointer to the OpenAL audio context.
		std::unique_ptr<ALCcontext, deleter> m_ptr;

		/// OpenAL API.
		al_api m_al_api;

		/// Maximum allowed number of audio sources.
		usize m_max_sources;

		/// The gain multipliers of audio classes.
		std::array<float, audio_class_count> m_class_gains;

		/// List of audio buffers owned by the audio context.
		std::vector<std::shared_ptr<audio_buffer>> m_buffers;

		/// List of audio sources owned by the audio context.
		std::vector<std::shared_ptr<audio_source>> m_sources;

		/// List of active audio commands.
		std::vector<generic_audio_command> m_commands;

		/// Audio context thread.
		std::jthread m_thread;

		/// Mutex protecting the audio context.
		std::mutex m_mutex;

		//

		/// Function used by the audio context thread.
		/// @param stoken Thread stop token.
		void thread_loop(std::stop_token stoken);

		//

		/// Creates an audio command on the context.
		/// @tparam T Argument type.
		/// @param source Audio source to command.
		/// @param method Method the command calls when setting the value.
		/// @param begin Initial value of the property being set.
		/// @param end Final value of the property being set.
		/// @param length Length of the command.
		template <typename T>
		void create_command(audio_source& source, audio_command<T>::method_type method, T begin, T end, fsecs length);

		/// Creates an audio command on the context.
		/// @tparam Ts Argument types.
		/// @param source Audio source to command.
		/// @param method Method the command calls when setting the value.
		/// @param begin Initial values of the property being set.
		/// @param end Final values of the property being set.
		/// @param length Length of the command.
		template <typename... Ts>
		void create_command(audio_source& source, audio_command<Ts...>::method_type method, const std::tuple<Ts...>& begin,
							const std::tuple<Ts...>& end, fsecs length);

		//

		// Accesses the raw OpenAL context pointer and the al_api instance.
		friend class audio_buffer;

		// Accesses the raw OpenAL context pointer, the al_api instance, and create_command.
		friend class audio_source;

		// Accesses m_buffers and m_mutex.
		friend std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context);

		// Accesses m_max_sources and m_sources.
		friend std::shared_ptr<audio_source> create_audio_source(audio_context& context, int priority);
	};
} // namespace tr

#include "impl/audio_context.hpp" // IWYU pragma: export