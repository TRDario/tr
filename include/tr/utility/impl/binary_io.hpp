/// @file
/// @brief Implements the templated parts of binary_io.hpp.

#pragma once
#include "../binary_io.hpp"
#include "../default_binary_io.hpp"

//

namespace tr
{
	/// Enables default binary IO for arithmetic types.
	/// @tparam Arithmetic Built-in arithmetic type.
	template <arithmetic Arithmetic>
		requires(cv_unqualified_object<Arithmetic>)
	inline constexpr bool enable_default_binary_io<Arithmetic>{true};

	// Enables default binary IO for enumerators.
	/// @tparam Enumerator Enumerator type.
	template <enumerator Enumerator>
		requires(cv_unqualified_object<Enumerator>)
	inline constexpr bool enable_default_binary_io<Enumerator>{true};

	/// Enables default binary IO for vectors.
	/// @tparam Dimensions Vector dimensions.
	/// @tparam Element Vector element type.
	template <int Dimensions, typename Element>
	inline constexpr bool enable_default_binary_io<glm::vec<Dimensions, Element>>{true};

	/// Enables default binary IO for matrices.
	/// @tparam Columns Matrix columns.
	/// @tparam Columns Matrix rows.
	/// @tparam Element Matrix element type.
	template <int Columns, int Rows, typename Element>
	inline constexpr bool enable_default_binary_io<glm::mat<Columns, Rows, Element>>{true};

	//

	/// Default binary reader.
	/// @tparam Defaulted Type with `enable_default_binary_io<Defaulted>{true}`.
	template <cv_unqualified_object Defaulted>
		requires(enable_default_binary_io<Defaulted>)
	struct binary_reader<Defaulted>
	{
		/// Marks the reader as default, this is checked in some specializations to enable optimizations.
		using default_reader = std::true_type;

