/// @file
/// @brief Provides `tr::atlas_packer`.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// @ingroup utility
	/// @brief Skyline rectangle packer intended for use in atlas textures.
	/// @details
	/// `tr::atlas_packer` is a minimal packer object intended to be used in tandem with other objects (rectangle map, sized texture) to
	/// build an atlas. The packer does not keep track of any information besides the points of the silhouette. In order to ensure the
	/// results of the algorithm are valid, the texture size passed into `try_insert()` must always be equal to or greater than its value
	/// during a previous call. That is to say, the algorithm expects that the atlas texture can only ever grow, not shrink.
	///
	/// The packing algorithm leaves all entries with a unit of leeway in both dimensions to prevent filtering bleed.
	class atlas_packer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Creates an empty packer.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] atlas_packer();

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Clears the packer.
		/// @details This resets the minimum texture size expectation of `try_insert()` back to (0, 0).
		void clear() noexcept;

		/// @brief Attempts to insert a rectangle.
		/// @param size Size of the rectangle.
		/// @param texture_size Size of the texture to insert the rectangle into.
		/// @pre `texture_size` must be equal to or greater than its value on the previous `try_insert()` call. When calling for the first
		/// time after construction or clearing, it may be any value, including (0, 0).
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return Position of the top-left corner of the rectangle, if found.
		[[nodiscard]] std::optional<glm::u16vec2> try_insert(glm::u16vec2 size, glm::u16vec2 texture_size);

		/// @}

	  private:
		/// @brief Defined skyline silhouette points.
		std::vector<glm::u16vec2> m_skyline;
	};
} // namespace tr