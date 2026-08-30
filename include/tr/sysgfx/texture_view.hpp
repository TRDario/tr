/// @file
/// @brief Defines an opaque view to a GPU texture that is passed to many functions.

#pragma once

//

namespace tr
{
	/// Opaque optional view to a GPU texture.
	class texture_view
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty texture view.
		constexpr texture_view() = default;

		/// @cond gl_interop

		/// Wraps an OpenGL texture ID.
		/// @param id OpenGL texture ID.
		explicit texture_view(unsigned int id);

		/// @endcond

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares whether two texture views point to the same texture.
		/// @param lhs, rhs Texture views to compare.
		/// @return Whether two texture views point to the same texture.
		friend bool operator==(texture_view lhs, texture_view rhs) = default;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the view is empty.
		/// @return `true` if the view is empty, `false` otherwise.
		bool empty() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interop
		/// @{

		/// Unwraps the OpenGL texture ID.
		/// @note This does not release the texture.
		/// @return OpenGL texture ID.
		unsigned int unwrap() const;

		/// @}
		/// @endcond

	  private:
		/// OpenGL texture ID.
		unsigned int m_id{0};
	};

	/// Empty texture view constant.
	constexpr texture_view no_texture{};
} // namespace tr