// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef FILE_HPP
#define FILE_HPP

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "s.hpp"
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
#include "log.hpp"

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
            
            inline File(const Path& full_path = W("."), const Id id = NULL_ID, const std::ios_base::openmode mode = DEFAULT_MODE) :
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

            inline virtual ~File() noexcept
            { 
                close(); 
                delete stream_ptr;
            }

            inline void close() noexcept { if (is_open()) stream_ptr->close(); }

            inline bool exists() const { return mfullpath.exists(); }

            inline const Path& fullpath() const noexcept { return mfullpath; }

            inline bool is_open() const noexcept { return stream_ptr->is_open(); }
            inline bool is_good() const noexcept { return stream_ptr->good(); }
            inline bool is_app() const noexcept { return (mmode & std::ios::app) != 0; }
            inline bool is_ate() const noexcept { return (mmode & std::ios::ate) != 0; }
            inline bool is_auto_seek_end_before_write() const noexcept { return is_app(); }
            inline bool is_auto_seek_end_on_open() const noexcept { return is_ate(); }
            inline bool is_binary() const noexcept { return (mmode & std::ios::binary) != 0; }
            inline bool is_in() const noexcept { return (mmode & std::ios::in) != 0; }
            inline bool is_out() const noexcept { return (mmode & std::ios::out) != 0; }
            inline bool is_trunc() const noexcept { return (mmode & std::ios::trunc) != 0; }
            inline bool eof() const noexcept { return stream_ptr->eof(); }
            inline bool fail() const noexcept { return stream_ptr->fail(); }
            inline bool bad() const noexcept { return stream_ptr->bad(); }
            inline bool good() const noexcept { return stream_ptr->good(); }

            inline const char* c_str() const noexcept 
            { S s = mfullpath.string().c_str(); // returns safely the null terminated string.
            }

            // Delete the file.
            inline bool remove() 
            { 
                if (is_open()) 
                    close();
                std::string s = mfullpath.string();
               
                return (std::remove(s.c_str ()) == 0);
            }

                

            // Implements initialize method from Initializable concept.
            inline virtual bool initialize(const Path& afull_path = CURRENT_DIR, const Id id = NULL_ID, const std::ios_base::openmode mode = DEFAULT_MODE) noexcept
            {
                mfullpath = afull_path;
                mmode = mode;
                Object::set_id(id);
                return true;
            }

            //FilePtr clone() const  noexcept { return pd::clone<File>(*this, mfullpath, mmode, id()); }

            inline virtual std::ios_base::openmode get_mode() const noexcept
            {
                return mmode;
            }

            inline virtual S debug_string() const noexcept
            {
                return Object::debug_string() + W(" path = ") + mfullpath.string<C>();
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

        class TextFile : public File
        {
            private:
            public:
                inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                TextFile(const Path& full_path, const S& content = EMPTY, 
                         const std::ios_base::openmode mode = File::DEFAULT_MODE, const Id id = NULL_ID) : File(full_path, id, (mode& (~std::ios::binary)))
                {
                    if (File::exists ())
                        append (content);
                    else
                        write (content);
                    File::close ();
                }

                //TextFile(const Path& full_path, const S& content = empty<C> (), const Id aid = NULL_ID) :
                //    TextFile(full_path, File::DEFAULT_MODE, content, id)
                //{
                //}

                inline virtual ~TextFile() = default;
                inline TextFile& write(const S& content)
                {
					// Writes content.
					if (! File::is_open()) 
						File::open ();

					File::stream_ptr->seekp(0, std::ios::beg);
                    *(File::stream_ptr) << content;
					return *this;
                }
                inline TextFile& append (const S& content)
                {
                    // Appends content.
                    if (! File::is_open()) 
                        File::open ();

                    File::stream_ptr->seekp(0, std::ios::end);
				    *(File::stream_ptr) << content;
                    return *this;
                }

                FStream& get_stream() 
				{
					return File::open();
				}

                inline S to_string() const noexcept
                {
                    return File::mfullpath.to_string ();
                }

                // Implicit conversion to string.
                inline operator S() const noexcept
                {
                    return to_string();
                }

                // Reads the file content and returns it as a std::basic_string<C>.
                inline S read() const
                {
					if (! File::is_open()) 
						File::open ();

					File::stream_ptr->seekg(0, std::ios::end);
					size_t size = File::stream_ptr->tellg();
					S content(size, 0);
					File::stream_ptr->seekg(0, std::ios::beg);
					File::stream_ptr->read(&content[0], size);
					return content;
				}
        };
        
        // Random file file name generator.
        
        class RandomFileNameGenerator
        {
			private:

				inline static const size_t DEFAULT_LENGTH                =     8;
				inline static const S      DEFAULT_TEXT_FILE_EXTENSION   = "txt";
                inline static const S      DEFAULT_BINARY_FILE_EXTENSION = "bin";
                
                // Extension type. It can be fixed, random, none, numeric sequence, C char sequence, or a function.  
                enum class ExtensionType { FIXED, RANDOM, NONE, NUMERIC_SEQUENCE, CHAR_SEQUENCE, FUNCTION };
                inline static const ExtensionType DEFAULT_EXTENSION_TYPE = ExtensionType::FIXED;

                // Determines the function signature as a typedef. It is named custom_exension_generator and it should return a string and take no arguments.
                typedef std::function<S()> CUSTOM_EXT_FUNCTION;
                // Defines a NULL_CUSTOM_EXT_FUNCTION.  That should always return an empty string.
                inline static const CUSTOM_EXT_FUNCTION NULL_CUSTOM_EXT_FUNCTION = []() { return EMPTY; };

				// Extension type.
                ExtensionType mextension_type;

                // Extension.
                S mextension;

                inline static const size_t MAX_FULL_NAME_LENGTH = 255;
                

                // Numeric sequence.    
                typedef Generator<RandomFileNameGenerator, size_t> G;
                G generator;

                // Char sequence.
                // todo: CharGenerator<C> char_generator;

                // Custom extension function.
                CUSTOM_EXT_FUNCTION mcustom_ext_function;

                // Generates a random string of length len.
                inline static S random_string(const size_t len)
                {
					static const S CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
                    Random<size_t> r(0, CHARS.size() - 1);

					S s;
                    size_t i = len;
					s.reserve(i);
                    typename S::value_type c = CHARS[r()];
                    
                    size_t count = 1000; // Max count is 1000. If c is still a number after 1000 tries, then throws an exception.
                    // while c is a number, try again.
                    while (c >= '0' && c <= '9') 
                    {
                        c = CHARS[r()];
                        if (--count == 0)
                        {
                            // Logs error and throws an exception.
                            S error_msg = W("RandomFileNameGenerator::random_string(): Error: Could not generate a random string after 1000 tries.");
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

                inline S get_extension() const
                {
                    switch (mextension_type)
                    {
						case ExtensionType::FIXED:
							return mextension;
						case ExtensionType::RANDOM:
							return random_string(DEFAULT_LENGTH);
						case ExtensionType::NONE:
							return EMPTY;
						//case ExtensionType::NUMERIC_SEQUENCE:
							//return std::to_string((const G::IdType)generator.get_id());
						case ExtensionType::CHAR_SEQUENCE:
							return EMPTY; // todo: char_generator();
						case ExtensionType::FUNCTION:
							return mcustom_ext_function();
						default:
							return EMPTY;
					}
				}   

                    
            public:
            // Constructor.
                inline RandomFileNameGenerator (const ExtensionType ext_type = DEFAULT_EXTENSION_TYPE, const S& extension = DEFAULT_TEXT_FILE_EXTENSION, const CUSTOM_EXT_FUNCTION& custom_ext_function = NULL_CUSTOM_EXT_FUNCTION) :
                mextension_type(ext_type), mextension(extension), mcustom_ext_function(custom_ext_function)
            {
			}

            // Generates a random text file name. () operator.
            inline Path operator() (const S& name_prefix = "", const S& name_suffix = "", const S& extension = DEFAULT_TEXT_FILE_EXTENSION, const Path& path = TMP_DIR)
            {
                return path / (name_prefix + random_string(DEFAULT_LENGTH) + name_suffix + W(".") + get_extension());
            }
        };

        // TmpTextFile is a temporary text file.
        class TmpTextFile : public TextFile
        {
			public:
				inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                inline TmpTextFile (const S& file_name = EMPTY, const S& content = EMPTY, const Id id = null_value<Id>()) : TextFile(TMP_PATH / file_name, content, File::DEFAULT_MODE, id)
                {
				}

                inline TmpTextFile (const C* file_name = EMPTY, const C* content = EMPTY, const Id id = null_value<Id>())
                    : TextFile (S(file_name), S(content), File::DEFAULT_MODE, id) {}

                inline void log_error()
                {
                    LOG(W("Failed to remove temporary file ") + File::mfullpath.s ());
                }

                inline void wlog_error()
                {
                    Path p = File::mfullpath;
                    WLOG(L"Failed to remove temporary file " + p.wstring());
                }

                template <typename Char>
                void log_err()
				{
					LOG("Failed to remove temporary file " + File::mfullpath.string());
				}

                template <>
                void log_err<wchar_t>()
				{
                    wlog_error();
                }
                inline virtual ~TmpTextFile()
                {
                    // Removes the file. If it is open, it is closed first. If operation fails, a log message is generated.
                    if (!File::remove())
                    {
                        log_err<C>();
                    }
				}
		};  // class TmpTextFile

        class BinaryFile : public File
        {
			private:
			public:
				inline static const VersionPtr VERSION = Version::get(1, 1, 1);
			private:
                inline BinaryFile (const Path&                   full_path,
                            const std::ios_base::openmode mode      = DEFAULT_MODE, 
                            const BytePtr                 data      = nullptr, 
                            const size_t                  size      = 0,
                            const Id                      id        = null_value<Id>()
                            ) : File (full_path, id, (mode | std::ios::binary))
                {
					append (data, size);
				}

                inline BinaryFile (const Path& full_path, const BytePtr data = nullptr, const size_t size = 0, const Id id = null_value<Id>()) : BinaryFile(full_path, DEFAULT_MODE, data, size, id)
                {
				}

                inline virtual ~BinaryFile() { close(); }

                inline BinaryFile& append(const BytePtr data = nullptr, const size_t size = 0)
                {
                    static_assert (sizeof(char) == sizeof(std::byte), W("sizeof(char) != sizeof(Byte)"));
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
        extern InStream&  operator >> (InStream&  is,       File& file);
        extern OutStream& operator << (OutStream& os, const File& file);
    } // namespace cpplib
} // namespace pensar_digital.

#endif // FILE_HPP