		//

		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, Defaulted& out)
		{
			is.read(reinterpret_cast<char*>(std::addressof(out)), sizeof(Defaulted));
		}
	};

	/// Standard array binary reader.
	/// @tparam Element Array element type.
	/// @tparam Size Size of the array.
	template <binary_readable Element, usize Size>
	struct binary_reader<std::array<Element, Size>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::array<Element, Size>& out)
		{
			read_binary(is, std::span{out});
		}
	};

	/// Pair binary reader.
	/// @tparam First, Second Pair elmenet types.
	template <binary_readable First, binary_readable Second>
	struct binary_reader<std::pair<First, Second>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::pair<First, Second>& out)
		{
			read_binary(is, out.first, out.second);
		}
	};

	/// Standard string binary reader.
	template <>
	struct binary_reader<std::string>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::string& out);
	};

	/// Standard vector binary reader.
	/// @tparam Element Vector element type.
	template <binary_constructible Element>
	struct binary_reader<std::vector<Element>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::vector<Element>& out)
		{
			out.resize(read_binary<u32>(is));
			read_binary(is, std::span{out});
		}
	};

	/// Standard set binary reader.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_constructible Key, typename... Other>
	struct binary_reader<std::set<Key, Other...>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::set<Key, Other...>& out)
		{
			const u32 size{read_binary<u32>(is)};
			out.clear();
			for (u32 i = 0; i < size; ++i) {
				out.insert(read_binary<Key>(is));
			}
		}
	};

	/// Standard map binary reader.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_constructible Key, binary_constructible Value, typename... Other>
	struct binary_reader<std::map<Key, Value, Other...>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::map<Key, Value, Other...>& out)
		{
			const u32 size{read_binary<u32>(is)};
			out.clear();
			for (u32 i = 0; i < size; ++i) {
				Key key{read_binary<Key>(is)};
				Value value{read_binary<Value>(is)};
				out.emplace(std::move(key), std::move(value));
			}
		}
	};

	/// Unordered flat set binary reader.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_constructible Key, typename... Other>
	struct binary_reader<boost::unordered_flat_set<Key, Other...>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, boost::unordered_flat_set<Key, Other...>& out)
		{
			const u32 size{read_binary<u32>(is)};
			out.clear();
			out.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				out.insert(read_binary<Key>(is));
			}
		}
	};

	/// Unordered node set binary reader.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_constructible Key, typename... Other>
	struct binary_reader<boost::unordered_node_set<Key, Other...>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, boost::unordered_node_set<Key, Other...>& out)
		{
			const u32 size{read_binary<u32>(is)};
			out.clear();
			out.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				out.insert(read_binary<Key>(is));
			}
		}
	};

	/// Unordered flat map binary reader.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_constructible Key, binary_constructible Value, typename... Other>
	struct binary_reader<boost::unordered_flat_map<Key, Value, Other...>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, boost::unordered_flat_map<Key, Value, Other...>& out)
		{
			const u32 size{read_binary<u32>(is)};
			out.clear();
			out.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				Key key{read_binary<Key>(is)};
				Value value{read_binary<Value>(is)};
				out.emplace(std::move(key), std::move(value));
			}
		}
	};

	/// Unordered node map binary reader.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_constructible Key, binary_constructible Value, typename... Other>
	struct binary_reader<boost::unordered_node_map<Key, Value, Other...>>
	{
		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, boost::unordered_node_map<Key, Value, Other...>& out)
		{
			const u32 size{read_binary<u32>(is)};
			out.clear();
			out.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				Key key{read_binary<Key>(is)};
				Value value{read_binary<Value>(is)};
				out.emplace(std::move(key), std::move(value));
			}
		}
	};

	//

	/// Default binary writer.
	/// @tparam Defaulted Type with `enable_default_binary_io<Defaulted>{true}`.
	template <cv_unqualified_object Defaulted>
		requires(enable_default_binary_io<Defaulted>)
	struct binary_writer<Defaulted>
	{
		/// Marks the writer as default, this is checked in some specializations to enable optimizations.
		using default_writer = std::true_type;

		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const Defaulted& in)
		{
			os.write(reinterpret_cast<const char*>(std::addressof(in)), sizeof(Defaulted));
		}
	};

	/// Span binary writer.
	/// @tparam Element Span element type.
	/// @tparam Size Span size.
	template <binary_writable Element, usize Size>
	struct binary_writer<std::span<Element, Size>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::span<Element, Size>& in)
		{
			if constexpr (requires {
							  requires std::same_as<typename binary_writer<std::remove_const_t<Element>>::default_writer, std::true_type>;
						  }) {
				os.write(reinterpret_cast<const char*>(in.data()), in.size_bytes());
			}
			else {
				for (const Element& element : in) {
					write_binary(os, element);
				}
			}
		}
	};

	/// Pair binary writer.
	/// @tparam First, Second Pair element types.
	template <binary_writable First, binary_writable Second>
	struct binary_writer<std::pair<First, Second>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::pair<First, Second> in)
		{
			write_binary(os, in.first, in.second);
		}
	};

	/// C String binary writer.
	template <>
	struct binary_writer<const char*>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const char* in);
	};

	/// Standard string view binary writer.
	template <>
	struct binary_writer<std::string_view>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::string_view& in);
	};

	/// C String binary writer.
	/// @tparam Size String size.
	template <usize Size>
	struct binary_writer<char[Size]>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const char (&in)[Size])
		{
			write_binary(os, std::string_view{in});
		}
	};

	/// Standard string binary writer.
	template <>
	struct binary_writer<std::string>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::string& in);
	};

	/// Raw array binary writer.
	/// @tparam Element Array element type.
	/// @tparam Size Array size.
	template <binary_writable Element, usize Size>
	struct binary_writer<Element[Size]>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const Element (&in)[Size])
		{
			write_binary(os, std::span{in});
		}
	};

	/// Standard array binary writer.
	/// @tparam Element Array element type.
	/// @tparam Size Array size.
	template <binary_writable Element, usize Size>
	struct binary_writer<std::array<Element, Size>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::array<Element, Size>& in)
		{
			write_binary(os, std::span{in});
		}
	};

	/// Standard vector binary writer.
	/// @tparam Element Vector element type.
	template <binary_writable Element>
	struct binary_writer<std::vector<Element>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::vector<Element>& in)
		{
			write_binary(os, static_cast<u32>(in.size()), std::span{in});
		}
	};

	/// Standard set binary writer.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_writable Key, typename... Other>
	struct binary_writer<std::set<Key, Other...>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::set<Key, Other...>& in)
		{
			write_binary(os, static_cast<u32>(in.size()));
			for (const Key& key : in) {
				write_binary(os, key);
			}
		}
	};

	/// Standard map binary writer.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<std::map<Key, Value, Other...>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::map<Key, Value, Other...>& in)
		{
			write_binary(os, static_cast<u32>(in.size()));
			for (const auto& [key, value] : in) {
				write_binary(os, key, value);
			}
		}
	};

	/// Unordered flat set binary writer.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_writable Key, typename... Other>
	struct binary_writer<boost::unordered_flat_set<Key, Other...>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const boost::unordered_flat_set<Key, Other...>& in)
		{
			write_binary(os, static_cast<u32>(in.size()));
			for (const Key& key : in) {
				write_binary(os, key);
			}
		}
	};

	/// Unordered node set binary writer.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_writable Key, typename... Other>
	struct binary_writer<boost::unordered_node_set<Key, Other...>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const boost::unordered_node_set<Key, Other...>& in)
		{
			write_binary(os, static_cast<u32>(in.size()));
			for (const Key& key : in) {
				write_binary(os, key);
			}
		}
	};

	/// Unordered flat map writer.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<boost::unordered_flat_map<Key, Value, Other...>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const boost::unordered_flat_map<Key, Value, Other...>& in)
		{
			write_binary(os, static_cast<u32>(in.size()));
			for (const auto& [key, value] : in) {
				write_binary(os, key, value);
			}
		}
	};

	/// Unordered node map writer.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<boost::unordered_node_map<Key, Value, Other...>>
	{
		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const boost::unordered_node_map<Key, Value, Other...>& in)
		{
			write_binary(os, static_cast<u32>(in.size()));
			for (const auto& [key, value] : in) {
				write_binary(os, key, value);
			}
		}
	};
} // namespace tr

