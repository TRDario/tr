/// @file
/// @brief Provides `tr::framebuffer`.

#pragma once
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	class graphics_context;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// @brief Owning container for a framebuffer object.
	/// @details
	/// A framebuffer is a collection of references to buffers used as a destination for rendering. The default framebuffer is used to
	/// render to the screen, while instances of `tr::framebuffer` may have arbitrary textures attached to them as rendering targets.
	///
	/// It's worth remembering that instances of `tr::framebuffer` are only containers for these underlying framebuffers. This means, for
	/// example, that setting a render target on a graphics context does not set a render target of a literal `tr::framebuffer` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of `tr::framebuffer`,
	/// that value will still be set on the context. If the instance is overriden with a new value, the old value is destroyed and the
	/// graphics context will no longer have a set render target.
	///
	/// Every instance of `tr::framebuffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::framebuffer` are left in a special 'invalid' state. Invalid `tr::framebuffer` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// `tr::framebuffer` instances may be labeled and are formattable. Example format output: `"My %framebuffer" (ID: 3, FBO: 5)`.
	class framebuffer
	{
	  public:
		/// @brief Framebuffer attachment points.
		enum class attachment
		{
			/// @brief First color attachment.
			/// @hideinitializer
			color0 = 36064,

			/// @brief Second color attachment.
			color1,

			/// @brief Third color attachment.
			color2,

			/// @brief Fourth color attachment.
			color3,

			/// @brief Fifth color attachment.
			color4,

			/// @brief Sixth color attachment.
			color5,

			/// @brief Seventh color attachment.
			color6,

			/// @brief Eighth color attachment.
			color7,

			/// @brief Depth attachment.
			/// @hideinitializer
			depth = 36096,

			/// @brief Stencil attachment.
			/// @hideinitializer
			stencil = 36128,
		};

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty framebuffer.
		/// @param context Graphics context to create the framebuffer on.
		/// @param label Label of the framebuffer.
		[[nodiscard]] explicit framebuffer(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Framebuffers are not copyable.
		framebuffer(const framebuffer&) = delete;

		/// @brief Framebuffers are not copyable.
		framebuffer& operator=(const framebuffer&) = delete;

		/// @brief Constructs a framebuffer by transferring ownership from another `tr::framebuffer`.
		/// @param rhs Framebuffer to move from.
		/// @par Invalidation
		/// If the framebuffer previously held by `rhs` is set on the associated graphics context, it stays set. Any render targets pointing
		/// to the framebuffer now point to the framebuffer stored in `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] framebuffer(framebuffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held framebuffer and replaces it with one moved from another `tr::framebuffer`.
		/// @param rhs Framebuffer to move from.
		/// @par Invalidation
		/// If the framebuffer previously held by `*this` was set on the associated graphics context, it is unset. Any render targets
		/// pointing to the framebuffer are invalidated.
		///
		/// If the framebuffer previously held by `rhs` is set on the associated graphics context, it stays set. Any render targets pointing
		/// to the framebuffer now point to the framebuffer stored in `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		framebuffer& operator=(framebuffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Attaches a texture to the framebuffer.
		/// @param attachment Attachment point to set the texture on.
		/// @param texture Texture to attach to the framebuffer.
		void attach(attachment attachment, texture_view texture) noexcept;

		/// @brief Detaches whatever is on the specified attachment point.
		/// @param attachment Attachment point to clear.
		void detach(attachment attachment) noexcept;

		/// @brief Sets the label of the framebuffer.
		/// @param label Label of the framebuffer.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the framebuffer is in a valid state.
		/// @return `true` if the framebuffer is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the framebuffer is in a valid state.
		/// @return `true` if the framebuffer is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the framebuffer is on.
		/// @pre The framebuffer must be in a valid state.
		/// @return Reference to the graphics context the framebuffer is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Returns the label of the framebuffer.
		/// @pre The framebuffer must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the framebuffer, or "<unnamed>".
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL framebuffer object.
		/// @return Handle to the underlying OpenGL framebuffer object.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @cond implementation_details
		/// @brief Returns ID uniquely identifying the framebuffer.
		/// @return ID uniquely identifying the framebuffer.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL framebuffer object deleter.
		struct gl_framebuffer_object_deleter
		{
			/// @brief Reference to the graphics context the framebuffer is on.
			ref<graphics_context> context;

			/// @brief ID uniquely identifying the framebuffer.
			unique_handle<internal::graphics_object_id, internal::graphics_object_id::invalid> id{internal::generate_graphics_object_id()};

			//

			/// @brief Destroys a framebuffer object.
			/// @param handle OpenGL framebuffer object handle.
			void operator()(unsigned int handle) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL framebuffer object.
		unique_handle<unsigned int, 0, gl_framebuffer_object_deleter> m_handle;
	};
} // namespace tr

//

/// @brief Framebuffer formatter.
template <>
struct std::formatter<tr::framebuffer>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid framebuffer format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a framebuffer.
	/// @tparam FormatContext Formatting context type.
	/// @param framebuffer Framebuffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::framebuffer& framebuffer, FormatContext& context) const
	{
		if (framebuffer) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, FBO: {})", framebuffer.label(), std::to_underlying(framebuffer.id()),
								  framebuffer.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid framebuffer at {}>", static_cast<const void*>(&framebuffer));
		}
	}
};