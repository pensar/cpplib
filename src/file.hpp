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
#include "error.hpp"
#include "icu_util.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>   // shared_ptr
#include <stdio.h> // fopen_s, _wfopen_s
#include <errno.h>
#include <cerrno>

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        namespace fs = std::filesystem;

        class OpenMode : public S 
        {
            public:
                OpenMode (const S& mode) : S (mode) 
			    {
				    if (mode.empty())
					    throw Error (W("OpenMode::OpenMode(): Error: Invalid mode: ") + mode);
			    }

                inline bool is_bin_mode(const OpenMode mode) noexcept
                {
                    // returns true if mode ends with b.
                    return mode.back() == 'b';
                }

                inline bool is_text_mode(const OpenMode mode) noexcept
                {
                    return !is_bin_mode(mode);
                }
        };

 
        class BinMode : public OpenMode
        {
            public:
                BinMode (const S& mode) : OpenMode (mode) 
                {
				    if (! is_bin_mode (mode))
					    throw Error (W("BinMode::BinMode(): Error: Invalid binary mode: ") + mode);
                }
        };

        class TextMode : public OpenMode
        {
            public:
                TextMode(const S& mode) : OpenMode(mode)
                {
                    if (!is_text_mode(mode))
                        throw Error(W("TextMode::TextMode(): Error: Invalid text mode: ") + mode);
                }
        };

        // Declares a BinMode constant with W("rb") value.
        inline static const BinMode BIN_READ                = BinMode (W("rb")); // read: Open file for input operations.The file must exist.
        inline static const BinMode BIN_WRITE               = BinMode (W("wb")); // write: Create an empty file for output operations.If a file with the same name already exists, its contents are discarded and the file is treated as a new empty file.
        inline static const BinMode BIN_APPEND_OR_CREATE    = BinMode (W("ab")); // append: Open file for output at the end of a file.Output operations always write data at the end of the file, expanding it.Repositioning operations(fseek, fsetpos, rewind) are ignored.The file is created if it does not exist.
        inline static const BinMode BIN_RW                  = BinMode (W("r+b")); // read / update : Open a file for update(both for input and output).The file must exist.
        inline static const BinMode BIN_CREATE_OVERWRITE_RW = BinMode (W("w+b")); // write / update : Create an empty file and open it for update(both for input and output).If a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
        inline static const BinMode BIN_RW_APPEND_OR_CREATE = BinMode (W("a+b")); // append / update : Open a file for update(both for input and output) with all output operations writing data at the end of the file.Repositioning operations(fseek, fsetpos, rewind) affects the next input operations, but output operations move the position back to the end of file.The file is created if it does not exist.

        inline static const TextMode TEXT_READ                = TextMode (W("r"));   // read: Open file for input operations.The file must exist.
        inline static const TextMode TEXT_WRITE               = TextMode (W("w"));   // write: Create an empty file for output operations.If a file with the same name already exists, its contents are discarded and the file is treated as a new empty file.
        inline static const TextMode TEXT_APPEND_OR_CREATE    = TextMode (W("a"));   // append: Open file for output at the end of a file.Output operations always write data at the end of the file, expanding it.Repositioning operations(fseek, fsetpos, rewind) are ignored.The file is created if it does not exist.
        inline static const TextMode TEXT_RW                  = TextMode (W("r+"));  // read / update : Open a file for update(both for input and output).The file must exist.
        inline static const TextMode TEXT_CREATE_OVERWRITE_RW = TextMode (W("w+"));  // write / update : Create an empty file and open it for update(both for input and output).If a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
        inline static const TextMode TEXT_RW_APPEND_OR_CREATE = TextMode (W("a+"));  // append / update : Open a file for update(both for input and output) with all output operations writing data at the end of the file.Repositioning operations(fseek, fsetpos, rewind) affects the next input operations, but output operations move the position back to the end of file.The file is created if it does not exist.

        inline static const BinMode  DEFAULT_BIN_MODE  = BIN_RW_APPEND_OR_CREATE;
        inline static const TextMode DEFAULT_TEXT_MODE = TEXT_RW_APPEND_OR_CREATE;
        inline static const OpenMode DEFAULT_OPEN_MODE = DEFAULT_BIN_MODE;

        /// \brief File class.
        ///
        class File : public Object
        {
            protected:
                OpenMode mmode; // Last mode used to open the file.
                Path     mfullpath; // Full path of the file. 
                FILE* mfile_ptr;
            public:
                inline static const size_t MAX_BUFFER_SIZE = 1024 ^ 3; // 1 GB
                enum Seek { SET = SEEK_SET, CUR = SEEK_CUR, END = SEEK_END };

                /*
                inline const static OpenMode INPUT = std::ios::in;
                inline const static OpenMode OUTPUT = std::ios::out;
                inline const static OpenMode IN_OUT_ATE_APP_MODE = std::ios::in | std::ios::out | std::ios::ate | std::ios::app;
			    inline const static OpenMode OUT_TRUNC_MODE = std::ios::out | std::ios::trunc;
                inline const static OpenMode DEFAULT_MODE = IN_OUT_ATE_APP_MODE;
                */
				//inline static const VersionPtr VERSION = Version::get(1, 1, 1); 

                inline S s_io_error_msg (const errno_t error) const
                {
                    C msg[icu::BUFFER_SIZE];
                    #ifdef WIDE_CHAR
                        strerror_s(msg, icu::BUFFER_SIZE, error);
                        icu::utf8_char_ptr_to_wchar_t_ptr(msg, sys_err_msg);
                    #else
                    if (msg == nullptr)
                        log_and_throw(W("File::s_io_error_msg(): Error: Could not convert error message to wide char."));
                    else
                        strerror_s(msg, icu::BUFFER_SIZE, error);
                    #endif  
                    // null terminates the string.
                    msg[icu::BUFFER_SIZE - 1] = 0;
					return S(msg);
                }

                decltype(File::mfile_ptr) file_open(const Path& file_name, OpenMode mode)
                {
                    errno_t err;
                    #ifdef WIDE_CHAR
                        err = _wfopen_s(&mfile_ptr, file_name.c_str(), mode.c_str());
                    #else
                        err = fopen_s (&mfile_ptr, file_name.str(), mode.c_str());
                    #endif

                    log_and_throw_if_error(err, W("File::file_open(): Error: Could not open file "));
                    return mfile_ptr;
                }
        public:
            
            inline File(const Path& full_path = W("."), const Id id = NULL_ID, const OpenMode mode = DEFAULT_OPEN_MODE) :
                mfullpath(full_path), Object(id), mmode(mode)
            {
                mfullpath.create_dir (); // Create the directory if it does not exist.

                mfile_ptr = file_open (mfullpath, mode);
            }

            inline virtual ~File() noexcept
            { 
                close();
            }

            inline void close() noexcept 
            { 
                if (is_open())
                {
                    errno_t err = fclose(mfile_ptr);
                    if (err != 0)
                    {
                        //todo log LOG(W("File::~File(): Error: Could not close file ") + mfullpath.str());
                    }
                    mfile_ptr = nullptr;
                }
            }

            inline bool exists() const { return mfullpath.exists(); }

            inline const Path& fullpath() const noexcept { return mfullpath; }

            inline bool is_open() const noexcept { return mfile_ptr != nullptr; }
            
            inline bool is_bin_read () const noexcept { return mmode == BIN_READ; }
            inline bool is_bin_write () const noexcept { return mmode == BIN_WRITE; }
            inline bool is_bin_append_or_create () const noexcept { return mmode == BIN_APPEND_OR_CREATE; }
            inline bool is_bin_rw () const noexcept { return mmode == BIN_RW; }
            inline bool is_bin_create_overwrite_rw () const noexcept { return mmode == BIN_CREATE_OVERWRITE_RW; }
            inline bool is_bin_rw_append_or_create () const noexcept { return mmode == BIN_RW_APPEND_OR_CREATE; }

            inline bool is_text_read () const noexcept { return mmode == TEXT_READ; }
            inline bool is_text_write () const noexcept { return mmode == TEXT_WRITE; }
            inline bool is_text_append_or_create () const noexcept { return mmode == TEXT_APPEND_OR_CREATE; }
            inline bool is_text_rw () const noexcept { return mmode == TEXT_RW; }
            inline bool is_text_create_overwrite_rw () const noexcept { return mmode == TEXT_CREATE_OVERWRITE_RW; }
            inline bool is_text_rw_append_or_create () const noexcept { return mmode == TEXT_RW_APPEND_OR_CREATE; }

            inline bool eof() const noexcept { return feof(mfile_ptr) != 0; }
            inline bool fail() const noexcept { return ferror(mfile_ptr) != 0; }

            inline const C* c_str() const noexcept 
            { 
                S s = mfullpath.str();
            }

            // Delete the file.
            inline bool remove() 
            { 
                if (is_open()) 
                    close();
               
                return (fs::remove(mfullpath) == 0);
            }

                

            // Implements initialize method from Initializable concept.
            inline virtual bool initialize(const Path& afull_path = CURRENT_DIR, const Id id = NULL_ID, const OpenMode mode = DEFAULT_OPEN_MODE) noexcept
            {
                mfullpath = afull_path;
                mmode = mode;
                Object::set_id(id);
                return true;
            }

            //FilePtr clone() const  noexcept { return pd::clone<File>(*this, mfullpath, mmode, id()); }

            inline virtual OpenMode get_mode() const noexcept
            {
                return mmode;
            }

            inline virtual S debug_string() const noexcept
            {
                return Object::debug_string() + W(" path = ") + mfullpath.str();
            }

            inline void log_if_error (const errno_t error, const S& error_msg = W("")) const noexcept
			{
                if (error != 0)
                { 
                    // Log error.
                    S errmsg = error_msg + W(" in file ") + mfullpath.to_string() + W (" ") + s_io_error_msg (error);
                    LOG(errmsg);
                }
            }

            inline void log_and_throw_if_error(const errno_t error, const S& error_msg = W("")) const
            {
                if (error != 0)
                {
                    // Log error.
                    S errmsg = error_msg + W(" in file ") + mfullpath.to_string() + W(" ") + s_io_error_msg(error);
                    LOG(errmsg);
                    throw Error (errmsg);
                }
            }

            inline void move_cursor (const long pos, const Seek seek = Seek::SET) const
			{
				errno_t err = fseek(mfile_ptr, pos, seek);
                if (err != 0)
				{
					S error_msg = W("File::move_cursor(): Error: Could not move cursor to position.");
                    log_and_throw_if_error(err, error_msg);
				}
			}
            inline void move_to_start () const
            {
                    
                move_cursor (0, Seek::SET);
            }

            inline void move_to_end () const
			{
				move_cursor (0, Seek::END); 
            }  

            long position() const
			{
				long pos = ftell (mfile_ptr);
				if (pos == -1) 
				{
					S error_msg = W("File::position(): Error: Could not get file position ");
					log_and_throw_if_error(errno, error_msg);
				}
				return pos;
			}
            
            inline void set_binary_mode() noexcept
            {
                long pos = 0;
                if (is_open())
				{
                    // Gets the current position in the file.
                    long pos = position ();
					close();
				}

                mmode = BIN_RW;
                open();
                if (pos > 0)
					move_cursor (pos);
            }

            inline void set_text_mode()
            {
                long pos = 0;
				if (is_open())
                {
					// Gets the current position in the file.
					long pos = position();
					close();
				}

                mmode = TEXT_RW;
                open();
                if (pos > 0)
                    move_cursor (pos);
            }

            // Opens the file if necessary and returns the file stream.
            inline decltype(mfile_ptr) open (const OpenMode& mode = DEFAULT_OPEN_MODE) 
            {
                mmode = mode;
                if (!is_open())
                {
                    mfile_ptr = file_open (mfullpath, mmode);
                }
                return mfile_ptr;
            }
            
            inline size_t size() const noexcept
            {
                move_to_end();

                long size = ftell(mfile_ptr);
                if (size == -1)
                {
                    S error_msg = W("TextFile::read(): Error: Could not get file size ");
                    //log_and_throw_if_error(errno, error_msg);
					LOG(error_msg);
                }
                return size;
            }

            decltype(mfile_ptr) get_stream()
            {
                open();
                return mfile_ptr;
            }

        };  // class File   

        class TextFile : public File
        {
            private:
            public:
                inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                TextFile(const Path& full_path, const S& content = EMPTY,
                    const TextMode mode = DEFAULT_TEXT_MODE, const Id id = NULL_ID) : File(full_path, id, mode)
                {
                    if (File::exists())
                        append (content);
                    else
                    {
                        open ();
                        move_to_start ();
                        write (content);
                    }
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
                    
                    // Writes content.
                    errno_t err = fwrite(content.c_str(), sizeof(C), content.size(), mfile_ptr) != content.size();
                    if (err != 0)
					{
						log_and_throw_if_error (err, W("TextFile::write(): Error: Could not write to the file "));
					}
					
                    return *this;
                }
                inline TextFile& append (const S& content)
                {
                    // Appends content.
                    if (! File::is_open()) 
                        File::open ();

                    move_to_end ();
				    write (content);

                    return *this;
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
                inline S read() 
                {
					if (! File::is_open()) 
						open (TEXT_READ);

					// Gets the file size in bytes.
                    size_t file_size = size ();

                    size_t count = file_size / sizeof(C);

					// Allocates a buffer to store the file content.
                    C* content = new C[count + 1];
                    
                    // Initializes the buffer with zeros.
                    memset(content, 0, count + 1);

					move_to_start ();
					size_t read_count = fread (content, sizeof(C), count, mfile_ptr);
                    if (read_count != count)
					{
                        // Frees the buffer.    
                        delete[] content;

						S error_msg = W("TextFile::read(): Error: Could not read the file ");
                        log_and_throw (error_msg);
					}
                    // Disables warning C6001: Using uninitialized memory 'content'.
                    #pragma warning(suppress:6001)
                    return S(content);
				}
        };
        
        // Random file file name generator.
        
        class RandomFileNameGenerator
        {
			private:

				inline static const size_t DEFAULT_LENGTH                =     8;
				inline static const S      DEFAULT_TEXT_FILE_EXTENSION   = W("txt");
                inline static const S      DEFAULT_BINARY_FILE_EXTENSION = W("bin");
                
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
					static const S CHARS = W("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
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
                            runtime_error(error_msg);
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
            inline Path operator() (const S& name_prefix = W(""), const S& name_suffix = W(""), const S& extension = DEFAULT_TEXT_FILE_EXTENSION, const Path& path = TMP_DIR)
            {
                return path / (name_prefix + random_string(DEFAULT_LENGTH) + name_suffix + W(".") + get_extension());
            }
        };

        // TmpTextFile is a temporary text file.
        class TmpTextFile : public TextFile
        {
			public:
				inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                inline TmpTextFile (const S& file_name = EMPTY, const S& content = EMPTY, const Id id = null_value<Id>()) : 
                    TextFile(TMP_PATH / file_name, content, TEXT_CREATE_OVERWRITE_RW, id)
                {
				}

                inline TmpTextFile (const C* file_name = EMPTY, const C* content = EMPTY, const Id id = null_value<Id>())
                    : TextFile (S(file_name), S(content), TEXT_CREATE_OVERWRITE_RW, id) {}

                inline void log_error()
                {
                    LOG(W("Failed to remove temporary file ") + File::mfullpath.s ());
                }

                inline virtual ~TmpTextFile()
                {
                    // Removes the file. If it is open, it is closed first. If operation fails, a log message is generated.
                    if (!File::remove())
                    {
                        log_error();
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
                            const OpenMode mode                     = DEFAULT_BIN_MODE, 
                            const BytePtr                 data      = nullptr, 
                            const size_t                  size      = 0,
                            const Id                      id        = null_value<Id>()
                            ) : File (full_path, id, mode)
                {
					append (data, size);
				}

                inline BinaryFile (const Path& full_path, const BytePtr data = nullptr, const size_t size = 0, const Id id = null_value<Id>()) 
                    : BinaryFile(full_path, DEFAULT_BIN_MODE, data, size, id)
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

						move_to_end ();
                        size_t bytes_written = fwrite (reinterpret_cast<const char*>(data), sizeof(std::byte), size, mfile_ptr);
                        if (bytes_written != size)
						{
							S error_msg = W("BinaryFile::append(): Error: Could not append data to the file ");
							log_and_throw_if_error(errno, error_msg);
						}
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