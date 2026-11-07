/// @file
/// @brief Provides `tr::internal::ogg_audio_stream`.

#pragma once
#include <tr/audio/audio_stream.hpp>
#include <vorbis/vorbisfile.h>

//

namespace tr::internal
{
	/// @brief Ogg audio file backend.
	class ogg_audio_stream final : public audio_stream
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Loads an Ogg stream from file.
		/// @param path Path to the Ogg file.
		/// @exception audio_file_open_error If opening the Ogg file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] ogg_audio_stream(const std::filesystem::path& path);

		/// @brief Closes the Ogg stream.
		~ogg_audio_stream() noexcept override;

		/// @}
		/// @name Observers
		/// @{

		[[nodiscard]] usize length() const override;

		[[nodiscard]] int channels() const override;

		[[nodiscard]] int sample_rate() const override;

		/// @}
		/// @name Operations
		/// @{

		[[nodiscard]] usize tell() const override;

		void seek(usize where) override;

		/// @}

	  protected:
		void raw_read(std::span<i16> buffer) override;

	  private:
		/// @brief Handle to the Ogg file.
		mutable OggVorbis_File m_file{};
	};
} // namespace tr::internal