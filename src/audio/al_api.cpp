/// @file
/// @brief Implements al_api.hpp.

#include "../../include/tr/audio/al_api.hpp"
#include <AL/alc.h>

//

namespace tr
{
	namespace
	{
		// Hack needed to overload loaded_gl_function_proxy conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		using function_pointer = Return (*)(Args...);

		//

		/// Wrapper around a void pointer that automatically casts it to a function pointer type.
		struct loaded_al_function_proxy
		{
			/// Base pointer type.
			void* ptr;

			//

			/// Conversion operator.
			/// @tparam Return Function return type.
			/// @tparam Args Function argument types.
			template <typename Return, typename... Args>
			operator function_pointer<Return, Args...>()
			{
				return reinterpret_cast<function_pointer<Return, Args...>>(ptr);
			}
		};

		//

		/// Wraps alcGetProcAddress to return an OpenAL function proxy.
		/// @param device Device to get the function address from.
		/// @param name Function name.
		/// @return OpenAL function proxy.
		loaded_al_function_proxy al_function_address(ALCdevice* device, const char* name)
		{
			return {alcGetProcAddress(device, name)};
		}
	} // namespace
} // namespace tr

tr::al_api::al_api(ALCdevice* device)
	: delete_buffers{al_function_address(device, "alDeleteBuffersDirect")}
	, delete_sources{al_function_address(device, "alDeleteSourcesDirect")}
	, generate_buffers{al_function_address(device, "alGenBuffersDirect")}
	, generate_sources{al_function_address(device, "alGenSourcesDirect")}
	, get_buffer_property_i{al_function_address(device, "alGetBufferiDirect")}
	, get_buffer_property_iv{al_function_address(device, "alGetBufferivDirect")}
	, get_error{al_function_address(device, "alGetErrorDirect")}
	, get_listener_property_f{al_function_address(device, "alGetListenerfDirect")}
	, get_listener_property_fv{al_function_address(device, "alGetListenerfvDirect")}
	, get_source_property_f{al_function_address(device, "alGetSourcefDirect")}
	, get_source_property_fv{al_function_address(device, "alGetSourcefvDirect")}
	, get_source_property_i{al_function_address(device, "alGetSourceiDirect")}
	, set_buffer_data{al_function_address(device, "alBufferDataDirect")}
	, set_buffer_property_iv{al_function_address(device, "alBufferivDirect")}
	, set_listener_property_f{al_function_address(device, "alListenerfDirect")}
	, set_listener_property_fv{al_function_address(device, "alListenerfvDirect")}
	, set_source_property_f{al_function_address(device, "alSourcefDirect")}
	, set_source_property_fv{al_function_address(device, "alSourcefvDirect")}
	, set_source_property_i{al_function_address(device, "alSourceiDirect")}
	, source_pause{al_function_address(device, "alSourcePauseDirect")}
	, source_play{al_function_address(device, "alSourcePlayDirect")}
	, source_queue_buffers{al_function_address(device, "alSourceQueueBuffersDirect")}
	, source_stop{al_function_address(device, "alSourceStopDirect")}
	, source_unqueue_buffers{al_function_address(device, "alSourceUnqueueBuffersDirect")}
{
}