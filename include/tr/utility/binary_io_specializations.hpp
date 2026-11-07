/// @file
/// @brief Provides default binary reading/writing specializations for a number of common types.

#pragma once
#include <tr/utility/binary_io.hpp>
#include <tr/utility/default_binary_io.hpp>

//

namespace tr
{
	/// @brief Default binary reader.
	/// @tparam Defaulted Type with `enable_default_binary_io<Defaulted>{true}`.
	template <typename Defaulted>
		requires enable_default_binary_io<Defaulted>
	struct binary_reader<Defaulted>
	{
		/// @brief Marks the reader as default, this is checked in some specializations to enable optimizations.
		using default_reader = std::true_type;

		//

		/// @brief Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, Defaulted& out)
		{
			is.read(reinterpret_cast<char*>(std::addressof(out)), sizeof(Defaulted));
		}
	};

	/// @brief Standard array binary reader.
	/// @tparam Element Array element type.
	/// @tparam Size Size of the array.
	template <binary_readable Element, usize Size>
	struct binary_reader<std::array<Element, Size>>
	{
		/// @brief Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::array<Element, Size>& out)
		{
			read_binary(is, std::span{out});
		}
	};

	/// @brief Pair binary reader.
	/// @tparam First, Second Pair element types.
	template <binary_readable First, binary_readable Second>
	struct binary_reader<std::pair<First, Second>>
	{
		/// @brief Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::pair<First, Second>& out)
		{
			read_binary(is, out.first, out.second);
		}
	};

	/// @brief Standard string binary reader.
	template <>
	struct binary_reader<std::string>
	{
		/// @brief Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::string& out);
	};

	/// @brief Standard vector binary reader.
	/// @tparam Element Vector element type.
	template <binary_constructible Element>
	struct binary_reader<std::vector<Element>>
	{
		/// @brief Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, std::vector<Element>& out)
		{
			out.resize(read_binary<u32>(is));
			read_binary(is, std::span{out});
		}
	};

	/// @brief Standard set binary reader.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_constructible Key, typename... Other>
	struct binary_reader<std::set<Key, Other...>>
	{
		/// @brief Reads a variable from a stream.
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

	/// @brief Standard map binary reader.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_constructible Key, binary_constructible Value, typename... Other>
	struct binary_reader<std::map<Key, Value, Other...>>
	{
		/// @brief Reads a variable from a stream.
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

	/// @brief Unordered flat set binary reader.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_constructible Key, typename... Other>
	struct binary_reader<boost::unordered_flat_set<Key, Other...>>
	{
		/// @brief Reads a variable from a stream.
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

	/// @brief Unordered node set binary reader.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_constructible Key, typename... Other>
	struct binary_reader<boost::unordered_node_set<Key, Other...>>
	{
		/// @brief Reads a variable from a stream.
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

	/// @brief Unordered flat map binary reader.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_constructible Key, binary_constructible Value, typename... Other>
	struct binary_reader<boost::unordered_flat_map<Key, Value, Other...>>
	{
		/// @brief Reads a variable from a stream.
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

	/// @brief Unordered node map binary reader.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_constructible Key, binary_constructible Value, typename... Other>
	struct binary_reader<boost::unordered_node_map<Key, Value, Other...>>
	{
		/// @brief Reads a variable from a stream.
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

	/// @brief Default binary writer.
	/// @tparam Defaulted Type with `enable_default_binary_io<Defaulted>{true}`.
	template <typename Defaulted>
		requires enable_default_binary_io<Defaulted>
	struct binary_writer<Defaulted>
	{
		/// @brief Marks the writer as default, this is checked in some specializations to enable optimizations.
		using default_writer = std::true_type;

		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const Defaulted& in)
		{
			os.write(reinterpret_cast<const char*>(std::addressof(in)), sizeof(Defaulted));
		}
	};

	/// @brief Span binary writer.
	/// @tparam Element Span element type.
	/// @tparam Size Span size.
	template <binary_writable Element, usize Size>
	struct binary_writer<std::span<Element, Size>>
	{
		/// @brief Writes a variable to a stream.
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

	/// @brief Pair binary writer.
	/// @tparam First, Second Pair element types.
	template <binary_writable First, binary_writable Second>
	struct binary_writer<std::pair<First, Second>>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::pair<First, Second> in)
		{
			write_binary(os, in.first, in.second);
		}
	};

	/// @brief C String binary writer.
	template <>
	struct binary_writer<const char*>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const char* in);
	};

	/// @brief Standard string view binary writer.
	template <>
	struct binary_writer<std::string_view>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::string_view& in);
	};

	/// @brief C String binary writer.
	/// @tparam Size String size.
	template <usize Size>
	struct binary_writer<char[Size]>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const char (&in)[Size])
		{
			write_binary(os, std::string_view{in});
		}
	};

	/// @brief Standard string binary writer.
	template <>
	struct binary_writer<std::string>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::string& in);
	};

	/// @brief Raw array binary writer.
	/// @tparam Element Array element type.
	/// @tparam Size Array size.
	template <binary_writable Element, usize Size>
	struct binary_writer<Element[Size]>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const Element (&in)[Size])
		{
			write_binary(os, std::span{in});
		}
	};

	/// @brief Standard array binary writer.
	/// @tparam Element Array element type.
	/// @tparam Size Array size.
	template <binary_writable Element, usize Size>
	struct binary_writer<std::array<Element, Size>>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::array<Element, Size>& in)
		{
			write_binary(os, std::span{in});
		}
	};

	/// @brief Standard vector binary writer.
	/// @tparam Element Vector element type.
	template <binary_writable Element>
	struct binary_writer<std::vector<Element>>
	{
		/// @brief Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const std::vector<Element>& in)
		{
			write_binary(os, static_cast<u32>(in.size()), std::span{in});
		}
	};

	/// @brief Standard set binary writer.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_writable Key, typename... Other>
	struct binary_writer<std::set<Key, Other...>>
	{
		/// @brief Writes a variable to a stream.
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

	/// @brief Standard map binary writer.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<std::map<Key, Value, Other...>>
	{
		/// @brief Writes a variable to a stream.
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

	/// @brief Unordered flat set binary writer.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_writable Key, typename... Other>
	struct binary_writer<boost::unordered_flat_set<Key, Other...>>
	{
		/// @brief Writes a variable to a stream.
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

	/// @brief Unordered node set binary writer.
	/// @tparam Key Set key type.
	/// @tparam Other Other set types.
	template <binary_writable Key, typename... Other>
	struct binary_writer<boost::unordered_node_set<Key, Other...>>
	{
		/// @brief Writes a variable to a stream.
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

	/// @brief Unordered flat map writer.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<boost::unordered_flat_map<Key, Value, Other...>>
	{
		/// @brief Writes a variable to a stream.
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

	/// @brief Unordered node map writer.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Other Other map types.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<boost::unordered_node_map<Key, Value, Other...>>
	{
		/// @brief Writes a variable to a stream.
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