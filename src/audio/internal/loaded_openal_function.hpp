/// @file
/// @brief Provides `tr::internal::loaded_openal_function`.

#pragma once
#include <AL/alc.h>

//

namespace tr::internal
{
	/// @brief Wrapper around a void pointer that automatically casts it to a function pointer type.
	class loaded_openal_function
	{
	  public:
		/// @brief Hack needed to overload the conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		using function_pointer = Return (*)(Args...) noexcept;

		//

		/// @brief Wraps alcGetProcAddress to return an OpenAL function proxy.
		/// @param device Device to get the function address from.
		/// @param name Function name.
		[[nodiscard]] inline loaded_openal_function(ALCdevice* device, const char* name) noexcept
			: m_ptr{alcGetProcAddress(device, name)}
		{
		}

		//

		/// @brief Conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		[[nodiscard]] operator function_pointer<Return, Args...>() noexcept
		{
			return reinterpret_cast<function_pointer<Return, Args...>>(m_ptr);
		}

	  private:
		/// @brief Base pointer type.
		void* m_ptr;
	};
} // namespace tr::internal