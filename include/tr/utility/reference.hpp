#pragma once

namespace tr {
	// Alternative reference wrapper.
	template <class T> class ref {
	  public:
		constexpr ref(T& ref);
		constexpr ref(T&&) = delete;

		constexpr operator T&() const;
		constexpr operator ref<const T>() const;

		constexpr bool operator==(const ref&) const = default;

		constexpr T* as_ptr() const;
		constexpr T* operator->() const;
		constexpr T& operator*() const;

	  private:
		T* m_base;
	};
	// Wrapper over a pointer representing optional reference semantics.
	template <class T> class opt_ref {
	  public:
		constexpr opt_ref() = default;
		constexpr opt_ref(std::nullopt_t);
		constexpr opt_ref(T& ref);
		constexpr opt_ref(T&&) = delete;

		constexpr operator opt_ref<const T>() const;

		constexpr bool operator==(const opt_ref&) const = default;

		constexpr explicit operator bool() const;
		constexpr bool has_value() const;

		constexpr T* as_ptr() const;
		constexpr T* operator->() const;
		constexpr T& operator*() const;

	  private:
		T* m_base{nullptr};

		constexpr explicit opt_ref(T* ptr);
		friend opt_ref<T> make_opt_ref(T* ptr);
	};
	// Converts a pointer into an optional reference.
	template <class T> opt_ref<T> make_opt_ref(T* ptr);
} // namespace tr

#include "reference_impl.hpp"
