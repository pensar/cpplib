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
                inline const static std::ios_base::openmode IN_OUT_ATE_MODE = std::ios::in | std::ios::out | std::ios::ate;
			    inline const static std::ios_base::openmode IN_OUT_TRUNC_MODE = std::ios::in | std::ios::out | std::ios::trunc;
                inline const static std::ios_base::openmode DEFAULT_MODE = IN_OUT_ATE_MODE;

        protected:
                std::ios_base::openmode mmode; // Last mode used to open the file.
                Path mfullpath;                // Full path of the file. 
                typedef std::basic_fstream<C> FStream;
                FStream* stream_ptr; 
        public:
            
            File(const Path& full_path = L".", const Id id = NULL_ID, const std::ios_base::openmode mode = IN_OUT_ATE_MODE) :
                mfullpath(full_path), Object(id), mmode(mode)
            {
                mfullpath.create_dir (); // Create the directory if it does not exist.
                stream_ptr = new FStream (full_path.std_path (), mode);
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
            virtual S json() const noexcept
            {
                std::stringstream ss;
                S s = pd::json<File>(*this);
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
                    mfullpath = j["mfullpath"].get<S>();
                    mmode = j["mode"].get<std::ios_base::openmode>();
                }
                return is;
            };

            // Convertion to xml string.
            virtual S xml() const noexcept
            {
                S xml = ObjXMLPrefix() + "><path>";
                xml += mfullpath.string() + "</path><mode>";
                xml += std::to_string(mmode) + "</mode>";
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual void from_xml(const S& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                XMLNode n = node.getChildNode("path");
                if (!n.isEmpty()) 
                    this->mfullpath = n.getText();
                n = node.getChildNode("mode");
                if (!n.isEmpty()) 
                    this->mmode = std::stoi(n.getText());
            }

            virtual S debug_string() const noexcept
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
            inline FStream& open ()
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
                typedef std::basic_string<C> S;
                inline static const VersionPtr VERSION = Version::get(1, 1, 1);
                TextFile(const Path& full_path, const S& content = EMPTY<C>, 
                         const std::ios_base::openmode mode = File<C>::DEFAULT_MODE, const Id id = NULL_ID) : File<C>(full_path, id, (mode& (~std::ios::binary)))
                {
                    append (content);
                }

                //TextFile(const Path& full_path, const S& content = EMPTY<C>, const Id aid = NULL_ID) :
                //    TextFile(full_path, File<C>::DEFAULT_MODE, content, id)
                //{
                //}

                virtual ~TextFile() = default;

                TextFile<C>& append (const S& content)
                {
                    // Appends content.
                    if (! File<C>::is_open()) 
                        File<C>::open ();

                    File<C>::stream_ptr->seekp(0, std::ios::end);
				    *(File<C>::stream_ptr) << content;
                    return *this;
                }

                S to_string() const noexcept
                {
                    return File<C>::mfullpath.to_string ();
                }

                // Implicit conversion to string.
                operator S() const noexcept
                {
                    return to_string();
                }
        };

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
                            const Id                      id        = NULL_ID
                            ) : File (full_path, id, (mode | std::ios::binary))
                {
					append (data, size);
				}

                BinaryFile (const Path& full_path, const BytePtr data = nullptr, const size_t size = 0, const Id id = NULL_ID) : BinaryFile(full_path, IN_OUT_ATE_MODE, data, size, id)
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