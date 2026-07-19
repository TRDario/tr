///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a smart texture reference class.                                                                                             //
//                                                                                                                                       //
// tr::texture_ref represents a reference to logical textures (As opposed to just a pointer) and will continue to reference the correct  //
// texture even after moves or swaps. Additionally, it will automatically get emptied when the referenced texture is destroyed (this can //
// be checked with the .empty() method)                                                                                                  //
//     - tr::texture tex1, tex2;                                                                                                         //
//       tr::texture_ref ref{tex1}                                                                                                       //
//       std::swap(tex1, tex2)                                                                                                           //
//       -> ref now points to tex2                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/reference.hpp"

namespace tr {
	class texture;
}

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Smart texture reference (updated on texture moves and updates, emptied on deletion).
	class texture_ref {
	  public:
		// Creates an empty reference.
		constexpr texture_ref() = default;
		// Creates an empty reference.
		constexpr texture_ref(std::nullopt_t) {};
		// Creates a non-empty reference.
		texture_ref(const texture& tex);
		// Creating a reference to an rvalue is not allowed.
		texture_ref(texture&& tex) = delete;
		// Copy-constructs a reference.
		texture_ref(const texture_ref& r);
		// Move-constructs a reference.
		texture_ref(texture_ref&& r) noexcept;
		// Destroys the reference.
		~texture_ref();

		// Clears the reference.
		texture_ref& operator=(std::nullopt_t);
		// Reassigns the reference.
		texture_ref& operator=(const texture& tex);
		// Reassigning to an rvalue is not allowed.
		texture_ref& operator=(texture&& tex) = delete;
		// Copy-assigns the reference.
		texture_ref& operator=(const texture_ref& r);
		// Move-assigns the reference.
		texture_ref& operator=(texture_ref&& r) noexcept;

		// Compares references for equality.
		friend bool operator==(const texture_ref& l, const texture_ref& r) = default;

		// Dereferences the texture.
		const texture& operator*() const;
		// Dereferences the texture.
		const texture* operator->() const;

		// Checks whether the reference is empty.
		bool empty() const;

	  private:
		// A reference to a texture.
		opt_ref<const texture> m_ref{};

		// Unbinds the reference.
		void unbind();
		// Rebinds the reference.
		void rebind(const texture& texture);

		friend class texture;
#ifdef TR_HAS_IMGUI
		friend ImTextureID ImGui::GetTextureID(const texture_ref& texture);
#endif
	};
} // namespace tr