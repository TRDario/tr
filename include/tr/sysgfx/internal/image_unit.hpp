/// @file
/// @brief Provides an image unit class.

#pragma once
#include <tr/utility/handle.hpp>
#include <tr/utility/integer.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	enum class access : u32;
	class graphics_context;
	class mutable_texture_view;
} // namespace tr

//

namespace tr::internal
{
	/// Image location a texture can bind to.
	class image_unit
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Allocates an image unit on a graphics context.
		/// @param context Context the image unit belongs to.
		[[nodiscard]] explicit image_unit(graphics_context& context) noexcept;

		/// @}

		/// Gets the ID of the image unit.
		/// @return ID of the image unit.
		[[nodiscard]] unsigned int id() const noexcept;

		//

		/// Sets the image unit.
		/// @param texture Texture to set on the image unit.
		/// @param access Access type of the image.
		void set(mutable_texture_view texture, access access) noexcept;

	  private:
		/// Image unit freer.
		struct deleter
		{
			/// Reference to the graphics context the image unit is on.
			ref<graphics_context> context;

			//

			/// Frees the image unit.
			/// @param unit ID of the image unit.
			void operator()(unsigned int unit) const noexcept;
		};

		/// Handle to the image unit.
		handle<unsigned int, UINT_MAX, deleter> m_handle;
	};
} // namespace tr::internal