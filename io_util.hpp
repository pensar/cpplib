// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef io_utilH
#define io_utilH

#include "object.hpp"
#include "string_def.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "version.hpp"
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
        void binary_write(std::ostream& os, const T& t, const size_t& size, const ByteOrder& byte_order = LITTLE_ENDIAN)
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

        template <SizeableType T>
        void binary_write (std::ostream& os, const T& t, const ByteOrder& byte_order = LITTLE_ENDIAN)
        {
            os.write ((char*)&t, sizeof(t));
		}

        // binary_write for std::basic_string.
        template <typename CharType>
		void binary_write (std::ostream& os, const std::basic_string<CharType>& s, const ByteOrder& byte_order = LITTLE_ENDIAN)
		{
			binary_write<size_t> (os, s.size(), byte_order);
			for (auto&& c : s) {
				binary_write<CharType> (os, c, byte_order);
			}
		}  

        // binary_write for String.
        inline void binary_write (std::ostream& os, const String& s, const ByteOrder& byte_order = LITTLE_ENDIAN)
        {
            binary_write<String::value_type> (os, s, byte_order);
		}

        template <typename T>
        void binary_read (std::istream& is, T& t, const size_t& size, const ByteOrder& byte_order = LITTLE_ENDIAN)
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

        template <Sizeable T>
        void binary_read (std::istream& is, T& t, const ByteOrder& byte_order = LITTLE_ENDIAN)
        {
            is.read ((char*)(&t), sizeof(t));
        }

        template <typename CharType>
        void binary_read (std::istream& is, std::basic_string<CharType>& s, const ByteOrder& byte_order = LITTLE_ENDIAN)
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

        inline void binary_read (std::istream& is, String& s, const ByteOrder& byte_order = LITTLE_ENDIAN)
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

        class Path;
        typedef std::shared_ptr<Path> PathPtr;
        class File;
        typedef std::shared_ptr<File> FilePtr;
        // Path class represents a path in the file system. It inherits from fs::path adding implicit conversion to std::string.
        class Path : public fs::path, public Object
		{
			public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                typedef Path   I;  // Interface type.
                typedef Path IRO; // Read only interface type.

                Path (const fs::path& p = ".", const Id& aid = NULL_ID) : Object(aid), fs::path(p) {}
			    Path (const std::string& s = ".", const Id& aid = NULL_ID) : Object(aid), fs::path (s) {}
			    Path (const char* s = ".", const Id& aid = NULL_ID) : Object(aid), fs::path (s) {}

                // Copy constructor.
                Path (const Path& p, const Id& aid = NULL_ID) noexcept : Object(aid), fs::path (p) {}

                // Move constructor.
                Path (Path&& p, const Id& aid = NULL_ID) noexcept : Object (aid), fs::path (p) {}

                // virtual destructor.
                virtual ~Path() noexcept = default;

                bool remove () const noexcept
                {
                    if (exists())
                    {
						return fs::remove (*this);
					}
					return true;
				}

				bool exists () const noexcept
				{
					return fs::exists (*this);
				}

				bool is_directory () const noexcept
				{
					return fs::is_directory (*this);
				}

                bool is_regular_file() const noexcept
                {
					return fs::is_regular_file (*this);
				}

				bool is_symlink () const noexcept
				{
					return fs::is_symlink (*this);
				}

                bool is_empty() const noexcept
                {
					return fs::is_empty (*this);
				}

				/*
                bool is_absolute() const noexcept
				{
					return fs::is_absolute (*this);
				}

				bool is_relative () const noexcept
				{
					return fs::is_relative (*this);
				}
                */ // todo.

				Path absolute () const noexcept
				{
					return fs::absolute (*this);
				}

				Path canonical () const noexcept
				{
					return fs::canonical (*this);
				}

				Path relative () const noexcept
				{
					return fs::relative (*this);
				}

				Path parent_path () const noexcept
				{
					return fs::path::parent_path ();
				}

				Path filename () const noexcept
				{
					return fs::path::filename ();
				}

				Path stem () const noexcept
				{
					return fs::path::stem ();
				}

                Path extension() const noexcept
                {
					return fs::path::extension ();
				}

				Path& replace_extension (const std::string& s) noexcept
				{
					fs::path::replace_extension (s);
					return *this;
				}

				Path& replace_extension (const Path& p) noexcept
				{
					fs::path::replace_extension (p);
					return *this;
				}

				Path& make_preferred () noexcept
				{
					fs::path::make_preferred ();
					return *this;
				}

				Path& remove_filename () noexcept
				{
					fs::path::remove_filename ();
					return *this;
				}

				Path& remove_trailing_separator () noexcept
				{
					//fs::path::remove_trailing_separator (); // todo.
					return *this;
				}
                
                // Implements initialize method from Initializable concept.
                virtual bool initialize (const fs::path& p, const Id& aid = NULL_ID) noexcept
                {
                    fs::path::operator = (p);
                    Object::set_id(aid);
                    return true;
                }

                PathPtr clone () const  noexcept { return pd::clone<Path>(*this, static_cast<const fs::path&>(*this), get_id()); }

                // Conversion to json string.
                virtual String json () const noexcept
                {
					std::stringstream ss (pd::json<Path> (*this));
                    ss << ",path:" << this->fs::path::string() << "}";
                    return ss.str ();
				}   

                virtual std::istream& read (std::istream& is, const IO_Mode amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN)
                {
                    if (amode == BINARY)
                    {
                        // todo: implement binary read.
                    }
                    else // json format
                    {
                        Json j;
                        VersionPtr v;
                        Id stream_id;
                        pd::read_json<Path>(is, *this, &stream_id, &v, &j);
                        set_id (stream_id);
                        *this = j["path"].get<std::string>();

                        if (*VERSION != *v) throw std::runtime_error ("Version mismatch.");
                    }
                    return is;
                };

                virtual std::ostream& write (std::ostream& os, const IO_Mode amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) const
                {
                    if (amode == BINARY)
                    {
                        // todo: implement binary read.
                    }
                    else // json format
                    {
                        os << json ();
                    }
                    return os;
                };

                // Convertion to xml string.
                virtual String xml () const noexcept
                {
                    String xml = ObjXMLPrefix () + "><path>";
                    xml += fs::path::string() + "</path>";
                    xml += "</object>";
                    return xml;
                }

                // Convertion from xml string.
                virtual void from_xml(const String& sxml)
                {
                    XMLNode node = parse_object_tag(sxml);
                    XMLNode n = node.getChildNode("path");
                    if (!n.isEmpty()) *this = n.getText();
                }
                
                virtual String debug_string() const noexcept
                {
                    return Object::debug_string() + " path = " + fs::path::string ();
                }

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

                inline fs::path std_path () const noexcept { return static_cast<const fs::path&>(*this); }  

                // Implicit conversion to fs::path.
                operator fs::path () const noexcept { return std_path (); }
                
                // Implicit conversion to std::string.
                operator std::string () const noexcept { return fs::path::string (); }

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
                friend void to_json(Json& j, const Path& d);
                friend void from_json(const Json& j, Path& d);

                // Inherited via Object
                inline void set_id(const Id& value) { Object::set_id (value); }
};  // class Path


        /// \brief File class.
        ///
        class File: public Object
        {
            protected:
                typedef std::vector<char> Buffer;
                Buffer binary_data;
                String text_data;
                Path full_path;
                std::ios_base::openmode mode;
                std::fstream file;

                template <typename T = const String&>
                File& _append(const T& content)
                {
                    if (is_open())
                    {
                        file.seekg(0, std::ios::end);
                    }
                    else
                    {
                        if (full_path.exists())
                        {
                            if (full_path.has_filename())
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
                            full_path.create_dir_if_does_not_exist();
                            mode |= std::ios::out;
                        }
                        file.open(static_cast<fs::path>(full_path), mode);
                    };
                    file << content;
                    close();
                    return *this;
                }

            public:
                typedef File   I;  // Interface type.
                typedef File IRO; // Read only interface type.

                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                inline static const size_t MAX_IN_MEMORY_FILE_SIZE_BYTE = 1024 ^ 3; // 1 GB
                inline const static std::ios_base::openmode INPUT               = std::ios::in;
                inline const static std::ios_base::openmode OUTPUT              = std::ios::out;
                inline const static std::ios_base::openmode IN_OUT_MODE         = std::ios::in | std::ios::out;
                inline const static std::ios_base::openmode IN_OUT              = std::ios::out;
                inline const static std::ios_base::openmode IN_OUT_ATE_MODE     = std::ios::in | std::ios::out | std::ios::ate;
                inline const static std::ios_base::openmode IN_OUT_ATE_BIN_MODE = IN_OUT_ATE_MODE | std::ios::binary;
                File (const Path& afull_path = ".", const std::ios_base::openmode amode = IN_OUT_ATE_BIN_MODE, const Id aid = NULL_ID) : 
                    Object(aid), full_path(afull_path), mode(amode)
                {
                    initialize (afull_path, amode, aid);
                }  
            
                virtual ~File () noexcept { close (); }

                void close () noexcept { if (is_open ()) file.close(); }
            
                bool exists () const { return full_path.exists(); }

                void open (const Path& afull_path = "", const std::ios_base::openmode amode = 0)
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

                // Implements initialize method from Initializable concept.
                virtual bool initialize(const Path& afull_path, const std::ios_base::openmode amode = IN_OUT_ATE_BIN_MODE, const Id aid = NULL_ID) noexcept
                {
                    binary_data = Buffer();
                    full_path = afull_path;
                    mode = amode;
                    Object::set_id(aid);
                    if (full_path.exists())
                    {
                        if (full_path.has_filename())
                        {
                            size_t file_size = fs::file_size(full_path);
                            if ((file_size < MAX_IN_MEMORY_FILE_SIZE_BYTE) && (file_size < get_available_memory()))
                            {
                                std::ifstream file(static_cast<fs::path>(full_path), mode);
                                if (file.is_open())
                                {
                                    if (is_binary())
                                    {
                                        file.seekg(0, std::ios::end);
                                        size_t size = file.tellg();
                                        file.seekg(0, std::ios::beg);
                                        binary_data.resize(size);
                                        file.read(binary_data.data(), size);
                                        close();
                                    }
                                    else
                                    {
										std::stringstream buffer;
										buffer << file.rdbuf();
										text_data = buffer.str();
										close();
									}
                                }
                                else
                                {
                                    // File exists but could not be opened.
                                    // todo: log ("Could not open file " + full_path.string());
                                    return false;
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
                            // todo: log ("Path " + full_path.string() + " does not have a filename.");
                            return false;
                        }
                    }
                    else
                    {
                        // File does not exist.
                        full_path.create_dir_if_does_not_exist();
                        mode |= std::ios::out;
                        open(full_path, mode);
                        close();
                    }
                    return true;
                }

                FilePtr clone() const  noexcept { return pd::clone<File>(*this, full_path, mode, get_id()); }

                // Conversion to json string.
                virtual String json() const noexcept
                {
                    std::stringstream ss;
                    String s = pd::json<File>(*this);
                    ss << ", \"full_path\" : " << full_path.to_string () << " , \"mode\" : " << mode;
                    if (is_binary ())
					{
                        String s;
                        pd::binary_to_string<decltype(binary_data)::value_type, String::value_type>(binary_data, s);
                        ss << " , \"binary_data\" : " << s;
					}
					else
					{
						ss << " , \"text_data\" : \"" << text_data;
					}
                    ss << "\" }";
                    return ss.str();
                }

                virtual std::ios_base::openmode get_mode() const noexcept
                {
                    return mode;
                }

                virtual std::istream& read (std::istream& is, const IO_Mode amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN)
                {
                    if (amode == BINARY)
                    {
                        // todo: implement binary read.
                    }
                    else // json format
                    {
                        Json j;
                        VersionPtr v;
                        Id id;
                        read_json<File>(is, *this, &id, &v, &j);
                        set_id (id);
                        full_path = j["full_path"].get<String>();
						mode = j["mode"].get<std::ios_base::openmode>();
						if (is_binary())
						{
							String s = j["binary_data"].get<String>();
							//pd::string_to_binary<String::value_type, decltype(binary_data)::value_type>(s, binary_data);
						}
						else
						{
							text_data = j["text_data"].get<String>();
						}
                    }
                    return is;
                };

                virtual std::ostream& write (std::ostream& os, const IO_Mode amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) const
                {
                    if (amode == BINARY)
                    {
                        // todo: implement binary read.
                    }
                    else // json format
                    {
                        os << json ();
                    }
                    return os;
                };

                // Convertion to xml string.
                virtual String xml() const noexcept
                {
                    String xml = ObjXMLPrefix() + "><path>";
                    xml += full_path.string() + "</path><mode>";
                    xml += std::to_string(mode) + "</mode>";
                    // Check if buffer is not empty.
                    if (!binary_data.empty())
                    {
                        // Check if buffer is binary.
                        if (is_binary())
						{
							xml += "<buffer type=\"binary\">";
                            String s;
                            xml += pd::binary_to_string<decltype(binary_data)::value_type, String::value_type> (binary_data, s);  
						}
						else
						{
							xml += "<buffer type=\"text\">";
                            xml += String(binary_data.data(), binary_data.size());    
						}
                        xml += "</buffer>";
					}
                    xml += "</object>";
                    return xml;
                }

                // Convertion from xml string.
                virtual void from_xml(const String& sxml)
                {
                    XMLNode node = parse_object_tag(sxml);
                    XMLNode n = node.getChildNode("path");
                    if (!n.isEmpty()) this->full_path = n.getText();
                    n = node.getChildNode("mode");  
                    if (!n.isEmpty()) this->mode = std::stoi(n.getText());
                    n = node.getChildNode("buffer");
                    if (!n.isEmpty())
                    {
						String type = n.getAttribute("type");
                        if (type == "binary")
                        {
							String s = n.getText();
                            pd::binary_to_string<decltype(binary_data)::value_type, String::value_type>(binary_data, s);
						}
						else
						{
							text_data = n.getText();
						}
					}
                }

                virtual String debug_string() const noexcept
                {
                    return Object::debug_string() + " path = " + full_path.to_string ();
                }

               inline File& set_binary_mode() noexcept
                {
					mode |= std::ios::binary;
					return *this;
				}

                inline File& set_text_mode() noexcept
                {
                    mode &= ~std::ios::binary;
                    return *this;
                }

                // Opens the file if necessary and returns the file stream.
                inline std::fstream& fstream()
                {
                    if (!is_open())
                    {
						file.open (static_cast<fs::path>(full_path), mode);
					}
					return file;
				}

                //friend void to_json   (      Json& j, const File& f);
                //friend void from_json (const Json& j,       File& f);

                // Inherited via File
                fs::path get_full_path () const noexcept;

                // Inherited via Object
                void set_id (const Id& value);

                // Inherited via File
                void set_name (const String& name) noexcept;
};  // class File   

        class TextFile : public File
        {
			private:
			public:
            TextFile(const Path& full_path, const std::ios_base::openmode amode = IN_OUT_ATE_MODE, const String& content = "", const Id aid = NULL_ID) : File(full_path, (amode & (~std::ios::binary)), aid)
            { 
                append(content);
            }
            TextFile(const Path& full_path, const String& content = "", const Id aid = NULL_ID) : TextFile(full_path, IN_OUT_ATE_MODE, content, aid)
            {
			}
            
            virtual ~TextFile () { close (); }

            File& append (const String& content)
            {
               return _append<String>(content);
			} 

            String to_string() const noexcept
			{
				return text_data;
			}

            // Implicit conversion to string.
            operator String() const noexcept
			{
				return to_string();
			}
		};

        class BinaryFile : public File
		{   
		};  

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

        // Streaming operators.
        extern std::istream& operator >> (std::istream& is,       Path& path);
        extern std::ostream& operator << (std::ostream& os, const Path& path);
        extern std::istream& operator >> (std::istream& is,       File& file);
        extern std::ostream& operator << (std::ostream& os, const File& file);

        // Json conversion.
        //extern void to_json   (      Json& j, const Path& p);
        //extern void from_json (const Json& j,       Path& p);
        //extern void to_json   (      Json& j, const File& f);
        //extern void from_json (const Json& j,       File& f);

    }   // namespace cpplib
}       // namespace pensar_digital
#endif
