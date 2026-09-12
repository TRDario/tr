/// @file
/// @brief Provides a structure holding OpenAL API functions.

#pragma once

struct ALCcontext;
struct ALCdevice;

//

namespace tr::internal
{
	/// Structure holding OpenAL API functions.
	struct openal
	{
		/// Pointer to alDeleteBuffersDirect.
		void (*delete_buffers)(ALCcontext* context, int n, const unsigned int* buffers) noexcept;

		/// Pointer to alDeleteSourcesDirect.
		void (*delete_sources)(ALCcontext* context, int n, const unsigned int* sources) noexcept;

		/// Pointer to alGenBuffersDirect.
		void (*generate_buffers)(ALCcontext* context, int n, unsigned int* buffers) noexcept;

		/// Pointer to alGenSourcesDirect.
		void (*generate_sources)(ALCcontext* context, int n, unsigned int* sources) noexcept;

		/// Pointer to alGetBufferiDirect.
		void (*get_buffer_property_i)(ALCcontext* context, unsigned int buffer, unsigned int param, int* value) noexcept;

		/// Pointer to alGetBufferivDirect.
		void (*get_buffer_property_iv)(ALCcontext* context, unsigned int source, unsigned int param, int* values) noexcept;

		/// Pointer to alGetErrorDirect.
		[[nodiscard]] unsigned int (*get_error)(ALCcontext* context) noexcept;

		/// Pointer to alGetListenerfDirect.
		void (*get_listener_property_f)(ALCcontext* context, unsigned int param, float* value) noexcept;

		/// Pointer to alGetListenerfvDirect.
		void (*get_listener_property_fv)(ALCcontext* context, unsigned int param, float* values) noexcept;

		/// Pointer to alGetSourcefDirect.
		void (*get_source_property_f)(ALCcontext* context, unsigned int source, unsigned int param, float* value) noexcept;

		/// Pointer to alGetSourcefvDirect.
		void (*get_source_property_fv)(ALCcontext* context, unsigned int source, unsigned int param, float* values) noexcept;

		/// Pointer to alGetSourceiDirect.
		void (*get_source_property_i)(ALCcontext* context, unsigned int source, unsigned int param, int* value) noexcept;

		/// Pointer to alBufferDataDirect.
		void (*set_buffer_data)(ALCcontext* context, unsigned int buffer, unsigned int format, const void* data, int size,
								int sample_rate) noexcept;

		/// Pointer to alBufferivDirect.
		void (*set_buffer_property_iv)(ALCcontext* context, unsigned int buffer, unsigned int param, const int* values) noexcept;

		/// Pointer to alListenerfDirect.
		void (*set_listener_property_f)(ALCcontext* context, unsigned int param, float value) noexcept;

		/// Pointer to alListenerfvDirect.
		void (*set_listener_property_fv)(ALCcontext* context, unsigned int param, const float* values) noexcept;

		/// Pointer to alSourcefDirect.
		void (*set_source_property_f)(ALCcontext* context, unsigned int source, unsigned int param, float value) noexcept;

		/// Pointer to alSourcefvDirect.
		void (*set_source_property_fv)(ALCcontext* context, unsigned int source, unsigned int param, const float* values) noexcept;

		/// Pointer to alSourceiDirect.
		void (*set_source_property_i)(ALCcontext* context, unsigned int source, unsigned int param, int value) noexcept;

		/// Pointer to alSourcePauseDirect.
		void (*source_pause)(ALCcontext* context, unsigned int source) noexcept;

		/// Pointer to alSourcePlayDirect.
		void (*source_play)(ALCcontext* context, unsigned int source) noexcept;

		/// Pointer to alSourceQueueBuffersDirect.
		void (*source_queue_buffers)(ALCcontext* context, unsigned int source, int n, const unsigned int* buffers) noexcept;

		/// Pointer to alSourceStopDirect.
		void (*source_stop)(ALCcontext* context, unsigned int source) noexcept;

		/// Pointer to alSourceUnqueueBuffersDirect.
		void (*source_unqueue_buffers)(ALCcontext* context, unsigned int source, int n, unsigned int* buffers) noexcept;

		//

		/// Loads OpenAL functions.
		/// @param device Pointer to an OpenAL device.
		[[nodiscard]] openal(ALCdevice* device) noexcept;
	};
} // namespace tr::internal