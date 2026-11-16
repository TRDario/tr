/// @file
/// @brief Provides `tr::mapped_graphics_buffer_object`.

#pragma once
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/utility/ranges.hpp>

//

namespace tr
{
	/// @brief Object managing a map of an object stored in a graphics buffer.
	/// @details
	/// `tr::mapped_graphics_buffer_object` represents the CPU mapping of a graphics buffer object and models access to it as such. The
	/// object is automatically unmapped when the map is destroyed.
	///
	/// Every instance of `tr::mapped_graphics_buffer_object` is associated with a graphics context, as well as with a buffer, and
	/// cannot outlive either.
	///
	/// Moved-from instances of `tr::mapped_graphics_buffer_object` are left in a special 'invalid' state. Invalid
	/// `tr::mapped_graphics_buffer_object` instances may not be interacted with besides moving a new value into them and checking for
	/// validity using `valid()` or `operator bool()`.
	/// @tparam Object Type of the mapped object.
	template <typename Object>
	class mapped_graphics_buffer_object
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @cond implementation_details
		/// @brief Constructs a graphics buffer object map.
		/// @param span Base graphics buffer span map.
		[[nodiscard]] explicit mapped_graphics_buffer_object(mapped_untyped_graphics_buffer_span&& span) noexcept
			: m_span{std::move(span)}
		{
		}
		/// @endcond

		/// @brief Graphics buffer object maps are not copyable.
		mapped_graphics_buffer_object(const mapped_graphics_buffer_object&) = delete;

		/// @brief Graphics buffer object maps are not copyable.
		mapped_graphics_buffer_object& operator=(const mapped_graphics_buffer_object&) = delete;

		/// @brief Constructs a graphics buffer object map by transferring ownership from another `tr::mapped_graphics_buffer_object`.
		/// @param rhs Graphics buffer object map to move from.
		/// @par Invalidation
		/// All references to the object previously associated with `rhs` remain valid, but are now associated with the equivalent object in
		/// `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] mapped_graphics_buffer_object(mapped_graphics_buffer_object&& rhs) = default;

		/// @brief Destroys the currently held graphics buffer object map and replaces it with one moved from another
		/// `tr::mapped_graphics_buffer_object`.
		/// @param rhs Graphics buffer object map to move from.
		/// @par Invalidation
		/// All references to the object previously associated with `*this` are invalidated.
		///
		/// All references to the object previously associated with `rhs` remain valid, but are now associated with the equivalent object in
		/// `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		mapped_graphics_buffer_object& operator=(mapped_graphics_buffer_object&& rhs) = default;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a reference to the mapped object.
		/// @pre The graphics buffer object map must be in a valid state.
		/// @return Reference to the mapped object.
		[[nodiscard]] operator Object&() const noexcept
		{
			return get();
		}

		/// @brief Returns a reference to the mapped object.
		/// @pre The graphics buffer object map must be in a valid state.
		/// @return Reference to the mapped object.
		[[nodiscard]] Object& get() const noexcept
		{
			TR_ASSERT(valid(), "Tried to get a mapped graphics buffer object in an invalid state.");

			return as_mut_object<Object>(m_span);
		}

		/// @brief Returns a reference to the mapped object.
		/// @pre The graphics buffer object map must be in a valid state.
		/// @return Reference to the mapped object.
		[[nodiscard]] Object& operator*() const noexcept
		{
			return get();
		}

		/// @brief Returns a pointer to the mapped object.
		/// @pre The graphics buffer object map must be in a valid state.
		/// @return Pointer to the mapped object.
		[[nodiscard]] Object* operator->() const noexcept
		{
			return std::addressof(get());
		}

		/// @brief Assigns the mapped object.
		/// @tparam ObjectAssignable Type assignable to `Object`.
		/// @param rhs Value to assign to the mapped object.
		/// @pre The graphics buffer object map must be in a valid state.
		/// @return Reference to the mapped object.
		template <std::assignable_from<Object> ObjectAssignable>
		[[nodiscard]] Object& operator=(ObjectAssignable&& rhs) const noexcept(std::is_nothrow_assignable_v<Object, ObjectAssignable>)
		{
			TR_ASSERT(valid(), "Tried to assign a mapped graphics buffer object in an invalid state.");

			return get() = std::forward<ObjectAssignable>(rhs);
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the graphics buffer object map is in a valid state.
		/// @return `true` if the graphics buffer object map is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept
		{
			return !m_span.empty();
		}

		/// @brief Checks whether the graphics buffer object map is in a valid state.
		/// @return `true` if the graphics buffer object map is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept
		{
			return !m_span.empty();
		}

		/// @brief Returns a reference to the graphics context the graphics buffer object map is on.
		/// @pre The graphics buffer object map must be in a valid state.
		/// @return Reference to the graphics context the graphics buffer object map is on.
		[[nodiscard]] graphics_context& context() const noexcept
		{
			return m_span.context();
		}

		/// @}

	  private:
		/// @brief Base graphics buffer span map.
		mapped_untyped_graphics_buffer_span m_span;
	};
} // namespace tr