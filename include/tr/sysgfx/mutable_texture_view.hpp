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
	/// Lightweight optional reference to a mutable texture object.
	/// @details
	/// `tr::texture_view` is a lightweight optional reference to a specific mutable texture object. It is designed to be cheap to pass
	/// around and copy.
	///
	/// If not empty, the texture view should not outlive the texture object it is pointing at, as it will otherwise become dangling.
	///
	/// @warning It is important to stress the distinction between texture objects and instances of `tr::texture` or other similar classes
	/// for the purposes of this class, see the documentation of `tr::texture` for more information.
	class mutable_texture_view
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an empty texture view.
		[[nodiscard]] constexpr mutable_texture_view() noexcept = default;

		/// @cond gl_interop

		/// Wraps an OpenGL texture ID.
		/// @param id OpenGL texture ID.
		/// @param format Texture pixel format.
		[[nodiscard]] explicit mutable_texture_view(unsigned int id, pixel_format format) noexcept;

		/// @endcond

		/// Texture views are trivially copyable.
		[[nodiscard]] constexpr mutable_texture_view(const mutable_texture_view&) noexcept = default;

		/// Texture views are trivially movable.
		[[nodiscard]] constexpr mutable_texture_view(mutable_texture_view&&) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Texture views are trivially copyable.
		/// @return Reference to `*this`.
		mutable_texture_view& operator=(const mutable_texture_view&) noexcept = default;

		/// Texture views are trivially movable.
		/// @return Reference to `*this`.
		mutable_texture_view& operator=(mutable_texture_view&&) noexcept = default;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Texture views are trivially equality-comparable.
		/// @return `true` if the views point to the same texture object, `false` otherwise.
		[[nodiscard]] friend bool operator==(mutable_texture_view, mutable_texture_view) noexcept = default;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets the base texture view equivalent to `*this`.
		/// @return Base texture view equivalent to `*this`.
		[[nodiscard]] operator texture_view() const noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the view is empty.
		/// @return `true` if the view is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// Gets the pixel format of the texture.
		/// @return Pixel format of the texture.
		[[nodiscard]] pixel_format format() const noexcept;

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

		/// Pixel format of the texture.
		pixel_format m_format;
	};
} // namespace tr