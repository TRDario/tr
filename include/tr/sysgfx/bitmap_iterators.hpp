///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides bitmap iterators.                                                                                                            //
//                                                                                                                                       //
// All bitmap iterators iterate through the bitmap left-to-right, top-to-bottom. They fulfill the requirements of random-access          //
// iterators, but not contiguous iterators due to bitmaps not being guaranteed to be contiguous. As an extension to the iterator         //
// interface, bitmap iterators can report their 2D position within the bitmap using the .pos() method:                                   //
//     - tr::bitmap bmp{{100, 100}}; (bmp.begin() + 2413).pos() -> {13, 24}                                                              //
//                                                                                                                                       //
// The iterators return proxy objects representing pixels. These proxies may be converted to tr::rgba8, and mutable proxies may set the  //
// pixel to the closest equivalent of an tr::rgba8 color:                                                                                //
//     - tr::rgba8 color{*it} -> gets a pixel's color                                                                                    //
//     - *it = "FF0000"_rgba8 -> sets a pixel's color                                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/iterator.hpp"
#include "../utility/reference.hpp"
#include "bitmap.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Read-only reference to a bitmap pixel.
	class sub_bitmap::reference {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const;

	  private:
		// A pointer to the pixel data.
		const std::byte* m_ptr;
		// The format of the pixel.
		pixel_format m_format;

		// Undefined, required to default-construct iterators.
		reference() = default;
		// Wraps a pointer to the pixel data.
		reference(const std::byte* ptr, pixel_format format);

		friend class sub_bitmap;
		friend class iterator;
	};

	// Immutable bitmap pixel iterator.
	class sub_bitmap::iterator : public iterator_interface<iterator, glm::ivec2> {
	  public:
		using value_type = reference;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		iterator() = default;
		// Constructs an iterator to a bitmap pixel.
		iterator(const sub_bitmap& bitmap, glm::ivec2 pos);

		// Compares iterators.
		std::partial_ordering operator<=>(const iterator&) const;
		// Compares iterators for equality.
		bool operator==(const iterator&) const;

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator through a pointer.
		pointer operator->() const;

		// Increments the iterator.
		iterator& operator++();
		// Advances the iterator.
		iterator& operator+=(difference_type diff);
		// Advances an iterator.
		iterator& operator+=(glm::ivec2 diff);

		// Decrements the iterator.
		iterator& operator--();
		// Gets the distance between two iterators.
		friend difference_type operator-(const iterator& lhs, const iterator& rhs);

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		reference m_pixel;
		// The size of the pointed-to bitmap.
		glm::ivec2 m_bitmap_size;
		// The pitch of the pointed-to-bitmap.
		int m_bitmap_pitch;
		// The position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};

	// Mutable reference to a bitmap pixel.
	class bitmap::reference {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const;
		// Sets the pixel's color value.
		reference& operator=(rgba8 color);

	  private:
		// A pointer to the pixel data.
		std::byte* m_ptr;
		// The format of the pixel.
		pixel_format m_format;

		// Undefined, required to default-construct iterators.
		reference() = default;
		// Wraps a pointer to the pixel data.
		reference(std::byte* ptr, pixel_format format);

		friend class iterator;
	};

	// Mutable pixel iterator.
	class bitmap::iterator : public iterator_interface<iterator, glm::ivec2> {
	  public:
		using value_type = reference;
		using reference = reference;
		using const_reference = const reference;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		iterator() = default;
		// Constructs an iterator to a bitmap pixel.
		iterator(bitmap& bitmap, glm::ivec2 pos);

		// Compares iterators.
		std::partial_ordering operator<=>(const iterator&) const;
		// Compares iterators for equality.
		bool operator==(const iterator&) const;

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator through a pointer.
		pointer operator->() const;

		// Increments the iterator.
		iterator& operator++();
		// Advances the iterator.
		iterator& operator+=(difference_type diff);
		// Advances the iterator.
		iterator& operator+=(glm::ivec2 diff);

		// Decrements the iterator.
		iterator& operator--();
		// Gets the distance between two iterators.
		friend difference_type operator-(const iterator& lhs, const iterator& rhs);

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		reference m_pixel;
		// The pointed-to-bitmap.
		opt_ref<bitmap> m_bitmap;
		// The position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};
} // namespace tr