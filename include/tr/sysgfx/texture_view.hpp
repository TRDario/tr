/// @file
/// @brief Provides `tr::texture_view`.

#pragma once

//

namespace tr
{
	/// @brief Lightweight optional reference to a two-dimensional image texture accessable to shaders.
	/// @details
	/// `tr::texture_view` is a lightweight optional reference to a specific two-dimensional image texture. It is designed to be cheap to
	/// pass around and copy.
	///
	/// If not empty, the texture view should not outlive the texture it is pointing at, as it will otherwise become dangling.
	///
	/// @warning It is important to stress the distinction between textures and instances of `tr::texture` or other similar classes for the
	/// purposes of this class, see the documentation of `tr::texture` for more information.
	class texture_view
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty texture view.
		[[nodiscard]] constexpr texture_view() noexcept = default;

		/// @cond gl_interop
		/// @brief Constructs a texture view.
		/// @param handle Handle to an OpenGL texture.
		[[nodiscard]] explicit texture_view(unsigned int handle) noexcept;
		/// @endcond

		/// @brief Constructs a texture view by copying from another texture view.
		/// @details Texture views are trivially copyable.
		/// @param rhs Texture view to copy from.
		[[nodiscard]] constexpr texture_view(const texture_view& rhs) noexcept = default;

		/// @brief Replaces the texture view with a view to another texture.
		/// @details Texture views are trivially copyable.
		/// @param rhs Texture view to copy from.
		/// @return Reference to `*this`.
		texture_view& operator=(const texture_view& rhs) noexcept = default;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the texture view points to a texture.
		/// @return `true` if the texture view points to a texture, `false` otherwise.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the texture view is empty.
		/// @return `true` if the texture view is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL texture.
		/// @return Handle to the underlying OpenGL texture.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @}
		/// @name Comparison
		/// @{

		/// @brief Compares whether two texture views point to the same texture (or are both empty).
		/// @details Texture views are trivially equality-comparable.
		/// @return `true` if the views point to the same texture (or are both empty), `false` otherwise.
		[[nodiscard]] friend bool operator==(texture_view, texture_view) noexcept = default;

		/// @}

	  private:
		/// @brief Handle to an OpenGL texture.
		unsigned int m_handle{0};
	};
} // namespace tr