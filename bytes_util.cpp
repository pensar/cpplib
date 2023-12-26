#include "bytes_util.hpp"

#include <span>
#include <string>
#include <vector>
#include <cstddef>	 // std::byte
#include <bit>		 // std::endian
#include <algorithm> // std::reverse

namespace pensar_digital
{
	namespace cpplib
	{

		/// \brief A method to convert a string into a std::vector<std::byte>.
		/// \param s The string to convert.
		/// \param v The std::vector<std::byte> to convert the string into.
		void string_to_bytes(const std::string& s, Bytes& v)
		{
			const char* cp = s.data();
			for (size_t i = 0; i < s.size(); ++i)
			{
				v.push_back(std::byte(*cp++));
			}
		}

		/// \brief A method to convert a string into a std::vector<std::byte>.
		/// \param s The wstring to convert.
		/// \param v The std::vector<std::byte> to convert the string into.
		void string_to_bytes(const std::wstring& s, Bytes& v) 
		{
			for (auto& ch : s) 
			{
				const std::byte* p = reinterpret_cast<const std::byte*>(&ch);
				Bytes bytes(p, p + sizeof(wchar_t));
				if constexpr (std::endian::native == std::endian::little) {
					std::reverse(bytes.begin(), bytes.end());
				}
				v.insert(v.end(), bytes.begin(), bytes.end());
			}
		}

		///\brief A method to convert a std::span<std::byte> into a string.
		///\param bytes The std::span<std::byte> to convert.
		/// \return A string of the std::span<std::byte>.
		void bytes_to_string(const Bytes& bytes, std::string& s)
		{
			s.clear();
			for (auto& b : bytes)
			{
				s.push_back(static_cast<char>(b));
			}
		}


		///\brief A method to convert a std::span<std::byte> into a string.
		///\param bytes The std::span<std::byte> to convert.
		/// \return A string of the std::span<std::byte>.
		void bytes_to_wstring(Bytes& bytes, std::wstring s)
		{
			s.clear();
			for (size_t i = 0; i < bytes.size(); i += sizeof(wchar_t))
			{
				std::byte* p = bytes.data() + i;
				wchar_t ch;
				std::copy(p, p + sizeof(wchar_t), reinterpret_cast<std::byte*>(&ch));
				if constexpr (std::endian::native == std::endian::little) {
					std::reverse(reinterpret_cast<std::byte*>(&ch), reinterpret_cast<std::byte*>(&ch) + sizeof(wchar_t));
				}
				s.push_back(ch);
			}
		}

		void add_bytes(const Bytes& bytes, Bytes& dest)
		{
			dest.insert (dest.end(), bytes.begin(), bytes.end());
		}

		///\brief Adds a string to a std::span<std::byte>.
		///\param bytes The std::span<std::byte> to add the string to.	
		/// 
		/// The string is added to the end of the std::span<std::byte> and the size of the std::span<std::byte> is increased by the size of the string.
		/// The std::span<std::byte> is resized if it is not large enough to hold the string.
		/// \param str The string to add to the std::span<std::byte>.

		void add_string_to_bytes (const std::string& s, Bytes& bytes)
		{
			Bytes str_bytes;
			string_to_bytes (s, str_bytes);
			add_bytes (bytes, str_bytes);
		}

		void int16_t_to_bytes (const int16_t i, Bytes& bytes)
		{
			add_number_to_bytes<int16_t> (i, bytes);
		}

		void int32_t_to_bytes (const int32_t i, Bytes& bytes)
		{
			add_number_to_bytes<int32_t>(i, bytes);
		}

		void int64_t_to_bytes (const int64_t i, Bytes& bytes)
		{
			add_number_to_bytes<int64_t>(i, bytes);
		}

		void int_to_bytes (const int i, Bytes& bytes)
		{
			add_number_to_bytes<int>(i, bytes);
		}

		void uint16_t_to_bytes (const uint16_t i, Bytes& bytes)
		{
			add_number_to_bytes<uint16_t> (i, bytes);
		}

		void uint32_t_to_bytes (const uint32_t i, Bytes& bytes)
		{
			add_number_to_bytes<uint32_t> (i, bytes);
		}

		void uint64_t_to_bytes (const uint64_t i, Bytes& bytes)
		{
			add_number_to_bytes<uint64_t> (i, bytes);
		}

		void uint_to_bytes (const unsigned int i, Bytes& bytes)
		{
			add_number_to_bytes<unsigned int> (i, bytes);
		}

		void float_to_bytes (const float f, Bytes& bytes)
		{
			// todo.
		}

		void double_to_bytes (const double d, Bytes& bytes)

		{
			// todo.
		}

		void bool_to_bytes (const bool b, Bytes& bytes)
		{
			add_number_to_bytes<bool>(b, bytes);
		}

		void long_to_bytes (const long l, Bytes& bytes)
		{
			add_number_to_bytes<long>(l, bytes);
		}

		void ulong_to_bytes (const unsigned long l, Bytes& bytes)
		{
			add_number_to_bytes<unsigned long>(l, bytes);
		}

		void long_long_to_bytes (const long long ll, Bytes& bytes)
		{
			add_number_to_bytes<long long> (ll, bytes);
		}	

		void ulong_long_to_bytes (const unsigned long long ll, Bytes& bytes)
		{
			add_number_to_bytes<unsigned long long> (ll, bytes);
		}

		int32_t bytes_to_int32_t (const Bytes& bytes)
		{
			return bytes_to_type<int32_t> (bytes);
		}	

	} // namespace cpplib
} // namespace pensar_digital