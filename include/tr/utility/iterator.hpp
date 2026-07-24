///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides iterator utilities.                                                                                                          //
//                                                                                                                                       //
// tr::iterator_interface<Iterator> is a CRTP base class that provides automatic derivation of various iterator functions from just a    //
// small subset of basic operators (operator++, operator--, operator+=, operator*).                                                      //
// The second parameter in the template, 'DefaultBracketedDifference', may be overriden to allow automatic deduction of bracketed values //
// in operator[].                                                                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Interface an iterator class can inherit from to automatically derive various functions from a small subset of basic operators
	// (operator++, operator--, operator+=, operator*).
	template <typename Iterator, typename DefaultBracketedDifference = void> class iterator_interface;
} // namespace tr

#include "impl/iterator.hpp" // IWYU pragma: export