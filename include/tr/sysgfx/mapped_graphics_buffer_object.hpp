/// @file
/// @brief Provides `tr::mapped_graphics_buffer_object`.

#pragma once
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>

//

namespace tr
{
	/// Map of a graphics buffer object.
	/// @details
	/// `tr::mapped_graphics_buffer_object` represents the CPU mapping of a graphics buffer object and models access to it as such. The
	/// object is automatically unmapped when the map is destroyed.
	///
	/// Every instance of `tr::mapped_graphics_buffer_object` is associated with a graphics context, as well as with a buffer, and
	/// cannot outlive either.
	///
	/// Moved-from instances of `tr::mapped_graphics_buffer_object` are left in a special 'invalid' state. Invalid
	/// `tr::mapped_graphics_buffer_object` instances may not be interacted with besides moving a new value into them.
	/// @tparam Object Type of the mapped object.
	template <typename Object>
	class mapped_graphics_buffer_object
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// @cond implementation_details

		/// Wraps an untyped buffer map.
		/// @param map Untyped buffer map.
		[[nodiscard]] explicit mapped_graphics_buffer_object(mapped_untyped_graphics_buffer_span&& map) noexcept
			: m_raw_map{std::move(map)}
		{
		}

		/// @endcond

		/// Graphics buffer object maps are not copyable.
		mapped_graphics_buffer_object(const mapped_graphics_buffer_object&) = delete;

		/// Moves a graphics buffer object map.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Graphics buffer object map to move.
		[[nodiscard]] mapped_graphics_buffer_object(mapped_graphics_buffer_object&& rhs) = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Graphics buffer object maps are not copyable.
		mapped_graphics_buffer_object& operator=(const mapped_graphics_buffer_object&) = delete;

		/// Moves a graphics buffer object map.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Graphics buffer object map to move.
		/// @return Reference to `*this`.
		mapped_graphics_buffer_object& operator=(mapped_graphics_buffer_object&& rhs) = default;

		/// @}
		/// @name Access
		/// @{

		/// Gets a reference to the object.
		/// @return Reference to the contained object.
		[[nodiscard]] operator Object&() const noexcept
		{
			return get();
		}

		/// Gets a reference to the object.
		/// @return Reference to the contained object.
		[[nodiscard]] Object& get() const noexcept
		{
			return as_mut_object<Object>(m_raw_map.span());
		}

		/// Gets a reference to the object.
		/// @return Reference to the contained object.
		[[nodiscard]] Object& operator*() const noexcept
		{
			return get();
		}

		/// Pointer access to the mapped object.
		/// @return Pointer to the contained object.
		[[nodiscard]] Object* operator->() const noexcept
		{
			return std::addressof(get());
		}

		/// Assigns the object.
		/// @tparam ObjectAssignable Type assignable to `Object`.
		/// @param rhs Value to assign to the contained object.
		/// @return Reference to the contained object.
		template <std::assignable_from<Object> ObjectAssignable>
		[[nodiscard]] Object& operator=(ObjectAssignable&& rhs) const noexcept(std::is_nothrow_assignable_v<Object, ObjectAssignable>)
		{
			return get() = std::forward<ObjectAssignable>(rhs);
		}

		/// @}
	  private:
		/// Wrapped untyped buffer map.
		mapped_untyped_graphics_buffer_span m_raw_map;
	};
} // namespace tr