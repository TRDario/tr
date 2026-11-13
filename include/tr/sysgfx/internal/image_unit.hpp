/// @file
/// @brief Provides `tr::internal::image_unit`.

#pragma once
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	enum class access : u32;
	class graphics_context;
	class mutable_texture_view;
} // namespace tr

//

namespace tr::internal
{
	/// @brief Image location a texture can be set to.
	class image_unit
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Allocates an image unit on a graphics context.
		/// @param context Context to allocate the image unit on.
		/// @pre `context` does not already have 80 allocated image units.
		[[nodiscard]] explicit image_unit(graphics_context& context) noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets a texture to the image unit.
		/// @param texture Texture to set on the image unit.
		/// @param access Access type of the image.
		void set(mutable_texture_view texture, access access) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @return Returns the handle of the image unit.
		/// @return Handle of the image unit.
		[[nodiscard]] unsigned int handle() const noexcept;

		/// @}

	  private:
		/// @brief Graphics context image unit freer.
		struct graphics_context_image_unit_freer
		{
			/// @brief Reference to the graphics context the image unit is on.
			ref<graphics_context> context;

			//

			/// @brief Frees an image unit.
			/// @param unit Handle to an image unit.
			void operator()(unsigned int unit) const noexcept;
		};

		//

		/// @brief Managed handle to an image unit.
		unique_handle<unsigned int, UINT_MAX, graphics_context_image_unit_freer> m_handle;
	};
} // namespace tr::internal