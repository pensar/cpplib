// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef FILE_HPP
#define FILE_HPP

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "io_util.hpp"
#include "path.hpp"
#include "object.hpp"
#include "ifile.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

namespace pensar_digital
{
    namespace cpplib
    {
        class File;
        typedef std::shared_ptr<File> FilePtr;

        /// \brief File class.
        ///
        class File : public Object, public IFile
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
            typedef Path    I;  // Interface type.
            typedef PathRO IRO; // Read only interface type.

            inline static const VersionPtr VERSION = pd::versionf.get(1, 1, 1);
            inline static const size_t MAX_IN_MEMORY_FILE_SIZE_BYTE = 1024 ^ 3; // 1 GB
            inline const static std::ios_base::openmode IN_OUT_ATE_MODE = std::ios::in | std::ios::out | std::ios::ate;
            inline const static std::ios_base::openmode IN_OUT_ATE_BIN_MODE = IN_OUT_ATE_MODE | std::ios::binary;
            File(const Path& afull_path = ".", const std::ios_base::openmode amode = IN_OUT_ATE_BIN_MODE, const Id aid = NULL_ID) :
                Object(aid), full_path(afull_path), mode(amode)
            {
                initialize(afull_path, amode, aid);
            }

            virtual ~File() noexcept { close(); }

            void close() noexcept { if (is_open()) file.close(); }

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

            bool is_open() const noexcept { return file.is_open(); }
            bool is_good() const noexcept { return file.good(); }
            bool is_app() const noexcept { return (mode & std::ios::app) != 0; }
            bool is_ate() const noexcept { return (mode & std::ios::ate) != 0; }
            bool is_auto_seek_end_before_write() const noexcept { return is_app(); }
            bool is_auto_seek_end_on_open() const noexcept { return is_ate(); }
            bool is_binary() const noexcept { return (mode & std::ios::binary) != 0; }
            bool is_in() const noexcept { return (mode & std::ios::in) != 0; }
            bool is_out() const noexcept { return (mode & std::ios::out) != 0; }
            bool is_trunc() const noexcept { return (mode & std::ios::trunc) != 0; }
            bool eof() const noexcept { return file.eof(); }
            bool fail() const noexcept { return file.fail(); }
            bool bad() const noexcept { return file.bad(); }
            bool good() const noexcept { return file.good(); }

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
                ss << ", \"full_path\" : " << full_path.to_string() << " , \"mode\" : " << mode;
                if (is_binary())
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

            virtual std::istream& read(std::istream& is, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN)
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
                    set_id(id);
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
                    return is;
                }
            };

            virtual std::ostream& write(std::ostream& os, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) const
            {
                if (amode == BINARY)
                {
                    // todo: implement binary read.
                }
                else // json format
                {
                    os << json();
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
                        xml += pd::binary_to_string<decltype(binary_data)::value_type, String::value_type>(binary_data, s);
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
                if (!n.isEmpty()) 
                    this->full_path = n.getText();
                n = node.getChildNode("mode");
                if (!n.isEmpty()) 
                    this->mode = std::stoi(n.getText());
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
                return Object::debug_string() + " path = " + full_path.to_string();
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
                    file.open(static_cast<fs::path>(full_path), mode);
                }
                return file;
            }

            //friend void to_json   (      Json& j, const File& f);
            //friend void from_json (const Json& j,       File& f);

            // Inherited via IFile
            fs::path get_full_path() const noexcept override;

            // Inherited via Object
            void set_id(const Id& value) override;

            // Inherited via IFile
            void set_name(const String& name) noexcept override;
        };  // class File   

        class TextFile : public File
        {
        private:
        public:
            TextFile(const Path& full_path, const std::ios_base::openmode amode = IN_OUT_ATE_MODE, const String& content = "", const Id aid = NULL_ID) : File(full_path, (amode& (~std::ios::binary)), aid)
            {
                append(content);
            }
            TextFile(const Path& full_path, const String& content = "", const Id aid = NULL_ID) : TextFile(full_path, IN_OUT_ATE_MODE, content, aid)
            {
            }

            virtual ~TextFile() { close(); }

            File& append(const String& content)
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
        // Streaming operators.
        extern std::istream& operator >> (std::istream& is, File& file);
        extern std::ostream& operator << (std::ostream& os, const File& file);

        // Json conversion.
        extern void to_json   (      Json& j, const File& f);
        extern void from_json (const Json& j,       File& f);


    } // namespace cpplib
} // namespace pensar_digital.

#endif // FILE_HPP