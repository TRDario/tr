/// @file
/// @brief Provides `tr::ogg_audio_stream`.

#pragma once
#include <tr/audio/audio_stream.hpp>
#include <vorbis/vorbisfile.h>

//

namespace tr
{
	/// Ogg audio file backend.
	class ogg_audio_stream final : public audio_stream
	{
	  public:
		/// @name Constructors
		/// @{

		/// Loads an Ogg stream from file.
		/// @param path Path to the Ogg file.
		[[nodiscard]] ogg_audio_stream(const std::filesystem::path& path);

		/// Closes the Ogg stream.
		~ogg_audio_stream() noexcept override;

		/// @}
		/// @name Information
		/// @{

		[[nodiscard]] usize length() const override;

		[[nodiscard]] int channels() const override;

		[[nodiscard]] int sample_rate() const override;

		/// @}
		/// @name IO
		/// @{

		[[nodiscard]] usize tell() const override;

		void seek(usize where) override;

		/// @}

	  protected:
		void raw_read(std::span<i16> buffer) override;

	  private:
		/// Handle to the Ogg file.
		mutable OggVorbis_File m_file{};
	};
} // namespace tr