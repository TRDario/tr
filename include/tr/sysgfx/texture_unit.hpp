/// @file
/// @brief Provides a texture unit class.

#pragma once
#include "../utility/handle.hpp"
#include "../utility/reference.hpp"

namespace tr
{
	class graphics_context;
	class texture_view;
} // namespace tr

namespace tr
{
	/// Location a texture can bind to.
	class texture_unit
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates a texture unit on a graphics context.
		/// @param context Context the texture unit belongs to.
		explicit texture_unit(graphics_context& context);

		/// @}

		/// Gets the ID of the texture unit.
		/// @return ID of the texture unit.
		unsigned int id() const;

		//

		/// Sets the texture unit.
		/// @param texture Texture to set on the texture unit.
		void set(texture_view texture);

	  private:
		/// Texture unit freer.
		struct deleter
		{
			/// Reference to the graphics context the texture unit is on.
			ref<graphics_context> context;

			//

			/// Frees the texture unit.
			/// @param unit ID of the texture unit.
			void operator()(unsigned int unit) const;
		};

		/// Handle to the texture unit.
		handle<unsigned int, UINT_MAX, deleter> m_handle;
	};
} // namespace tr