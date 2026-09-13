/// @file
/// @brief Provides a texture unit class.

#pragma once
#include <tr/utility/handle.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class graphics_context;
	class texture_view;
} // namespace tr

//

namespace tr::internal
{
	/// Location a texture can bind to.
	class texture_unit
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates a texture unit on a graphics context.
		/// @param context Context the texture unit belongs to.
		[[nodiscard]] explicit texture_unit(graphics_context& context) noexcept;

		/// @}

		/// Gets the ID of the texture unit.
		/// @return ID of the texture unit.
		[[nodiscard]] unsigned int id() const noexcept;

		//

		/// Sets the texture unit.
		/// @param texture Texture to set on the texture unit.
		void set(texture_view texture) noexcept;

	  private:
		/// Texture unit freer.
		struct deleter
		{
			/// Reference to the graphics context the texture unit is on.
			ref<graphics_context> context;

			//

			/// Frees the texture unit.
			/// @param unit ID of the texture unit.
			void operator()(unsigned int unit) const noexcept;
		};

		/// Handle to the texture unit.
		handle<unsigned int, UINT_MAX, deleter> m_handle;
	};
} // namespace tr::internal