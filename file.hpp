// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef FILE_HPP
#define FILE_HPP

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "string_def.hpp"
#include "string_types.hpp"
#include "system.hpp"
#include "object.hpp"
#include "io_util.hpp"
#include "path.hpp"
#include "version.hpp"  
#include "generator.hpp"
#include "clone_util.hpp"
#include "memory_buffer.hpp"
#include "random_util.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>   // shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        namespace fs = std::filesystem;

        /// \brief File class.
        ///
        template <typename C = char>
        class File : public Object
        {
            public:
                inline static const size_t MAX_BUFFER_SIZE = 1024 ^ 3; // 1 GB
                inline const static std::ios_base::openmode INPUT = std::ios::in;
                inline const static std::ios_base::openmode OUTPUT = std::ios::out;
                inline const static std::ios_base::openmode IN_OUT_ATE_APP_MODE = std::ios::in | std::ios::out | std::ios::ate | std::ios::app;
			    inline const static std::ios_base::openmode OUT_TRUNC_MODE = std::ios::out | std::ios::trunc;
                inline const static std::ios_base::openmode DEFAULT_MODE = IN_OUT_ATE_APP_MODE;

        protected:
                std::ios_base::openmode mmode; // Last mode used to open the file.
                Path mfullpath;                // Full path of the file. 
                typedef std::basic_fstream<C> FStream;
                FStream* stream_ptr; 
        public:
            
            File(const Path& full_path = L".", const Id id = NULL_ID, const std::ios_base::openmode mode = DEFAULT_MODE) :
                mfullpath(full_path), Object(id), mmode(mode)
            {
                mfullpath.create_dir (); // Create the directory if it does not exist.
                if (!mfullpath.exists())
                {
                    stream_ptr = new FStream(mfullpath.cstr(), OUT_TRUNC_MODE);
                    stream_ptr->close();
                    stream_ptr->open(mfullpath.std_path(), mmode);
                }
                else
                {
                    stream_ptr = new FStream(full_path.std_path(), mmode);
                }
            }

            virtual ~File() noexcept 
            { 
                close(); 
                delete stream_ptr;
            }

            void close() noexcept { if (is_open()) stream_ptr->close(); }

            bool exists() const { return mfullpath.exists(); }

            const Path& fullpath() const noexcept { return mfullpath; }

            bool is_open() const noexcept { return stream_ptr->is_open(); }
            bool is_good() const noexcept { return stream_ptr->good(); }
            bool is_app() const noexcept { return (mmode & std::ios::app) != 0; }
            bool is_ate() const noexcept { return (mmode & std::ios::ate) != 0; }
            bool is_auto_seek_end_before_write() const noexcept { return is_app(); }
            bool is_auto_seek_end_on_open() const noexcept { return is_ate(); }
            bool is_binary() const noexcept { return (mmode & std::ios::binary) != 0; }
            bool is_in() const noexcept { return (mmode & std::ios::in) != 0; }
            bool is_out() const noexcept { return (mmode & std::ios::out) != 0; }
            bool is_trunc() const noexcept { return (mmode & std::ios::trunc) != 0; }
            bool eof() const noexcept { return stream_ptr->eof(); }
            bool fail() const noexcept { return stream_ptr->fail(); }
            bool bad() const noexcept { return stream_ptr->bad(); }
            bool good() const noexcept { return stream_ptr->good(); }

            char* c_str() const noexcept { return (char*)mfullpath.c_str(); }

            // Delete the file.
            bool remove() 
            { 
                if (is_open()) 
                    close();
                std::string s = mfullpath.string();
               
                return (std::remove(s.c_str ()) == 0);
            }

                

            // Implements initialize method from Initializable concept.
            virtual bool initialize(const Path& afull_path = CURRENT_DIR<C>, const Id id = NULL_ID, const std::ios_base::openmode mode = DEFAULT_MODE) noexcept
            {
                mfullpath = afull_path;
                mmode = mode;
                Object::set_id(id);
                return true;
            }

            //FilePtr clone() const  noexcept { return pd::clone<File>(*this, mfullpath, mmode, id()); }

            // Conversion to json string.
            virtual String json() const noexcept
            {
                std::stringstream ss;
                String s = pd::json<File>(*this);
                ss << ", \"mfullpath\" : " << mfullpath.to_string() << " , \"mode\" : " << mmode;
                ss << "\" }";
                return ss.str();
            }

            virtual std::ios_base::openmode get_mode() const noexcept
            {
                return mmode;
            }

            virtual std::istream& read_json(std::istream& is)
            {
                {
                    Json j;
                    VersionPtr v;
                    Id id;
                    pd::read_json<File<C>>(is, *this, &id, &v, &j);
                    set_id(id);
                    mfullpath = j["mfullpath"].get<String>();
                    mmode = j["mode"].get<std::ios_base::openmode>();
                }
                return is;
            };

            // Convertion to xml string.
            virtual String xml() const noexcept
            {
                String xml = ObjXMLPrefix() + "><path>";
                xml += mfullpath.string() + "</path><mode>";
                xml += std::to_string(mmode) + "</mode>";
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual void from_xml(const String& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                XMLNode n = node.getChildNode("path");
                if (!n.isEmpty()) 
                    this->mfullpath = n.getText();
                n = node.getChildNode("mode");
                if (!n.isEmpty()) 
                    this->mmode = std::stoi(n.getText());
            }

            virtual String debug_string() const noexcept
            {
                return Object::debug_string() + " path = " + mfullpath.to_string();
            }

            inline File& set_binary_mode() noexcept
            {
                mmode |= std::ios::binary;
                return *this;
            }

            inline File& set_text_mode() noexcept
            {
                mmode &= ~std::ios::binary;
                return *this;
            }

            // Opens the file if necessary and returns the file stream.
            inline FStream& open () const
            {
                if (!is_open())
                {
                    stream_ptr->open (mfullpath, mmode);
                }
                return *stream_ptr;
            }

            //friend void to_json   (      Json& j, const File& f);
            friend void from_json (const Json& j,       File& f);

        };  // class File   

        template <typename C = char>
        class TextFile : public File<C>
        {
            private:
            public:
                typedef std::basic_string<C> String;
                inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                TextFile(const Path& full_path, const String& content = EMPTY<C>, 
                         const std::ios_base::openmode mode = File<C>::DEFAULT_MODE, const Id id = NULL_ID) : File<C>(full_path, id, (mode& (~std::ios::binary)))
                {
                    if (File<C>::exists ())
                        append (content);
                    else
                        write (content);
                    File<C>::close ();
                }

                //TextFile(const Path& full_path, const String& content = EMPTY<C>, const Id aid = NULL_ID) :
                //    TextFile(full_path, File<C>::DEFAULT_MODE, content, id)
                //{
                //}

                virtual ~TextFile() = default;
                TextFile<C>& write(const String& content)
                {
					// Writes content.
					if (! File<C>::is_open()) 
						File<C>::open ();

					File<C>::stream_ptr->seekp(0, std::ios::beg);
                    *(File<C>::stream_ptr) << content;
					return *this;
                }
                TextFile<C>& append (const String& content)
                {
                    // Appends content.
                    if (! File<C>::is_open()) 
                        File<C>::open ();

                    File<C>::stream_ptr->seekp(0, std::ios::end);
				    *(File<C>::stream_ptr) << content;
                    return *this;
                }

                String to_string() const noexcept
                {
                    return File<C>::mfullpath.to_string ();
                }

                // Implicit conversion to string.
                operator String() const noexcept
                {
                    return to_string();
                }

                // Reads the file content and returns it as a std::basic_string<C>.
                String read() const
                {
					if (! File<C>::is_open()) 
						File<C>::open ();

					File<C>::stream_ptr->seekg(0, std::ios::end);
					size_t size = File<C>::stream_ptr->tellg();
					String content(size, 0);
					File<C>::stream_ptr->seekg(0, std::ios::beg);
					File<C>::stream_ptr->read(&content[0], size);
					return content;
				}
        };
        
        // Random file file name generator.
        
        template <typename C = char>
        class RandomFileNameGenerator
        {
			private:
				typedef std::basic_string<C> String;

				inline static const size_t DEFAULT_LENGTH                =     8;
				inline static const String      DEFAULT_TEXT_FILE_EXTENSION   = "txt";
                inline static const String      DEFAULT_BINARY_FILE_EXTENSION = "bin";
                
                // Extension type. It can be fixed, random, none, numeric sequence, C char sequence, or a function.  
                enum class ExtensionType { FIXED, RANDOM, NONE, NUMERIC_SEQUENCE, CHAR_SEQUENCE, FUNCTION };
                inline static const ExtensionType DEFAULT_EXTENSION_TYPE = ExtensionType::FIXED;

                // Determines the function signature as a typedef. It is named custom_exension_generator and it should return a string and take no arguments.
                typedef std::function<String()> CUSTOM_EXT_FUNCTION;
                // Defines a NULL_CUSTOM_EXT_FUNCTION.  That should always return an empty string.
                inline static const CUSTOM_EXT_FUNCTION NULL_CUSTOM_EXT_FUNCTION = []() { return EMPTY<C>; };

				// Extension type.
                ExtensionType mextension_type;

                // Extension.
                String mextension;

                inline static const size_t MAX_FULL_NAME_LENGTH = 255;
                

                // Numeric sequence.    
                typedef Generator<RandomFileNameGenerator, size_t> G;
                G generator;

                // Char sequence.
                // todo: CharGenerator<C> char_generator;

                // Custom extension function.
                CUSTOM_EXT_FUNCTION mcustom_ext_function;

                // Generates a random string of length len.
                inline static String random_string(const size_t len)
                {
					static const String CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
                    Random<size_t> r(0, CHARS.size() - 1);

					String s;
                    size_t i = len;
					s.reserve(i);
                    typename String::value_type c = CHARS[r()];
                    
                    size_t count = 1000; // Max count is 1000. If c is still a number after 1000 tries, then throws an exception.
                    // while c is a number, try again.
                    while (c >= '0' && c <= '9') 
                    {
                        c = CHARS[r()];
                        if (--count == 0)
                        {
                            // Logs error and throws an exception.
                            String error_msg = "RandomFileNameGenerator::random_string(): Error: Could not generate a random string after 1000 tries.";
                            LOG(error_msg);
                            throw std::runtime_error(error_msg);
                        }
							
                    }
					s += c;
                    --i;
					while (i--)
						s += CHARS[r ()];

					return s;
				}

                String get_extension() const
                {
                    switch (mextension_type)
                    {
						case ExtensionType::FIXED:
							return mextension;
						case ExtensionType::RANDOM:
							return random_string(DEFAULT_LENGTH);
						case ExtensionType::NONE:
							return EMPTY<C>;
						//case ExtensionType::NUMERIC_SEQUENCE:
							//return std::to_string((const G::IdType)generator.get_id());
						case ExtensionType::CHAR_SEQUENCE:
							return EMPTY<C>; // todo: char_generator();
						case ExtensionType::FUNCTION:
							return mcustom_ext_function();
						default:
							return EMPTY<C>;
					}
				}   

                    
            public:
            // Constructor.
            RandomFileNameGenerator (const ExtensionType ext_type = DEFAULT_EXTENSION_TYPE, const String& extension = DEFAULT_TEXT_FILE_EXTENSION, const CUSTOM_EXT_FUNCTION& custom_ext_function = NULL_CUSTOM_EXT_FUNCTION) :
                mextension_type(ext_type), mextension(extension), mcustom_ext_function(custom_ext_function)
            {
			}

            // Generates a random text file name. () operator.
            inline Path operator() (const String& name_prefix = "", const String& name_suffix = "", const String& extension = DEFAULT_TEXT_FILE_EXTENSION, const Path& path = TMP_DIR<char>)
            {
                return path / (name_prefix + random_string(DEFAULT_LENGTH) + name_suffix + "." + get_extension());
            }
        };

        // TmpTextFile is a temporary text file.
        template <typename C = char>
        class TmpTextFile : public TextFile<C>
        {
			public:
				inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                TmpTextFile(const std::basic_string<C>& file_name = EMPTY<C>, const std::basic_string<C>& content = EMPTY<C>, const Id id = null_value<Id>()) : TextFile<C>(TMP_PATH / file_name, content, File<C>::DEFAULT_MODE, id)
                {
				}

                void log_error()
                {
                    LOG("Failed to remove temporary file " + File<char>::mfullpath.string<char>());
                }

                void wlog_error()
                {
                    Path p = File<wchar_t>::mfullpath;
                    WLOG(L"Failed to remove temporary file " + p.wstring());
                }

                template <typename Char>
                void log_err()
				{
					LOG("Failed to remove temporary file " + File<Char>::mfullpath.string<Char>());
				}

                template <>
                void log_err<wchar_t>()
				{
                    wlog_error();
                }
                virtual ~TmpTextFile()
                {
                    // Removes the file. If it is open, it is closed first. If operation fails, a log message is generated.
                    if (!File<C>::remove())
                    {
                        log_err<C>();
                    }
				}
		};  // class TmpTextFile

        class BinaryFile : public File<char>
        {
			private:
			public:
				inline static const VersionPtr VERSION = Version::get(1, 1, 1);
			private:
                BinaryFile (const Path&                   full_path, 
                            const std::ios_base::openmode mode      = DEFAULT_MODE, 
                            const BytePtr                 data      = nullptr, 
                            const size_t                  size      = 0,
                            const Id                      id        = null_value<Id>()
                            ) : File (full_path, id, (mode | std::ios::binary))
                {
					append (data, size);
				}

                BinaryFile (const Path& full_path, const BytePtr data = nullptr, const size_t size = 0, const Id id = null_value<Id>()) : BinaryFile(full_path, DEFAULT_MODE, data, size, id)
                {
				}

				virtual ~BinaryFile() { close(); }

                BinaryFile& append(const BytePtr data = nullptr, const size_t size = 0)
                {
                    static_assert (sizeof(char) == sizeof(std::byte), "sizeof(char) != sizeof(Byte)");
                    if ((data != nullptr) && (size > 0))
					{
						if (! is_open()) 
							open ();

						stream_ptr->seekp(0, std::ios::end);
						stream_ptr->write(reinterpret_cast<const char*>(data), size);
					}
					return *this;
                }
			private:
                MemoryBuffer  data;
        };
        // Streaming operators.
        extern std::istream& operator >> (std::istream& is, File<char>& file);
        extern std::ostream& operator << (std::ostream& os, const File<char>& file);
        extern std::wistream& operator >> (std::wistream& is, File<wchar_t>& file);
        extern std::wostream& operator << (std::wostream& os, const File<wchar_t>& file);

        // Json conversion.
        extern void to_json   (      Json& j, const File<char>& f);
        extern void from_json (const Json& j,       File<char>& f);


    } // namespace cpplib
} // namespace pensar_digital.

#endif // FILE_HPP