#pragma once

namespace tr {
	// Overloaded lambda helper class.
	template <class... Fs> struct overloaded : Fs... {
		using Fs::operator()...;
	};
	// Deduction guide.
	template <class... Fs> overloaded(Fs...) -> overloaded<Fs...>;
} // namespace tr