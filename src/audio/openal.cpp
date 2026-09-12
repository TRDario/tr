/// @file
/// @brief Implements internal/openal.hpp.

#include "internal/loaded_openal_function.hpp"
#include <AL/alc.h>
#include <tr/audio/internal/openal.hpp>

//

tr::internal::openal::openal(ALCdevice* device) noexcept
	: delete_buffers{loaded_openal_function{device, "alDeleteBuffersDirect"}}
	, delete_sources{loaded_openal_function{device, "alDeleteSourcesDirect"}}
	, generate_buffers{loaded_openal_function{device, "alGenBuffersDirect"}}
	, generate_sources{loaded_openal_function{device, "alGenSourcesDirect"}}
	, get_buffer_property_i{loaded_openal_function{device, "alGetBufferiDirect"}}
	, get_buffer_property_iv{loaded_openal_function{device, "alGetBufferivDirect"}}
	, get_error{loaded_openal_function{device, "alGetErrorDirect"}}
	, get_listener_property_f{loaded_openal_function{device, "alGetListenerfDirect"}}
	, get_listener_property_fv{loaded_openal_function{device, "alGetListenerfvDirect"}}
	, get_source_property_f{loaded_openal_function{device, "alGetSourcefDirect"}}
	, get_source_property_fv{loaded_openal_function{device, "alGetSourcefvDirect"}}
	, get_source_property_i{loaded_openal_function{device, "alGetSourceiDirect"}}
	, set_buffer_data{loaded_openal_function{device, "alBufferDataDirect"}}
	, set_buffer_property_iv{loaded_openal_function{device, "alBufferivDirect"}}
	, set_listener_property_f{loaded_openal_function{device, "alListenerfDirect"}}
	, set_listener_property_fv{loaded_openal_function{device, "alListenerfvDirect"}}
	, set_source_property_f{loaded_openal_function{device, "alSourcefDirect"}}
	, set_source_property_fv{loaded_openal_function{device, "alSourcefvDirect"}}
	, set_source_property_i{loaded_openal_function{device, "alSourceiDirect"}}
	, source_pause{loaded_openal_function{device, "alSourcePauseDirect"}}
	, source_play{loaded_openal_function{device, "alSourcePlayDirect"}}
	, source_queue_buffers{loaded_openal_function{device, "alSourceQueueBuffersDirect"}}
	, source_stop{loaded_openal_function{device, "alSourceStopDirect"}}
	, source_unqueue_buffers{loaded_openal_function{device, "alSourceUnqueueBuffersDirect"}}
{
}