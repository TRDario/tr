/// @file
/// @brief Provides `tr::mutable_texture_view`.

#pragma once

namespace tr
{
	enum class pixel_format : int;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// @brief Lightweight optional reference to a mutable two-dimensional image texture accessable to shaders.
	/// @details
	/// `tr::mutable_texture_view` is a lightweight optional reference to a specific mutable two-dimensional image texture. It is designed
	/// to be cheap to pass around and copy.
	///
	/// If not empty, the texture view should not outlive the texture it is pointing at, as it will otherwise become dangling.
	///
	/// @warning It is important to stress the distinction between textures and instances of `tr::texture` or other similar classes for the
	/// purposes of this class, see the documentation of `tr::texture` for more information.
	class mutable_texture_view
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty texture view.
		[[nodiscard]] constexpr mutable_texture_view() noexcept = default;

		/// @cond gl_interop
		/// @brief Constructs a mutable texture view.
		/// @param handle Handle to an OpenGL texture.
		/// @param format Pixel format of the texture.
		[[nodiscard]] explicit mutable_texture_view(unsigned int handle, pixel_format format) noexcept;
		/// @endcond

		/// @brief Constructs a mutable texture view by copying from another mutable texture view.
		/// @details Mutable texture views are trivially copyable.
		/// @param rhs Mutable texture view to copy from.
		[[nodiscard]] constexpr mutable_texture_view(const mutable_texture_view& rhs) noexcept = default;

		/// @brief Replaces the texture view with a view to another texture.
		/// @details Mutable texture views are trivially copyable.
		/// @param rhs Mutable texture view to copy from.
		/// @return Reference to `*this`.
		mutable_texture_view& operator=(const mutable_texture_view& rhs) noexcept = default;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the texture view is empty.
		/// @return `true` if the texture view is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @brief Returns the pixel format of the texture.
		/// @pre The texture view must not be empty.
		/// @return Pixel format of the texture.
		[[nodiscard]] pixel_format format() const noexcept;

		/// @brief Returns an immutable texture view equivalent to `*this`.
		/// @return Immutable texture view equivalent to `*this`.
		[[nodiscard]] operator texture_view() const noexcept;

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
		[[nodiscard]] friend bool operator==(mutable_texture_view, mutable_texture_view) noexcept = default;

		/// @}

	  private:
		/// @brief Handle to an OpenGL texture.
		unsigned int m_handle{0};

		/// @brief Pixel format of the texture.
		pixel_format m_format;
	};
} // namespace tr