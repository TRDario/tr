/// @file
/// @brief Provides `tr::unique_handle`.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// @brief Default handle deleter object.
	/// @details Invoking a `tr::default_handle_deleter` is a no-op.
	template <typename Handle>
	struct default_handle_deleter
	{
		/// @brief Deletes the handle.
		/// @details This function is a no-op.
		constexpr void operator()(Handle) noexcept {}
	};

	/// @ingroup handle
	/// @brief `std::unique_ptr`-like object owning and managing an object passed via an integral handle.
	/// @details `tr::unique_handle` is designed to be used as the base layer on top of which a more specialized wrapper is built. It
	/// provides the proper move and RAII semantics, but has no support for other methods.
	/// @tparam Handle Handle representation type.
	/// @tparam Empty Sentinel value set aside as the representation for an empty value.
	/// @tparam Deleter Deleter object invoked when destroying a managed object. Must be nothrow constructible and invocable.
	template <typename Handle, Handle Empty, typename Deleter = default_handle_deleter<Handle>>
		requires std::is_nothrow_invocable_v<Deleter, Handle> && (std::integral<Handle> || std::is_enum_v<Handle>)
	class unique_handle : private Deleter
	{
	  public:
		/// @brief Handle representation type.
		using handle_type = Handle;

		/// @brief Sentinel value set aside as the representation for an empty value.
		static constexpr Handle empty_sentinel{Empty};

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty unique handle.
		[[nodiscard]] constexpr unique_handle() noexcept
			requires std::is_nothrow_default_constructible_v<Deleter>
			: Deleter{}
			, m_handle{Empty}
		{
		}

		/// @brief Constructs an empty unique handle.
		/// @param deleter Deleter to copy to the unique handle.
		[[nodiscard]] constexpr unique_handle(const Deleter& deleter) noexcept
			requires std::is_nothrow_copy_constructible_v<Deleter>
			: Deleter{deleter}
			, m_handle{Empty}
		{
		}

		/// @brief Constructs an empty unique handle.
		/// @param deleter Deleter to move into the unique handle.
		[[nodiscard]] constexpr unique_handle(Deleter&& deleter) noexcept
			requires std::is_nothrow_move_constructible_v<Deleter>
			: Deleter{std::move(deleter)}
			, m_handle{Empty}
		{
		}

		/// @brief Takes ownership over the object passed via `handle`.
		/// @param handle Handle to the object to take ownership of, or `Empty` to construct an empty unique handle.
		[[nodiscard]] constexpr explicit unique_handle(Handle handle) noexcept
			requires std::is_nothrow_default_constructible_v<Deleter>
			: Deleter{}
			, m_handle{handle}
		{
		}

		/// @brief Takes ownership over the object passed via `handle`.
		/// @param handle Handle to the object to take ownership of, or `Empty` to construct an empty unique handle.
		/// @param deleter Deleter to copy to the unique handle.
		[[nodiscard]] constexpr explicit unique_handle(Handle handle, const Deleter& deleter) noexcept
			requires std::is_nothrow_copy_constructible_v<Deleter>
			: Deleter{deleter}
			, m_handle{handle}
		{
		}

		/// @brief Takes ownership over the object passed via `handle`.
		/// @param handle Handle to the object to take ownership of, or `Empty` to construct an empty `tr:unique_handle`.
		/// @param deleter Deleter to move into the unique handle.
		[[nodiscard]] constexpr explicit unique_handle(Handle handle, Deleter&& deleter) noexcept
			requires std::is_nothrow_move_constructible_v<Deleter>
			: Deleter{std::move(deleter)}
			, m_handle{handle}
		{
		}

		/// @brief Transfers ownership over an object from `rhs` (if any) and moves its deleter.
		/// @param rhs Unique handle to transfer object ownership and move the deleter from.
		/// @post `rhs` is left empty after the move, while the state of its deleter is type-specific.
		[[nodiscard]] constexpr unique_handle(unique_handle&& rhs) noexcept
			requires std::move_constructible<Deleter>
			: Deleter{std::move(rhs)}
			, m_handle{std::exchange(rhs.m_handle, Empty)}
		{
		}

		/// @brief Destroys the managed object (if any), transfers ownership over the object (if any) from `rhs`, and moves its deleter.
		/// @param rhs Unique handle to transfer object ownership and move the deleter from.
		/// @post `rhs` is left empty after the move, while the state of its deleter is type-specific.
		/// @return Reference to `*this`.
		constexpr unique_handle& operator=(unique_handle&& rhs) noexcept
			requires std::is_nothrow_move_assignable_v<Deleter>
		{
			if (m_handle != Empty) {
				Deleter::operator()(m_handle);
			}
			m_handle = std::exchange(rhs.m_handle, Empty);
			get_deleter() = std::move(rhs.get_deleter());
			return *this;
		}

		/// @brief Destroys the handled object if the unique handle is not empty.
		constexpr ~unique_handle() noexcept
		{
			if (m_handle != Empty) {
				Deleter::operator()(m_handle);
			}
		}

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Releases ownership over the managed object (if any).
		/// @post `*this` is left empty after the release.
		/// @return Handle to the formerly managed object, or `Empty` if there was none.
		[[nodiscard]] constexpr Handle release() noexcept
		{
			return std::exchange(m_handle, Empty);
		}

		/// @brief Replaces the managed object.
		/// @details The currently managed object (if there is one) is destroyed.
		/// @param handle Handle to the object to take ownership of, or `Empty` to empty the unique handle.
		constexpr void reset(Handle handle = Empty) noexcept
		{
			if (m_handle != Empty) {
				Deleter::operator()(m_handle);
			}
			m_handle = handle;
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether `*this` owns an object, i.e. whether `get() != Empty`.
		/// @return `true` if `*this` owns an object, `false` otherwise.
		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return m_handle != Empty;
		}

		/// @brief Returns a handle to the managed object or `Empty` if no object is owned.
		/// @return Handle to the managed object or `Empty` if no object is owned.
		[[nodiscard]] constexpr Handle get() const noexcept
		{
			return m_handle;
		}

		/// @brief Returns a reference to the deleter that would be used to destroy managed objects.
		/// @return Reference to the deleter that would be used to destroy managed objects.
		[[nodiscard]] constexpr Deleter& get_deleter() noexcept
		{
			return *this;
		}

		/// @brief Returns a reference to the deleter that would be used to destroy managed objects.
		/// @return Reference to the deleter that would be used to destroy managed objects.
		[[nodiscard]] constexpr const Deleter& get_deleter() const noexcept
		{
			return *this;
		}

		/// @}

	  private:
		/// @brief Handle to the managed object.
		Handle m_handle;
	};
} // namespace tr

//

/// @ingroup handle
/// @brief Unique handle formatter.
template <typename Handle, Handle Empty, typename Deleter, typename CharT>
class std::formatter<tr::unique_handle<Handle, Empty, Deleter>, CharT> : private std::formatter<Handle, CharT>
{
	using std::formatter<Handle, CharT>::parse;

	/// @brief Formats a unique handle.
	/// @tparam FormatContext Formatting context type.
	/// @param handle Unique handle to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::unique_handle<Handle, Empty, Deleter>& handle, FormatContext& context) const
	{
		return std::formatter<Handle, CharT>::format(handle.get(), context);
	}
};

#ifndef TR_DOXYGEN
/// @ingroup handle
/// @brief Unique handle hasher.
template <typename Handle, Handle Empty, typename Deleter>
struct boost::hash<tr::unique_handle<Handle, Empty, Deleter>>
{
	/// @brief Hashes a unique handle.
	/// @param handle Unique handle to hash.
	/// @return `boost::hash<Handle>{}(handle.get())`.
	constexpr tr::usize operator()(const tr::unique_handle<Handle, Empty, Deleter>& handle) const noexcept
	{
		return boost::hash<Handle>{}(handle.get());
	}
};
#endif