///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides reference types.                                                                                                             //
//                                                                                                                                       //
// tr::ref is provided as an alternative to std::reference_wrapper, being both shorter to type, as well as being dereferencable with     //
// operator* and operator->                                                                                                              //
//     - tr::ref<int> ref{x}; *ref -> accesses x                                                                                         //
//     - tr::ref<std::pair<int, int>> ref{pair}; ref->first -> accesses pair.first                                                       //
//                                                                                                                                       //
// tr::ref<T> is implicitly convertible to/from T& as well as convertible to tr::ref<const T>. It can also be turned into a pointer      //
// using the .as_ptr() method. References compare true if they point to the same object:                                                 //
//     - tr::ref<int> xr{x}; int y; y = xr -> works, xr converted to int&                                                                //
//     - tr::ref<int> xr{x}; tr::ref<const int> cxr{xr}; -> works, xr const-qualified                                                    //
//     - int x; tr::ref xr{x}; xr.as_ptr() -> &x                                                                                         //
//     - int x = 0; int y = 0; tr::ref xr{x}; tr::ref xr2{x}; tr::ref yr{y}; xr == yr; xr == xr2 -> false; true                          //
//                                                                                                                                       //
// tr::opt_ref is provided as an alternative to using raw observer pointers to hold an optional reference to an object. It can be        //
// constructed from a reference or std::nullopt, as well as default-constructed (equivalent to constructing with std::nullopt).          //
// A pointer can be directly converted to an optional reference using tr::make_opt_ref:                                                  //
//     - tr::opt_ref<int> ref; -> empty optional reference                                                                               //
//     - tr::opt_ref<int> ref{std::nullopt}; -> empty optional reference                                                                 //
//     - tr::opt_ref<int> ref{x} -> reference to x                                                                                       //
//     - tr::make_opt_ref(ptr) -> reference to *ptr if non-NULL, empty optional reference otherwise                                      //
//                                                                                                                                       //
// Much like tr::ref, tr::opt_ref can be dereferenced with operator* and operator->, as well as turned into a pointer using the          //
// .as_ptr() method (returns nullptr if empty). The optional reference can be checked for holding a reference using the .has_ref()       //
// method. Optional references compare true if they point to the same object or are both empty:                                          //
//     - tr::opt_ref<int> oref{x}; *oref -> accesses x                                                                                   //
//     - tr::opt_ref<std::pair<int, int>> oref{pair}; oref->first -> accesses pair.first                                                 //
//     - tr::opt_ref<int> oref{x}; oref.as_ptr() -> &x                                                                                   //
//     - tr::opt_ref<int> oref; oref.as_ptr() -> nullptr                                                                                 //
//     - int x{0}; int y{0}; tr::opt_ref xr{x}; tr::opt_ref yr{y}; xr == yr -> false                                                     //
//     - tr::opt_ref o1; tr::opt_ref o2; o1 == o2 -> true                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Alternative reference wrapper.
	template <class T> class ref {
	  public:
		// Wraps a reference.
		constexpr ref(T& ref);
		// Wrapping an rvalue is not permitted.
		constexpr ref(T&&) = delete;

		// Unwraps the reference.
		constexpr operator T&() const;
		// Const-qualifies the reference.
		constexpr operator ref<const T>() const;

		// Returns whether two references point to the same object.
		constexpr friend bool operator==(const ref&, const ref&) = default;

		// Converts the reference into a pointer to the referenced object.
		constexpr T* as_ptr() const;
		// Dereferences the referenced object.
		constexpr T* operator->() const;
		// Dereferences the referenced object.
		constexpr T& operator*() const;

	  private:
		// Pointer to the referenced object.
		T* m_base;
	};
	template <class T> constexpr bool operator==(const T& l, const ref<T>& r);
	template <class T> constexpr bool operator==(const ref<T>& l, const T& r);

	// Wrapper over a pointer representing optional reference semantics.
	template <class T> class opt_ref {
	  public:
		// Creates an empty optional reference.
		constexpr opt_ref() = default;
		// Creates an empty optional reference.
		constexpr opt_ref(std::nullopt_t);
		// Wraps a reference.
		constexpr opt_ref(T& ref);
		// Wrapping an rvalue is not permitted.
		constexpr opt_ref(T&&) = delete;

		// Const-qualifies the optional reference.
		constexpr operator opt_ref<const T>() const;

		// Returns whether two optional references point to the same object (or both are empty).
		constexpr friend bool operator==(const opt_ref&, const opt_ref&) = default;

		// Returns whether the optional reference holds a reference to an object.
		constexpr bool has_ref() const;

		// Converts the optional reference into a pointer to the referenced object, or nullptr.
		constexpr T* as_ptr() const;
		// Dereferences the referenced object.
		constexpr T* operator->() const;
		// Dereferences the referenced object.
		constexpr T& operator*() const;

	  private:
		// Pointer to the referenced object, or nullptr.
		T* m_base{nullptr};

		// Wraps a pointer.
		constexpr explicit opt_ref(T* ptr);

		template <class U> friend opt_ref<U> make_opt_ref(U* ptr);
	};
	// Converts a pointer into an optional reference.
	template <class T> opt_ref<T> make_opt_ref(T* ptr);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "reference_impl.hpp" // IWYU pragma: export