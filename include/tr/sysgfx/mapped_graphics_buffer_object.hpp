/// @file
/// @brief Provides `tr::mapped_graphics_buffer_object`.

#pragma once
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>

//

namespace tr
{
	/// Mapped graphics buffer object.
	/// @tparam Object Object type contained in the map.
	template <typename Object>
	class mapped_graphics_buffer_object
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors
		/// @{

		/// Wraps an untyped buffer map.
		/// @param map Untyped buffer map.
		[[nodiscard]] explicit mapped_graphics_buffer_object(mapped_untyped_graphics_buffer_span&& map) noexcept
			: m_raw_map{std::move(map)}
		{
		}

		/// @}
		/// @endcond
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