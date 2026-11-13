/// @file
/// @brief Provides `tr::internal::texture_unit`.

#pragma once
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	class graphics_context;
	class texture_view;
} // namespace tr

//

namespace tr::internal
{
	/// @brief Location a texture can be set to.
	class texture_unit
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Allocates a texture unit on a graphics context.
		/// @param context Context to allocate the texture unit on.
		/// @pre `context` does not already have 80 allocated texture units.
		[[nodiscard]] explicit texture_unit(graphics_context& context) noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets a texture to the texture unit.
		/// @param texture Texture to set on the texture unit.
		void set(texture_view texture) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @return Returns the handle of the texture unit.
		/// @return Handle of the texture unit.
		[[nodiscard]] int handle() const noexcept;

		/// @}

	  private:
		/// @brief Graphics context texture unit freer.
		struct graphics_context_texture_unit_freer
		{
			/// Reference to the graphics context the texture unit is on.
			ref<graphics_context> context;

			//

			/// @brief Frees a texture unit.
			/// @param unit Handle to a texture unit.
			void operator()(int unit) const noexcept;
		};

		//

		/// @brief Managed handle to a texture unit.
		unique_handle<int, -1, graphics_context_texture_unit_freer> m_handle;
	};
} // namespace tr::internal