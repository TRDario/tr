/// @file
/// @brief Defines an opaque view to a GPU texture that is passed to many functions.

#pragma once

#ifdef TR_HAS_IMGUI
using ImTextureID = unsigned long long;
namespace tr
{
	class texture_view;
	namespace ImGui
	{
		ImTextureID GetTextureID(texture_view texture);
	}
} // namespace tr
#endif

//

namespace tr
{
	/// Opaque optional view to a GPU texture.
	class texture_view
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty texture view.
		constexpr texture_view() = default;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares whether two texture views point to the same texture.
		/// @param lhs, rhs Texture views to compare.
		/// @return Whether two texture views point to the same texture.
		friend bool operator==(texture_view lhs, texture_view rhs) = default;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the view is empty.
		/// @return `true` if the view is empty, `false` otherwise.
		bool empty() const;

		/// @}

	  private:
		/// OpenGL texture ID.
		unsigned int m_id{0};

		//

		/// Creates a texture view.
		/// @param id OpenGL texture ID.
		texture_view(unsigned int id);

		//

		// Accesses m_id.
		friend class framebuffer;

		// Accesses m_id.
		friend class texture;

		// Accesses m_id.
		friend class texture_unit;

		// Accesses m_id.
#ifdef TR_HAS_IMGUI
		friend ImTextureID ImGui::GetTextureID(texture_view texture);
#endif
	};

	/// Empty texture view constant.
	constexpr texture_view no_texture{};
} // namespace tr