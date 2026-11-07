/// @file
/// @brief Provides `tr::internal::loaded_opengl_function`.

#pragma once
#include <SDL3/SDL.h>

//

namespace tr::internal
{
	/// @brief Wrapper around a void pointer that automatically casts it to a function pointer type.
	class loaded_opengl_function
	{
	  public:
		/// @brief Hack needed to overload the conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		using function_pointer = Return (*)(Args...) noexcept;

		//

		/// @brief Wraps SDL_GL_GetProcAddress to return an OpenGL function proxy.
		/// @param name Function name.
		[[nodiscard]] inline loaded_opengl_function(const char* name) noexcept
			: m_ptr{SDL_GL_GetProcAddress(name)}
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
		SDL_FunctionPointer m_ptr;
	};
} // namespace tr::internal