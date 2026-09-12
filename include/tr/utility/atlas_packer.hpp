/// @file
/// @brief Provides `tr::atlas_packer`.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// Skyline rectangle packer for atlas textures.
	/// @note The packer does not keep track of texture information and assumes it is of constant size or growing.
	/// @note All entries are given a pixel of leeway in both dimensions to prevent filtering bleed.
	class atlas_packer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty packer.
		[[nodiscard]] atlas_packer();

		/// @}
		/// @name Manipulation
		/// @{

		/// Clears the packer.
		void clear();

		/// Attempts to insert a rectangle.
		/// @param size Size of the rectangle.
		/// @param texture_size Size of the texture to insert the rectangle into.
		/// @return Position of the top-left corner of the rectangle, if found.
		[[nodiscard]] std::optional<glm::u16vec2> try_insert(glm::u16vec2 size, glm::u16vec2 texture_size);

		/// @}

	  private:
		/// The skyline silhouette points.
		std::vector<glm::u16vec2> m_skyline;
	};
} // namespace tr