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
		[[nodiscard]] constexpr texture_view() noexcept = default;

		/// @cond gl_interop

		/// Wraps an OpenGL texture ID.
		/// @param id OpenGL texture ID.
		[[nodiscard]] explicit texture_view(unsigned int id) noexcept;

		/// @endcond

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares whether two texture views point to the same texture.
		/// @param lhs, rhs Texture views to compare.
		/// @return Whether two texture views point to the same texture.
		[[nodiscard]] friend bool operator==(texture_view lhs, texture_view rhs) noexcept = default;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the view is empty.
		/// @return `true` if the view is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL texture.
		/// @note This does not release the texture.
		/// @return OpenGL texture ID.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// OpenGL texture ID.
		unsigned int m_id{0};
	};

	/// Empty texture view constant.
	constexpr texture_view no_texture{};
} // namespace tr