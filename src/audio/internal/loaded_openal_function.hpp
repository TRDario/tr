/// @file
/// @brief Provides `tr::loaded_openal_function`.

#pragma once
#include <AL/alc.h>

//

namespace tr
{
	/// Wrapper around a void pointer that automatically casts it to a function pointer type.
	class loaded_openal_function
	{
	  public:
		// Hack needed to overload the conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		using function_pointer = Return (*)(Args...) noexcept;

		//

		/// Wraps alcGetProcAddress to return an OpenAL function proxy.
		/// @param device Device to get the function address from.
		/// @param name Function name.
		[[nodiscard]] inline loaded_openal_function(ALCdevice* device, const char* name) noexcept
			: m_ptr{alcGetProcAddress(device, name)}
		{
		}

		//

		/// Conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		[[nodiscard]] operator function_pointer<Return, Args...>() noexcept
		{
			return reinterpret_cast<function_pointer<Return, Args...>>(m_ptr);
		}

	  private:
		/// Base pointer type.
		void* m_ptr;
	};
} // namespace tr