/// @file
/// @brief Provides `tr::texture_view`.

#pragma once

//

namespace tr
{
	/// Lightweight optional reference to a texture object.
	/// @details
	/// `tr::texture_view` is a lightweight optional reference to a specific texture object. It is designed to be cheap to pass around and
	/// copy.
	///
	/// If not empty, the texture view should not outlive the texture object it is pointing at, as it will otherwise become dangling.
	///
	/// @warning It is important to stress the distinction between texture objects and instances of `tr::texture` or other similar classes
	/// for the purposes of this class, see the documentation of `tr::texture` for more information.
	class texture_view
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an empty texture view.
		[[nodiscard]] constexpr texture_view() noexcept = default;

		/// @cond gl_interop

		/// Wraps an OpenGL texture ID.
		/// @param id OpenGL texture ID.
		[[nodiscard]] explicit texture_view(unsigned int id) noexcept;

		/// @endcond

		/// Texture views are trivially copyable.
		[[nodiscard]] constexpr texture_view(const texture_view&) noexcept = default;

		/// Texture views are trivially movable.
		[[nodiscard]] constexpr texture_view(texture_view&&) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Texture views are trivially copyable.
		/// @return Reference to `*this`.
		texture_view& operator=(const texture_view&) noexcept = default;

		/// Texture views are trivially movable.
		/// @return Reference to `*this`.
		texture_view& operator=(texture_view&&) noexcept = default;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Texture views are trivially equality-comparable.
		/// @return `true` if the views point to the same texture object, `false` otherwise.
		[[nodiscard]] friend bool operator==(texture_view, texture_view) noexcept = default;

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