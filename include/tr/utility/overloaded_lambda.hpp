#pragma once

namespace tr {
	/** @ingroup utility
	 *  @defgroup overloaded_lambda Overloaded
	 *  Overloaded lambda helper class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Overloaded lambda helper class.
	 *
	 * @tparam Fs The lambdas/functors to inheret from.
	 ******************************************************************************************************************/
	template <class... Fs> struct overloaded : Fs... {
		using Fs::operator()...;
	};

	/******************************************************************************************************************
	 * Deduction guide.
	 ******************************************************************************************************************/
	template <class... Fs> overloaded(Fs...) -> overloaded<Fs...>;

	/// @}
} // namespace tr