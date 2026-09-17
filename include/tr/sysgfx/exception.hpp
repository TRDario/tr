/// @file
/// @brief Provides exceptions thrown by the system and graphics module.

#pragma once
#include <tr/utility/exception.hpp>

//

namespace tr
{
	/// Bitmap loading error.
	class bitmap_load_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the bitmap file.
		/// @param details Details of the error.
		[[nodiscard]] bitmap_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Bitmap loading error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	/// Bitmap saving error.
	class bitmap_save_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the bitmap file.
		/// @param details Details of the error.
		[[nodiscard]] bitmap_save_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Bitmap saving error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	/// Cursor operation error.
	class cursor_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a cursor error.
		/// @param description Description of the error.
		[[nodiscard]] cursor_error(std::string_view description) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Cursor error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string_view m_description;

		/// Details of the error.
		std::string_view m_details;
	};

	/// Graphics context initialization error.
	class graphics_context_init_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a graphics context initialization error.
		[[nodiscard]] graphics_context_init_error();

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Graphics context opening error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;
	};

	/// Error thrown when getting a path failed.
	class path_error : public exception
	{
	  public:
		/// Creates a path error.
		/// @param description Description of the error.
		[[nodiscard]] path_error(std::string_view description) noexcept;

		//

		/// Gets the name of the error.
		/// @return `"Path error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

	  private:
		/// Description of the error.
		std::string_view m_description;
	};

	/// Clipboard setting error.
	class set_clipboard_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a clipboard setting error.
		[[nodiscard]] set_clipboard_error() noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Clipboard setting error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string_view m_description;
	};

	/// Shader loading error.
	class shader_load_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the file that failed to load.
		/// @param details Shader loading error details.
		[[nodiscard]] shader_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Shader loading error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	/// Error thrown when font loading fails.
	class ttfont_load_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the file that failed to load.
		/// @param details Details of the error.
		[[nodiscard]] ttfont_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"TrueType font loading error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	/// Error thrown when font bitmap rendering fails.
	class ttfont_render_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an exception.
		/// @param description Description of the error.
		[[nodiscard]] ttfont_render_error(std::string_view description) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"TrueType font rendering error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		// Description of the error.
		std::string_view m_description;
	};

	/// Error thrown when font manipulation fails.
	class ttfont_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a window error.
		/// @param description Description of the error.
		[[nodiscard]] ttfont_error(std::string&& description) noexcept;

		/// Constructs a font error.
		/// @tparam Args Types of the formatting arguments.
		/// @param description_fmt Error description format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		[[nodiscard]] ttfont_error(std::format_string<Args...> description_fmt, Args&&... args)
			: ttfont_error{std::format(description_fmt, std::forward<Args>(args)...)}
		{
		}

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"TrueType font error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string_view m_details;
	};

	/// Window error.
	class window_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a window error.
		/// @param description Description of the error.
		[[nodiscard]] window_error(std::string&& description) noexcept;

		/// Constructs a window error.
		/// @tparam Args Types of the formatting arguments.
		/// @param description_fmt Description format string.
		/// @param args Description formatting arguments.
		template <typename... Args>
		[[nodiscard]] explicit window_error(std::format_string<Args...> description_fmt, Args&&... args) noexcept
			: window_error{std::format(description_fmt, std::forward<Args>(args)...)}
		{
		}

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Window error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string_view m_details;
	};

	/// Window opening error.
	class window_open_error : public exception
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a window opening error.
		[[nodiscard]] explicit window_open_error() noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Window opening error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;
	};
} // namespace tr