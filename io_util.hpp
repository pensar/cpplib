// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef io_utilH
#define io_utilH

#include "object.hpp"
#include "string_def.hpp"
#include "memory.hpp"

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

            // Copy constructor.
            Path (const Path& p) noexcept : fs::path (p) {}

            // Move constructor.
            Path (Path&& p) noexcept : fs::path (p) {}


            // Move assignment operator.
            Path& operator = (Path&& p) noexcept { fs::path::operator = (p); return *this; }

            // Assignment operator for std::string.
            Path& operator = (const std::string& s) { fs::path::operator = (s); return *this; }
            
            // Assignment operator for const Path&.
            Path& operator = (const Path& p) { fs::path::operator = (p); return *this; } 
            
            void create_dir_if_does_not_exist () const noexcept
            {
                Path dir = *this;
                if (has_filename())
                    dir = parent_path();
                if (! dir.exists ())
                {
                    fs::create_directories (dir);
                }
            }

            // Implicit conversion to std::string.
            operator std::string () const { return fs::path::string (); }

            // Implicit conversion to const char* returning a value allocated in the heap using _strdup.
            operator const char* () const { return _strdup (fs::path::string ().c_str ()); }

            // /= operator for std::string.
            Path& operator /= (const std::string& s) { fs::path::operator /= (s); return *this; }

            // /= operator for const Path&.
            Path& operator /= (const Path& p) { fs::path::operator /= (p); return *this; }
            
            // + operator for std::string.s
            Path operator + (const std::string& s) const { Path p = *this; p /= s; return p; }

            // == operator 
            bool operator == (const Path& apath) const { return apath == fs::path::c_str (); }

            // != operator for std::string.
            bool operator != (const std::string& s) const { return ! (*this == s); }

            // == operator for const char*.
            bool operator == (const char* s) const { return *this == std::string (s); }     

            // != operator for const char*.
            bool operator != (const char* s) const { return ! (*this == s); }
            

            bool exists () const { return fs::exists (*this); }
		};


        /// \brief File class.
        ///
        class File: Object
        {
            protected:

            Path full_path;
            std::ios_base::openmode mode;
            std::fstream file;

            public:

            const static Version VERSION                     = 1;
            const static Version PUBLIC_INTERFACE_VERSION    = 1;
            const static Version PROTECTED_INTERFACE_VERSION = 1;
            const static Version PRIVATE_INTERFACE_VERSION   = 1;

            virtual Version get_version                     () const noexcept { return VERSION;                     }
            virtual Version get_public_interface_version    () const noexcept { return PUBLIC_INTERFACE_VERSION;    }
            virtual Version get_protected_interface_version () const noexcept { return PROTECTED_INTERFACE_VERSION; }
            virtual Version get_private_interface_version   () const noexcept { return PRIVATE_INTERFACE_VERSION;   }

            const size_t MAX_IN_MEMORY_FILE_SIZE_BYTE = 1024 ^ 3; // 1 GB

            File(const Path& full_path, const std::ios_base::openmode amode, const Id aid = NULL_ID) : Object(aid),
                                                                                                       full_path(full_path), 
                                                                                                       mode(amode)
            {
                if (full_path.exists ())
                {
                    if (full_path.has_filename ())
                    {
                        std::vector<char> buffer;
                        size_t file_size = fs::file_size (full_path);
                        if ((file_size < MAX_IN_MEMORY_FILE_SIZE_BYTE) && (file_size < get_available_memory ()))
                        {
                            // Reads file into buffer.
                            std::ifstream file (static_cast<fs::path>(full_path), mode);
                            if (file.is_open ())
							{
								file.seekg (0, std::ios::end);
								size_t size = file.tellg ();
								file.seekg (0, std::ios::beg);
								buffer.resize (size);
								file.read (buffer.data (), size);
								close ();
							}
                            else
                            {
    							// File exists but could not be opened.
	    						throw std::runtime_error ("Could not open file " + full_path.string ());
							}
						}   
                        else
                        {
                            // Read a file when size > 1GB;

                        }
                    }
                    else
					{
						// Path does not have a filename.
						throw std::runtime_error("Path " + full_path.string() + " does not have a filename.");
                    }
                }
                else
                {
                    // File does not exist.
                    full_path.create_dir_if_does_not_exist();
                    mode |= std::ios::out;
                    open (full_path, mode);
                }
            }
            
            virtual ~File() noexcept { close(); }

            void close() noexcept { file.close(); }
            
            bool exists() const { return full_path.exists(); }

            void open(const Path& afull_path = "", const std::ios_base::openmode amode = 0)
            {
                if (afull_path != "")
				{
					full_path = afull_path;
				}  
                if (amode != 0)
                {
                    mode = amode;
    			}
                file.open(static_cast<fs::path>(full_path), mode);
            }

            bool is_open                       () const noexcept { return file.is_open ();                }
            bool is_good                       () const noexcept { return file.good ();                   }
            bool is_app                        () const noexcept { return (mode & std::ios::app) != 0;    }
            bool is_ate                        () const noexcept { return (mode & std::ios::ate) != 0;    }
            bool is_auto_seek_end_before_write () const noexcept { return is_app ();                      }
            bool is_auto_seek_end_on_open      () const noexcept { return is_ate();                       }
            bool is_binary                     () const noexcept { return (mode & std::ios::binary) != 0; }
            bool is_in                         () const noexcept { return (mode & std::ios::in) != 0;     }
            bool is_out                        () const noexcept { return (mode & std::ios::out) != 0;    }
            bool is_trunc                      () const noexcept { return (mode & std::ios::trunc) != 0;  }
            bool eof                           () const noexcept { return file.eof ();                    }
            bool fail                          () const noexcept { return file.fail ();                   }
            bool bad                           () const noexcept { return file.bad ();                    }
            bool good                          () const noexcept { return file.good ();                   }
        };

        class TextFile : public File
        {
			private:
			public:
            TextFile(const Path& full_path, const std::ios_base::openmode amode, const String& content = "", const Id aid = NULL_ID) : File(full_path, (amode & ~std::ios::binary), aid)
            {
                save(content);
            }
            TextFile(const Path& full_path, const String& content = "", const Id aid = NULL_ID) : TextFile(full_path, std::ios::in | std::ios::out, content, aid)
            {
			}

            File& save(const String& content)
            {
                if (is_open())
                {
                    file.seekg(0, std::ios::end);
                }
                else
                {
					if (full_path.exists())
					{
						if (full_path.has_filename ())
						{
							file.open(static_cast<fs::path>(full_path), mode);
						}
                        else
                        {
							// Path exists but is not a file.
							throw std::runtime_error("Path " + full_path.string() + " exists but is not a file.");
						}
					}
					else
					{
                        full_path.create_dir_if_does_not_exist ();
						mode |= std::ios::out;
					}   
                    file.open(static_cast<fs::path>(full_path), mode);
                };
                file << content;
                return *this;
			}    
		};

        class BinaryFile : public File
		{   
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
