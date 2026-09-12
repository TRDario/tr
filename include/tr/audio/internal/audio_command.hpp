/// @file
/// @brief Provides `tr::audio_command`.

#pragma once
#include <tr/utility/angle.hpp>
#include <tr/utility/chrono.hpp>
#include <tr/utility/reference.hpp>

namespace tr
{
	class audio_source;
}

//

namespace tr::internal
{
	/// Linearly interpolates a set of values memberwise.
	/// @tparam Ts Value types.
	/// @tparam Is Value indices.
	/// @param begin, end Sample values.
	/// @param ratio Interpolation factor.
	/// @return `begin + ratio * (end − begin)`.
	template <typename... Ts, usize... Is>
	[[nodiscard]] std::tuple<Ts...> lerp_tuple(const std::tuple<Ts...>& begin, const std::tuple<Ts...>& end, float ratio,
											   std::index_sequence<Is...>) noexcept
	{
		return {tr::lerp(std::get<Is>(begin), std::get<Is>(end), ratio)...};
	}

	/// Audio command template.
	/// @tparam First, Rest Types of the arguments to the command.
	template <typename First, typename... Rest>
	class audio_command
	{
	  public:
		/// Method signature used by the command.
		using method_type = void (audio_source::*)(First, Rest...) noexcept;

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
		[[nodiscard]] audio_command(audio_source& source, method_type method, const value_type& begin, const value_type& end,
									fsecs length) noexcept
			: m_source{source}
			, m_method{method}
			, m_begin{begin}
			, m_end{end}
			, m_length{std::chrono::duration_cast<duration>(length)}
			, m_last_update{std::chrono::steady_clock::now()}
			, m_elapsed{}
		{
		}

		/// @}
		/// @name Source
		/// @{

		/// Gets the source being commanded.
		/// @return Reference to the source being commanded.
		[[nodiscard]] audio_source& source() const noexcept
		{
			return m_source;
		}

		/// @}
		/// @name Execution
		/// @{

		/// Executes the command.
		/// @return Status of the command after execution.
		[[nodiscard]] status execute() noexcept
		{
			const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
			m_elapsed = std::min(m_elapsed + now - m_last_update, m_length);
			m_last_update = now;

			const float t{ratio(m_elapsed, m_length)};
			if constexpr (sizeof...(Rest) > 0) {
				std::apply([this](First first, Rest... rest) { ((*m_source).*m_method)(first, rest...); },
						   lerp_tuple(m_begin, m_end, t, std::index_sequence_for<First, Rest...>{}));
			}
			else {
				((*m_source).*m_method)(lerp(m_begin, m_end, t));
			}

			return (m_elapsed == m_length) ? status::done : status::ongoing;
		}

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
} // namespace tr::internal