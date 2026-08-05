/// @file
/// @brief Provides a structure holding OpenAL API functions.

#pragma once

struct ALCcontext;
struct ALCdevice;

//

namespace tr
{
	/// Structure holding OpenAL API functions.
	struct al_api
	{
		/// Pointer to alDeleteBuffersDirect.
		void (*delete_buffers)(ALCcontext* context, int n, const unsigned int* buffers);

		/// Pointer to alDeleteSourcesDirect.
		void (*delete_sources)(ALCcontext* context, int n, const unsigned int* sources);

		/// Pointer to alGenBuffersDirect.
		void (*generate_buffers)(ALCcontext* context, int n, unsigned int* buffers);

		/// Pointer to alGenSourcesDirect.
		void (*generate_sources)(ALCcontext* context, int n, unsigned int* sources);

		/// Pointer to alGetBufferiDirect.
		void (*get_buffer_property_i)(ALCcontext* context, unsigned int buffer, unsigned int param, int* value);

		/// Pointer to alGetBufferivDirect.
		void (*get_buffer_property_iv)(ALCcontext* context, unsigned int source, unsigned int param, int* values);

		/// Pointer to alGetErrorDirect.
		unsigned int (*get_error)(ALCcontext* context);

		/// Pointer to alGetListenerfDirect.
		void (*get_listener_property_f)(ALCcontext* context, unsigned int param, float* value);

		/// Pointer to alGetListenerfvDirect.
		void (*get_listener_property_fv)(ALCcontext* context, unsigned int param, float* values);

		/// Pointer to alGetSourcefDirect.
		void (*get_source_property_f)(ALCcontext* context, unsigned int source, unsigned int param, float* value);

		/// Pointer to alGetSourcefvDirect.
		void (*get_source_property_fv)(ALCcontext* context, unsigned int source, unsigned int param, float* values);

		/// Pointer to alGetSourceiDirect.
		void (*get_source_property_i)(ALCcontext* context, unsigned int source, unsigned int param, int* value);

		/// Pointer to alBufferDataDirect.
		void (*set_buffer_data)(ALCcontext* context, unsigned int buffer, unsigned int format, const void* data, int size, int sample_rate);

		/// Pointer to alBufferivDirect.
		void (*set_buffer_property_iv)(ALCcontext* context, unsigned int buffer, unsigned int param, const int* values);

		/// Pointer to alListenerfDirect.
		void (*set_listener_property_f)(ALCcontext* context, unsigned int param, float value);

		/// Pointer to alListenerfvDirect.
		void (*set_listener_property_fv)(ALCcontext* context, unsigned int param, const float* values);

		/// Pointer to alSourcefDirect.
		void (*set_source_property_f)(ALCcontext* context, unsigned int source, unsigned int param, float value);

		/// Pointer to alSourcefvDirect.
		void (*set_source_property_fv)(ALCcontext* context, unsigned int source, unsigned int param, const float* values);

		/// Pointer to alSourceiDirect.
		void (*set_source_property_i)(ALCcontext* context, unsigned int source, unsigned int param, int value);

		/// Pointer to alSourcePauseDirect.
		void (*source_pause)(ALCcontext* context, unsigned int source);

		/// Pointer to alSourcePlayDirect.
		void (*source_play)(ALCcontext* context, unsigned int source);

		/// Pointer to alSourceQueueBuffersDirect.
		void (*source_queue_buffers)(ALCcontext* context, unsigned int source, int n, const unsigned int* buffers);

		/// Pointer to alSourceStopDirect.
		void (*source_stop)(ALCcontext* context, unsigned int source);

		/// Pointer to alSourceUnqueueBuffersDirect.
		void (*source_unqueue_buffers)(ALCcontext* context, unsigned int source, int n, unsigned int* buffers);

		//

		/// Loads OpenAL functions.
		/// @param device Pointer to an OpenAL device.
		al_api(ALCdevice* device);
	};
} // namespace tr