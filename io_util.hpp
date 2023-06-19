// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef io_utilH
#define io_utilH

#include "object.hpp"
#include <string>
#ifdef _MSC_VER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif


#ifdef WINDOWS
    #include <io.h>
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        #ifdef _MSC_VER
            namespace fs = std::filesystem;
        #else
            namespace fs = std::experimental::filesystem;
        #endif

        // Path class represents a path in the file system. It inherits from fs::path adding implicit conversion to std::string.
        class Path : public fs::path
		{
			public:
			Path (const fs::path& p) : fs::path (p) {}
			Path (const std::string& s) : fs::path (s) {}
			Path (const char* s) : fs::path (s) {}

            // Assignment operator for std::string.
            Path& operator = (const std::string& s) { fs::path::operator = (s); return *this; }
            
            // Assignment operator for const Path&.
            Path& operator = (const Path& p) { fs::path::operator = (p); return *this; } 
            
            // Implicit conversion to std::string.
            operator std::string () const { return fs::path::string (); }

            // /= operator for std::string.
            Path& operator /= (const std::string& s) { fs::path::operator /= (s); return *this; }

            // /= operator for const Path&.
            Path& operator /= (const Path& p) { fs::path::operator /= (p); return *this; }
		};


        /// \brief File class.
        ///
        class File: Object
        {
            public:
            const static Version VERSION                     = 1;
            const static Version PUBLIC_INTERFACE_VERSION    = 1;
            const static Version PROTECTED_INTERFACE_VERSION = 1;
            const static Version PRIVATE_INTERFACE_VERSION   = 1;

            virtual Version get_version                     () const noexcept { return VERSION;                     }
            virtual Version get_public_interface_version    () const noexcept { return PUBLIC_INTERFACE_VERSION;    }
            virtual Version get_protected_interface_version () const noexcept { return PROTECTED_INTERFACE_VERSION; }
            virtual Version get_private_interface_version   () const noexcept { return PRIVATE_INTERFACE_VERSION;   }

            const size_t MAX_IN_MEMORY_FILE_SIZE_BYTE = 1024 ^ 3;

            File (const fs::path& full_path, const Id aid = NULL_ID) : Object (aid)
            {
                if (fs::exists (full_path))
                {
                    if (full_path.has_filename ())
                    {
                        std::vector<char> buffer;
                        if (fs::file_size (full_path) < MAX_IN_MEMORY_FILE_SIZE_BYTE)
                        {
                            // Reads file into buffer.
                            std::ifstream file (full_path, std::ios::binary);
                            if (file.is_open ())
							{
								file.seekg (0, std::ios::end);
								size_t size = file.tellg ();
								file.seekg (0, std::ios::beg);
								buffer.resize (size);
								file.read (buffer.data (), size);
								file.close ();
							}
                            else
                            {
                                // Discovers why file could not be opened.
                                if (fs::exists(full_path))
                                {
									// File exists but could not be opened.
									throw std::runtime_error ("Could not open file " + full_path.string ());
								}
								else
								{
									// File does not exist.
									throw std::runtime_error ("File " + full_path.string () + " does not exist.");
								}
							}   
						}
						else
						{
							;
                        }
                    }
                }
            }
        };

        using LINE_HANDLER = void(*)(const int64_t line_count, const std::string& line);
        extern uintmax_t read_file(const std::string& fname, LINE_HANDLER f);

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
        extern bool can_read (const char* file_name);

        #ifdef WIN32
        extern __int64 get_file_size (const std::string& file_name);
        #endif

        extern bool file_exists (std::string filename);
    }   // namespace cpplib
}       // namespace pensar_digital
#endif
