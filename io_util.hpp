// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef io_utilH
#define io_utilH

#include "string_def.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "concept.hpp"
#include "json_util.hpp"

#include <string>
#ifdef _MSC_VER
#include <io.h>
#include <windows.h>
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        namespace fs = std::filesystem;

#ifdef _MSC_VER
        inline String& windows_read_file (const String& filename, String* s) 
        {
            HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file == INVALID_HANDLE_VALUE) {
                // handle error
            }
            DWORD file_size = GetFileSize(file, NULL);
            HANDLE file_mapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
            if (file_mapping == NULL) {
                // handle error
            }
            char* addr = static_cast<char*>(MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, file_size));
            if (addr == NULL) {
                // handle error
            }
            s = new String(addr, file_size);
            UnmapViewOfFile(addr);
            CloseHandle(file_mapping);
            CloseHandle(file);
            return *s;
        }
#endif
#ifdef __linux__
        inline String& linux_read_file (const String& filename, String* s)
		{
			int fd = open(filename.c_str(), O_RDONLY);
			if (fd == -1) {
				// handle error
			}
			struct stat sb;
			if (fstat(fd, &sb) == -1) {
				// handle error
			}
			char* addr = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0u));
			if (addr == MAP_FAILED) {
				// handle error
			}
			s = new String (addr, sb.st_size);
			munmap(addr, sb.st_size);
			close(fd);
			return s;
		}
#endif  
        inline String& read_file_mmap (const String& filename, String *s) 
		{   
#ifdef _MSC_VER
			return windows_read_file (filename, s);
#endif  
#ifdef __linux__    
        	return linux_read_file (filename, s);
#endif  
        }

        template <typename T>
        void binary_write(std::ostream& os, const T& t, const size_t& size, const std::endian& byte_order = std::endian::native)
        {
            os.write((char*)&size, sizeof(size));
            os.write((char*)&t, size);
            
            /*
            if (byte_order == LITTLE_ENDIAN) {
				for (size_t i = 0; i < size; ++i) {
					os.put(static_cast<char>(t >> (i * 8)));
				}
			}
			else {
				for (size_t i = 0; i < size; ++i) {
					os.put(static_cast<char>(t >> ((size - i - 1) * 8)));
				}
			}
            */
		}
                      
        /*
        template <typename T>
        void binary_write(std::ostream& os, const T& t, const size_t& size)
        {
            os.write(reinterpret_cast<const char*>(&t), size);
        }
        */

        template <Sizeofable T>
        void binary_write (std::ostream& os, const T& t, const std::endian& byte_order = std::endian::native)
        {
            os.write ((char*)&t, sizeof(t));
		}

        // binary_write for std::basic_string.
        template <typename CharType>
		void binary_write (std::ostream& os, const std::basic_string<CharType>& s, const std::endian& byte_order = std::endian::native)
		{
			binary_write<size_t> (os, s.size(), byte_order);
			for (auto&& c : s) {
				binary_write<CharType> (os, c, byte_order);
			}
		}  

        // binary_write for String.
        inline void binary_write (std::ostream& os, const String& s, const std::endian& byte_order = std::endian::native)
        {
            binary_write<String::value_type> (os, s, byte_order);
		}

        template <typename T>
        void binary_read (std::istream& is, T& t, const size_t& size, const std::endian& byte_order = std::endian::native)
		{
            is.read ((char*)(&size), sizeof(size));
            //str.resize(size);
            is.read ((char*)(&t), size);
            /*
            t = 0;
			if (bye_order == LITTLE_ENDIAN) 
            {
                for (size_t i = 0; i < size; ++i) 
                {
                    t |= static_cast<T>(static_cast<uint8_t>(is.get())) << (i * 8);
                }
            }
            else 
            {
                for (size_t i = 0; i < size; ++i) 
                {
                    t |= static_cast<T>(static_cast<uint8_t>(is.get())) << ((size - i - 1) * 8);
                }
            }
            */
        }

        template <Sizeofable T>
        void binary_read (std::istream& is, T& t, const std::endian& byte_order = std::endian::native)
        {
            is.read ((char*)(&t), sizeof(t));
        }

        template <typename CharType>
        void binary_read (std::istream& is, std::basic_string<CharType>& s, const std::endian& byte_order = std::endian::native)
		{
			size_t size;
			binary_read<size_t> (is, size, byte_order);
			s.clear();
			s.reserve(size);
			for (size_t i = 0; i < size; ++i) {
				CharType c;
				binary_read<CharType> (is, c, byte_order);
				s.push_back(c);
			}
		}

        inline void binary_read (std::istream& is, String& s, const std::endian& byte_order = std::endian::native)
		{
			binary_read<String::value_type> (is, s, byte_order);
		}

        template <typename DataType = uint8_t, typename CharType = String::value_type>
        std::basic_string<CharType>& binary_to_string (const std::vector<DataType>& data, std::basic_string<CharType>& out)
        {
            out.clear();
            out.reserve(data.size() * sizeof(DataType) / sizeof(CharType));
            for (auto&& byte : data) {
                for (size_t i = 0; i < sizeof(DataType); ++i) {
                    out.push_back(static_cast<CharType>(byte >> (i * 8)));
                }
            }
            return out;
        }

        template <typename DataType = uint8_t, typename CharType = char>
        void string_to_binary (const std::basic_string<CharType>&in, std::vector<DataType>&out)
        {
			out.clear();
			out.reserve(in.size() * sizeof(CharType) / sizeof(DataType));
            for (size_t i = 0; i < in.size(); i += sizeof(DataType)) {
				DataType byte = 0;
                for (size_t j = 0; j < sizeof(DataType); ++j) {
					byte |= static_cast<DataType>(static_cast<uint8_t>(in[i + j])) << (j * 8);
				}
				out.push_back(byte);
			}
		} 

        using LINE_HANDLER = void(*)(const int64_t line_count, const std::string& line);
        extern uintmax_t read_file(const std::string& fname, LINE_HANDLER f);

/*
#include <fstream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

        std::string read_file(const std::string& filename) {
            int fd = open(filename.c_str(), O_RDONLY);
            if (fd == -1) {
                // handle error
            }
            struct stat sb;
            if (fstat(fd, &sb) == -1) {
                // handle error
            }
            char* addr = static_cast<char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
            if (addr == MAP_FAILED) {
                // handle error
            }
            std::string str(addr, sb.st_size);
            munmap(addr, sb.st_size);
            close(fd);
            return str;
        }
*/

        // Constantes usadas em is_same ().
        extern const int SAME_NAME;
        extern const int SAME_SIZE;
        extern const int SAME_TIME;
        extern const int SAME_ALL;

        #ifdef BORLAND
        extern bool operator == (const std::ftime& left, const std::ftime& right);
        extern bool operator != (const std::ftime& left, const std::ftime& right);
        extern bool operator <  (const std::ftime& left, const std::ftime& right);
        extern std::ftime fileTimestamp (const char* gridFilePath);
        extern std::string fileTimestampString (const char* gridFilePath);
        #endif

        //extern long long get_file_size(const char* sFileName);
        //extern bool can_read (const char* file_name);

        #ifdef WIN32
        extern __int64 get_file_size (const std::string& file_name);
        #endif

        //extern bool file_exists (std::string filename);
    }   // namespace cpplib
}       // namespace pensar_digital
#endif
