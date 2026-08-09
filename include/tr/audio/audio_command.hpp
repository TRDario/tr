/// @file
/// @brief Provides an audio command class.

#pragma once
#include "../utility/angle.hpp"
#include "../utility/chrono.hpp"
#include "../utility/reference.hpp"

namespace tr
{
	class audio_source;
}

//

namespace tr
{
	/// Audio command template.
	/// @tparam First, Rest Types of the arguments to the command.
	template <typename First, typename... Rest>
	class audio_command
	{
	  public:
		/// Method signature used by the command.
		using method_type = void (audio_source::*)(First, Rest...);

		/// command<float>::value_type = float, command<float, float>::value_type = std::tuple<float, float>.
		using value_type = std::conditional_t<sizeof...(Rest), std::tuple<First, Rest...>, First>;

		/// Audio command status returned after execution.
		enum class status : bool
		{
			/// The command is still ongoing.
			ongoing,
			/// The command is done.
			done
		};

		/// @name Constructors
		/// @{

		/// Constructs an audio command.
		/// @param source Audio source to command.
		/// @param method Method the command calls when setting the value.
		/// @param begin Initial value of the property being set.
		/// @param end Final value of the property being set.
		/// @param length Length of the command.
		audio_command(audio_source& source, method_type method, const value_type& begin, const value_type& end, fsecs length);

		/// @}
		/// @name Source
		/// @{

		/// Gets the source being commanded.
		/// @return Reference to the source being commanded.
		audio_source& source() const;

		/// @}
		/// @name Execution
		/// @{

		/// Executes the command.
		/// @return Status of the command after execution.
		status execute();

		/// @}

	  private:
		/// Source being commanded.
		tr::ref<audio_source> m_source;

		/// Method the command calls when setting the value.
		method_type m_method;

		/// Initial value of the property being set.
		value_type m_begin;

		/// Final value of the property being set.
		value_type m_end;

		/// Length of the command.
		duration m_length;

		/// When the last update was.
		std::chrono::steady_clock::time_point m_last_update;

		/// How much time has elapsed for the command.
		duration m_elapsed;
	};

	/// Generic audio command.
	using generic_audio_command = std::variant<audio_command<float>, audio_command<angle, angle>, audio_command<glm::vec3>>;
} // namespace tr

#include "impl/audio_command.hpp" // IWYU pragma: export