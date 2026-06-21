///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio context class.                                                                                                      //
//                                                                                                                                       //
// tr::audio_context is a class responsible for holding audio state (buffers, sources). Every audio context is located on an audio       //
// device (a device can have multiple active contexts on it):                                                                            //
//     - tr::audio_context context{device} -> creates an audio context on 'device'                                                       //
// Leaving audio context alive after their device is erroneous.                                                                          //
//                                                                                                                                       //
// The master gain, as well as the gains of any of the audio classes (see audio_class.hpp) can be gotten and set:                        //
//     - context.set_master_gain(context.master_gain() / 2) -> halve the master gain                                                     //
//     - context.set_class_gain(1, context.class_gain(1) * 2) -> double the gain of audio class 1                                        //
//                                                                                                                                       //
// Every context models one listener whose position, velocity, and orientation in 3D space can be set:                                   //
//     - context.listener_pos() -> {0, 0, 0}                                                                                             //
//     - context.set_listener_pos({5, 0, 5}) -> listener is now at {5, 0, 5}                                                             //
//     - context.listener_vel() -> {0, 0, 0}                                                                                             //
//     - context.set_listener_vel({5, 0, 5}) -> listener is now treated as if it has a velocity of {5, 0, 5}                             //
//     - context.listener_orientation() -> gets the listener's orientation                                                               //
//     - context.set_listener_orientation(view, up) -> sets the listener's orientation                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/angle.hpp"
#include "../utility/exception.hpp"
#include "../utility/reference.hpp"
#include "audio_buffer.hpp"
#include "audio_class.hpp"

struct ALCcontext;
struct ALCdevice;
namespace tr {
	class audio_device;
	class audio_stream;
	class audio_source;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Listener orientation.
	struct orientation {
		// View vector.
		glm::vec3 view;
		// Up vector.
		glm::vec3 up;

		friend bool operator==(const orientation&, const orientation&) = default;
	};

	// Error thrown when audio context creation fails.
	class audio_context_init_error final : public tr::exception {
	  public:
		// Creates an audio context initialization error.
		audio_context_init_error(ALCdevice* device);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Error description.
		std::string_view m_description;
	};

	// Object containing all audio context state.
	class audio_context {
	  public:
		// Creates an audio context on an audio device.
		// May throw: audio_context_init_error.
		audio_context(audio_device& device);
		// Audio contexts are not copyable.
		audio_context(const audio_context&) = delete;
		// Audio contexts are not movable.
		audio_context(audio_context&&) = delete;

		// Audio contexts are not copyable.
		audio_context& operator=(const audio_context&) = delete;
		// Audio contexts are not movable.
		audio_context& operator=(audio_context&&) = delete;

		// Gets the master gain.
		float master_gain() const;
		// Sets the master gain.
		void set_master_gain(float gain);

		// Gets an audio class's gain modifier.
		float class_gain(audio_class_id id) const;
		// Sets an audio class's gain modifier.
		void set_class_gain(audio_class_id id, float gain);

		// Gets the position of the listener.
		glm::vec3 listener_position() const;
		// Sets the position of the listener.
		void set_listener_position(const glm::vec3& position);

		// Gets the velocity of the listener.
		glm::vec3 listener_velocity() const;
		// Sets the velocity of the listener.
		void set_listener_velocity(const glm::vec3& velocity);

		// Gets the orientation of the listener.
		orientation listener_orientation() const;
		// Sets the orientation of the listener.
		void set_listener_orientation(orientation orientation);

	  private:
		// Destroys the audio context.
		struct deleter {
			void operator()(ALCcontext* context) const;
		};