//

template <tr::binary_readable Out>
void tr::read_binary(std::istream& is, Out& out)
{
	binary_reader<std::remove_volatile_t<Out>>{}(is, out);
}

template <tr::binary_readable Out, tr::usize Size>
void tr::read_binary(std::istream& is, std::span<Out, Size> out)
{
	if constexpr (requires { requires std::same_as<typename binary_reader<Out>::default_reader, std::true_type>; }) {
		is.read(reinterpret_cast<char*>(out.data()), out.size_bytes());
	}
	else {
		for (Out& v : out) {
			read_binary(is, v);
		}
	}
}

template <tr::span_or_ref_to_binary_readable... Outs>
	requires(sizeof...(Outs) >= 2)
void tr::read_binary(std::istream& is, Outs&&... outs)
{
	(read_binary(is, outs), ...);
}

template <tr::binary_constructible Out>
Out tr::read_binary(std::istream& is)
{
	Out out;
	read_binary(is, out);
	return out;
}

template <tr::binary_flushable_iterator Iterator>
void tr::flush_binary(std::istream& is, Iterator out)
{
	while (is.peek() != EOF) {
		if constexpr (std::output_iterator<Iterator, char>) {
			*out++ = static_cast<char>(is.get());
		}
		else if constexpr (std::output_iterator<Iterator, signed char>) {
			*out++ = static_cast<signed char>(is.get());
		}
		else if constexpr (std::output_iterator<Iterator, unsigned char>) {
			*out++ = static_cast<unsigned char>(is.get());
		}
		else {
			*out++ = static_cast<std::byte>(is.get());
		}
	}
}

template <tr::binary_writable In>
void tr::write_binary(std::ostream& os, const In& in)
{
	binary_writer<std::remove_cv_t<In>>{}(os, in);
}

template <tr::binary_writable... Ins>
	requires(sizeof...(Ins) >= 2)
void tr::write_binary(std::ostream& os, const Ins&... ins)
{
	(write_binary(os, ins), ...);
}