/// @file
/// @brief Provides `tr::internal::graphics_object_id` and `tr::internal::graphics_object_id_handle`.

#pragma once

//

namespace tr::internal
{
	/// Opaque ID for graphics objects.
	enum class graphics_object_id : unsigned int
	{
		/// Sentinel value for an invalid graphics object ID.
		invalid
	};

	/// Owning handle to a unique graphics object ID.
	class graphics_object_id_handle
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a new graphics object ID handle.
		[[nodiscard]] graphics_object_id_handle() noexcept;

		/// Moves a graphics object ID handle.
		/// @param rhs Handle to move.
		[[nodiscard]] graphics_object_id_handle(graphics_object_id_handle&& rhs) noexcept;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Moves a graphics object ID handle.
		/// @param rhs Handle to move.
		graphics_object_id_handle& operator=(graphics_object_id_handle&& rhs) noexcept;

		/// @}
		/// @name Constructors and destructors
		/// @{

		/// Gets the base graphics object ID.
		/// @return Base ID.
		[[nodiscard]] operator graphics_object_id() const noexcept;

		/// @}

	  private:
		/// Base ID.
		graphics_object_id m_id;
	};
} // namespace tr::internal