		// Structure holding OpenAL API functions.
		struct al_functions {
			void (*delete_buffers)(ALCcontext* context, int n, const unsigned int* buffers);
			void (*delete_sources)(ALCcontext* context, int n, const unsigned int* sources);
			void (*generate_buffers)(ALCcontext* context, int n, unsigned int* buffers);
			void (*generate_sources)(ALCcontext* context, int n, unsigned int* sources);
			void (*get_buffer_property_i)(ALCcontext* context, unsigned int buffer, unsigned int param, int* value);
			void (*get_buffer_property_iv)(ALCcontext* context, unsigned int source, unsigned int param, int* values);
			unsigned int (*get_error)(ALCcontext* context);
			void (*get_listener_property_f)(ALCcontext* context, unsigned int param, float* value);
			void (*get_listener_property_fv)(ALCcontext* context, unsigned int param, float* values);
			void (*get_source_property_f)(ALCcontext* context, unsigned int source, unsigned int param, float* value);
			void (*get_source_property_fv)(ALCcontext* context, unsigned int source, unsigned int param, float* values);
			void (*get_source_property_i)(ALCcontext* context, unsigned int source, unsigned int param, int* value);
			void (*set_buffer_data)(ALCcontext* context, unsigned int buffer, unsigned int format, const void* data, int size,
									int sample_rate);
			void (*set_buffer_property_iv)(ALCcontext* context, unsigned int buffer, unsigned int param, const int* values);
			void (*set_listener_property_f)(ALCcontext* context, unsigned int param, float value);
			void (*set_listener_property_fv)(ALCcontext* context, unsigned int param, const float* values);
			void (*set_source_property_f)(ALCcontext* context, unsigned int source, unsigned int param, float value);
			void (*set_source_property_fv)(ALCcontext* context, unsigned int source, unsigned int param, const float* values);
			void (*set_source_property_i)(ALCcontext* context, unsigned int source, unsigned int param, int value);
			void (*source_pause)(ALCcontext* context, unsigned int source);
			void (*source_play)(ALCcontext* context, unsigned int source);
			void (*source_queue_buffers)(ALCcontext* context, unsigned int source, int n, const unsigned int* buffers);
			void (*source_stop)(ALCcontext* context, unsigned int source);
			void (*source_unqueue_buffers)(ALCcontext* context, unsigned int source, int n, unsigned int* buffers);

			// Loads OpenAL functions.
			al_functions(ALCdevice* device);
		};

		// Audio command template.
		template <typename First, typename... Rest> class command {
		  private:
			template <typename T> using add_cref_if_large_t = std::conditional_t<(sizeof(T) > 8), const T&, T>;

		  public:
			// Method signature used by the command.
			using method_type = void (audio_source::*)(add_cref_if_large_t<First>, add_cref_if_large_t<Rest>...);
			// command<float>::value_type = float, command<float, float>::value_type = std::tuple<float, float>.
			using value_type = std::conditional_t<sizeof...(Rest), std::tuple<First, Rest...>, First>;

			// Audio command status returned after execution.
			enum class status : bool {
				ongoing, // The command is still ongoing.
				done     // The command is done.
			};

			// Constructs a command.
			command(audio_source& source, method_type method, const value_type& begin, const value_type& end, fsecs length);

			// Gets the source the command is commanding.
			audio_source& source() const;
			// Resets the reference after a source is moved.
			void reset_moved_source_reference(audio_source& source);

			// Executes the command and returns the subsequent status.
			status execute();

		  private:
			// Source being commanded.
			tr::ref<audio_source> m_source;
			// Method the command calls when setting the value.
			method_type m_method;
			// Initial value of the property being set.
			value_type m_begin;
			// Final value of the property being set.
			value_type m_end;
			// Length of the command.
			duration m_length;
			// When the last update was.
			std::chrono::steady_clock::time_point m_last_update;
			// How much time has elapsed for the command.
			duration m_elapsed;
		};
		// Generic audio command.
		using generic_command = std::variant<command<float>, command<angle, angle>, command<glm::vec3>>;

		// Owning pointer to the OpenAL audio context.
		std::unique_ptr<ALCcontext, deleter> m_ptr;
		// OpenAL function pointers.
		al_functions m_alapi;
		// Maximum allowed number of audio sources.
		usize m_max_sources;
		// The gain multipliers of audio classes.
		std::array<float, audio_class_count> m_class_gains;
		// List of audio buffers owned by the audio context.
		std::vector<std::shared_ptr<audio_buffer>> m_buffers;
		// List of audio sources owned by the audio context.
		std::vector<std::shared_ptr<audio_source>> m_sources;
		// List of active audio commands.
		std::vector<generic_command> m_commands;
		// Audio context thread.
		std::jthread m_thread;
		// Mutex protecting the audio context.
		std::mutex m_mutex;

		// Function used by the audio context thread.
		void thread_loop(std::stop_token stoken);

		// Creates a new audio command.
		template <typename T> void create_command(audio_source& source, command<T>::method_type method, T begin, T end, fsecs length);
		// Creates a new audio command.
		template <typename... Ts>
		void create_command(audio_source& source, command<Ts...>::method_type method, const std::tuple<Ts...>& begin,
							const std::tuple<Ts...>& end, fsecs length);

		friend class audio_buffer;
		friend class audio_source;
		friend std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context);
		friend std::shared_ptr<audio_source> create_audio_source(audio_context& context, int priority);
	};
} // namespace tr

#include "impl/audio_context.hpp" // IWYU pragma: export