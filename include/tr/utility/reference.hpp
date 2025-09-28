#pragma once

namespace tr {
	// Alternative reference wrapper.
	template <class T> class ref {
	  public:
		constexpr ref(T& ref);
		constexpr ref(T&&) = delete;

		constexpr operator T&() const;
		constexpr operator ref<const T>() const;

		constexpr friend bool operator==(const ref&, const ref&) = default;

		constexpr T* as_ptr() const;
		constexpr T* operator->() const;
		constexpr T& operator*() const;

	  private:
		T* m_base;
	};
	template <class T> constexpr bool operator==(const T& l, const ref<T>& r);
	template <class T> constexpr bool operator==(const ref<T>& l, const T& r);

	// Wrapper over a pointer representing optional reference semantics.
	template <class T> class opt_ref {
	  public:
		constexpr opt_ref() = default;
		constexpr opt_ref(std::nullopt_t);
		constexpr opt_ref(T& ref);
		constexpr opt_ref(T&&) = delete;

		constexpr operator opt_ref<const T>() const;

		constexpr friend bool operator==(const opt_ref&, const opt_ref&) = default;

		constexpr bool has_ref() const;

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

#include "reference_impl.hpp" // IWYU pragma: keep