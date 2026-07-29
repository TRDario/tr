///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Defines an opaque view to a GPU texture that is passed to many functions.                                                             //
//                                                                                                                                       //
// Views may be empty, which can be checked with the eponymous method. tr::no_texture is provided as an empty texture view constant.     //
// Not all functions accept empty texture views.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef TR_HAS_IMGUI
using ImTextureID = unsigned long long;
namespace tr {
	class texture_view;
	namespace ImGui {
		ImTextureID GetTextureID(texture_view texture);
	}
} // namespace tr
#endif

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Opaque view to a GPU texture.
	class texture_view {
	  public:
		// Creates an empty texture view.
		constexpr texture_view() = default;

		// Compares whether two texture views point to the same texture.
		friend bool operator==(texture_view lhs, texture_view rhs) = default;

		// Gets whether the view is empty.
		bool empty() const;

	  private:
		// An OpenGL texture ID.
		unsigned int m_id{0};

		// Creates a texture view.
		texture_view(unsigned int id);

		friend class framebuffer;
		friend class texture;
		friend class texture_unit;
#ifdef TR_HAS_IMGUI
		friend ImTextureID ImGui::GetTextureID(texture_view texture);
#endif
	};
	// Empty texture view constant.
	constexpr texture_view no_texture{};
} // namespace tr