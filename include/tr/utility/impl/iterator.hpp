///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements iterator.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../concepts.hpp"
#include "../iterator.hpp"

//////////////////////////////////////////////////////////// ITERATOR INTERFACE ///////////////////////////////////////////////////////////

namespace tr {
	// Iterator interface definition.
	template <typename Iterator, typename DefaultBracketedDifference> class iterator_interface {
	  private:
		// Gets a reference to the base type.
		constexpr Iterator& base()
		{
			return static_cast<Iterator&>(*this);
		}

		// Gets a reference to the base type.
		constexpr const Iterator& base() const
		{
			return static_cast<const Iterator&>(*this);
		}

	  public:
		friend constexpr auto operator<=>(const iterator_interface& lhs, const iterator_interface& rhs) = default;
		friend constexpr bool operator==(const iterator_interface& lhs, const iterator_interface& rhs) = default;

		// Post-increment operator.
		friend constexpr Iterator operator++(iterator_interface& it, int)
			requires(requires { ++it.base(); })
		{
			Iterator copy{it.base()};
			++it.base();
			return copy;
		}

		// Post-decrement operator.
		friend constexpr Iterator operator--(iterator_interface& it, int)
			requires(requires { --it.base(); })
		{
			Iterator copy{it.base()};
			--it.base();
			return copy;
		}

		// Subtraction assignment operator.
		friend constexpr Iterator& operator-=(iterator_interface& it, auto diff)
			requires(requires { it.base() += -diff; })
		{
			return it.base() += -diff;
		}

		// Addition operator.
		friend constexpr Iterator operator+(const iterator_interface& it, auto diff)
			requires(requires(Iterator i) { i += diff; })
		{
			Iterator copy{it.base()};
			copy += diff;
			return copy;
		}

		// Addition operator.
		friend constexpr Iterator operator+(auto diff, const iterator_interface& it)
			requires(requires(Iterator i) { i += diff; })
		{
			return it + diff;
		}

		// Subtraction operator.
		friend constexpr Iterator operator-(const iterator_interface& it, auto diff)
			requires(requires(Iterator i) { i -= diff; })
		{
			Iterator copy{it.base()};
			copy -= diff;
			return copy;
		}

		//

		// Dereference operator.
		constexpr auto operator->() const
			requires(requires {
				{ *base() } -> lvalue_reference;
			})
		{
			return &*base();
		}

		// Array access operator.
		template <typename Difference = DefaultBracketedDifference>
		constexpr decltype(auto) operator[](Difference diff) const
			requires(requires(Iterator it) { it + diff; })
		{
			return *(base() + diff);
		}
	};
} // namespace tr