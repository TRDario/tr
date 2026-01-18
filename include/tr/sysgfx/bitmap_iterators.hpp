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
#include "bitmap.hpp"

namespace tr {
	// Read-only reference to a bitmap pixel.
	class sub_bitmap::pixel_ref {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const;

	  private:
		// A pointer to the pixel data.
		const std::byte* m_ptr;
		// The format of the pixel.
		pixel_format m_format;

		// Undefined, required to default-construct iterators.
		pixel_ref() = default;
		// Wraps a pointer to the pixel data.
		pixel_ref(const std::byte* ptr, pixel_format format);

		friend class sub_bitmap;
		friend class iterator;
	};

	// Immutable bitmap pixel iterator.
	class sub_bitmap::iterator {
	  public:
		using value_type = pixel_ref;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		iterator() = default;
		// Constructs an iterator to a bitmap pixel.
		iterator(const sub_bitmap& bitmap, glm::ivec2 pos);

		std::partial_ordering operator<=>(const iterator&) const;
		bool operator==(const iterator&) const;

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator with a subscript.
		value_type operator[](difference_type diff) const;
		// Dereferences the iterator with a subscript.
		value_type operator[](glm::ivec2 diff) const;
		// Dereferences the iterator through a pointer.
		pointer operator->() const;

		// Increments the iterator.
		iterator& operator++();
		// Post-increments the iterator.
		iterator operator++(int);
		// Advances the iterator.
		iterator& operator+=(difference_type diff);
		// Advances an iterator.
		iterator& operator+=(glm::ivec2 diff);
		// Adds to an iterator.
		friend iterator operator+(const iterator& it, difference_type diff);
		// Adds to an iterator.
		friend iterator operator+(difference_type diff, const iterator& it);
		// Adds to an iterator.
		friend iterator operator+(const iterator& it, glm::ivec2 diff);
		// Adds to an iterator.
		friend iterator operator+(glm::ivec2 diff, const iterator& it);

		// Decrements the iterator.
		iterator& operator--();
		// Post-decrements the iterator.
		iterator operator--(int);
		// Moves the iterator back.
		iterator& operator-=(difference_type diff);
		// Moves the iterator back.
		iterator& operator-=(glm::ivec2 diff);
		// Subtracts from an iterator.
		friend iterator operator-(const iterator& it, difference_type diff);
		// Subtracts from an iterator.
		friend iterator operator-(difference_type diff, const iterator& it);
		// Subtracts from an iterator.
		friend iterator operator-(const iterator& it, glm::ivec2 diff);
		// Subtracts from an iterator.
		friend iterator operator-(glm::ivec2 diff, const iterator& it);
		// Gets the distance between two iterators.
		friend difference_type operator-(const iterator& l, const iterator& r);

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		pixel_ref m_pixel;
		// The size of the pointed-to bitmap.
		glm::ivec2 m_bitmap_size;
		// The pitch of the pointed-to-bitmap.
		int m_bitmap_pitch;
		// The position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};

	// Mutable reference to a bitmap pixel.
	class bitmap::pixel_ref {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const;
		// Sets the pixel's color value.
		pixel_ref& operator=(rgba8 color);

	  private:
		// A pointer to the pixel data.
		std::byte* m_ptr;
		// The format of the pixel.
		pixel_format m_format;

		// Undefined, required to default-construct iterators.
		pixel_ref() = default;
		// Wraps a pointer to the pixel data.
		pixel_ref(std::byte* ptr, pixel_format format);

		friend class mut_it;
	};

	// Mutable pixel iterator.
	class bitmap::mut_it {
	  public:
		using value_type = pixel_ref;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		mut_it() = default;
		// Constructs an iterator to a bitmap pixel.
		mut_it(bitmap& bitmap, glm::ivec2 pos);

		std::partial_ordering operator<=>(const mut_it&) const;
		bool operator==(const mut_it&) const;

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator with a subscript.
		value_type operator[](difference_type diff) const;
		// Dereferences the iterator with a subscript.
		value_type operator[](glm::ivec2 diff) const;
		// Dereferences the iterator through a pointer.
		pointer operator->() const;

		// Increments the iterator.
		mut_it& operator++();
		// Post-increments the iterator.
		mut_it operator++(int);
		// Advances the iterator.
		mut_it& operator+=(difference_type diff);
		// Advances the iterator.
		mut_it& operator+=(glm::ivec2 diff);
		// Adds to an iterator.
		friend mut_it operator+(const mut_it& it, difference_type diff);
		// Adds to an iterator.
		friend mut_it operator+(difference_type diff, const mut_it& it);
		// Adds to an iterator.
		friend mut_it operator+(const mut_it& it, glm::ivec2 diff);
		// Adds to an iterator.
		friend mut_it operator+(glm::ivec2 diff, const mut_it& it);

		// Decrements the iterator.
		mut_it& operator--();
		// Post-decrements the iterator.
		mut_it operator--(int);
		// Moves the iterator back.
		mut_it& operator-=(difference_type diff);
		// Moves the iterator back.
		mut_it& operator-=(glm::ivec2 diff);
		// Subtracts from an iterator.
		friend mut_it operator-(const mut_it& it, difference_type diff);
		// Subtracts from an iterator.
		friend mut_it operator-(difference_type diff, const mut_it& it);
		// Subtracts from an iterator.
		friend mut_it operator-(const mut_it& it, glm::ivec2 diff);
		// Subtracts from an iterator.
		friend mut_it operator-(glm::ivec2 diff, const mut_it& it);
		// Gets the distance between two iterators.
		friend difference_type operator-(const mut_it& l, const mut_it& r);

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		pixel_ref m_pixel;
		// The pointed-to-bitmap.
		opt_ref<bitmap> m_bitmap;
		// The position of the iterator within the bitmap.
		glm::ivec2 m_bitmap_pos;
	};
} // namespace tr