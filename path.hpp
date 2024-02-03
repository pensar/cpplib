// license: MIT (https://opensource.org/licenses/MIT)

#ifndef PATH_HPP
#define PATH_HPP

#include "object.hpp"
#include "s.hpp"
#include "system.hpp"
#include "string_def.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "version.hpp"
#include "concept.hpp"
#include "json_util.hpp"
#include "io_util.hpp"
#include "clone_util.hpp"

#include <string>
#include <cstdio>
#include <stdlib.h>

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
        namespace pd = pensar_digital::cpplib;  

        class Path;
        typedef std::shared_ptr<Path> PathPtr;
        typedef Factory<Path, fs::path, Id> PathFactory;

        // Path class represents a path in the file system. It inherits from fs::path adding implicit conversion to std::string.
        class Path : public fs::path, public Object
        {
            private:
                inline static PathFactory mfactory = { 3, 10, ".", null_value<Id>()};
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

            Path(const fs::path& p = ".", const Id& id = null_value<Id>()) : Object(id), fs::path(p) {}
            Path(const std::string& s, const Id& id = null_value<Id>()) : Object(id), fs::path(s) {}
            Path(const char* path, const Id& id = null_value<Id>()) : Object(id), fs::path(path) {}
            Path(const wchar_t* path, const Id& id = null_value<Id>()) : Object(id), fs::path(path) {}
            Path(const std::wstring& path, const Id& id = null_value<Id>()) : Object(id), fs::path(path) {}

            // Copy constructor.
            Path(const Path& p, const Id& aid = null_value<Id>()) noexcept : Object(aid), fs::path(p) {}

            // Move constructor.
            Path(Path&& p, const Id& aid = null_value<Id>()) noexcept : Object(aid), fs::path(p) {}

            // virtual destructor.
            virtual ~Path() noexcept = default;
            static PathFactory::P get(const fs::path& p = ".", const Id& aid = null_value<Id>())
            {
                return mfactory.get (p, aid);
            };

            PathFactory::P clone(const Path& apath)
            {
                return get(apath.std_path(), apath.id());
            };

            PathFactory::P clone(const PathPtr& ptr) { return clone(*ptr); }
           
            PathFactory::P parse_json(const String& sjson)
            {
                auto j = Json::parse(sjson);
                String json_class = j.at("class");
                if (json_class != pd::class_name<Path, char>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<Path, char>());
                Id id;
                VersionPtr v;
                Path p;
                pd::read_json<Path>(sjson, p, &id, &v, &j);
				set_id(id);

                return clone(p);
            };

            // is directory?
            bool is_directory() const noexcept
            {
				return fs::is_directory(*this);
			}

            bool remove() const noexcept
            {
                if (exists())
                {
                    // Remove file or directory.
                    if (is_directory ())
						return fs::remove_all(*this);
					else
						return fs::remove(*this);
                }
                return true;
            }

            bool exists() const noexcept
            {
                return fs::exists(*this);
            }

            Path absolute() const noexcept
            {
                return fs::absolute(*this);
            }

            Path canonical() const noexcept
            {
                return fs::canonical(*this);
            }

            Path relative() const noexcept
            {
                return fs::relative(*this);
            }

            Path parent_path() const noexcept
            {
                return fs::path::parent_path();
            }

            Path root_path () const noexcept
			{
				return fs::path::root_path();
			}

            Path filename() const noexcept
            {
                return fs::path::filename();
            }

            Path stem() const noexcept
            {
                return fs::path::stem();
            }

            Path filename_only () const noexcept
            {
                return stem ();
			}

            Path extension() const noexcept
            {
                return fs::path::extension();
            }

            Path& replace_extension(const std::string& s) noexcept
            {
                fs::path::replace_extension(s);
                return *this;
            }

            Path& replace_extension(const Path& p) noexcept
            {
                fs::path::replace_extension(p);
                return *this;
            }

            Path& make_preferred() noexcept
            {
                fs::path::make_preferred();
                return *this;
            }

            Path& remove_filename() noexcept
            {
                fs::path::remove_filename();
                return *this;
            }

            Path& remove_trailing_separator() noexcept
            {

                if (! has_filename()) 
                {
                    std::string s = this->string();
                    if (s.back() == pd::PATH_SEPARATOR<char>)
                    {
						s.pop_back();
						*this = s;
					}
                }
                return *this;
            }

            Path copy_without_trailing_separator() const noexcept
			{
				Path p = *this;
				return p.remove_trailing_separator();
			}

            // Implements initialize method from Initializable concept.
            virtual bool initialize(const fs::path& p, const Id& aid = null_value<Id>()) noexcept
            {
                fs::path::operator = (p);
                Object::set_id(aid);
                return true;
            }

            // Conversion to json string.
            virtual String json() const noexcept
            {
                std::stringstream ss(pd::json<Path>(*this));
                ss << ",path:" << this->fs::path::string() << "}";
                return ss.str();
            }

            virtual std::istream& read(std::istream& is, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native)
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
                    set_id(stream_id);
                    *this = j["path"].get<std::string>();

                    if (*VERSION != *v) throw std::runtime_error("Version mismatch.");
                }
                return is;
            };

            virtual std::ostream& write(std::ostream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const
            {
                if (amode == BINARY)
                {
                    Object::write(os, amode, byte_order);
                    VERSION->write(os, amode, byte_order);
                    os.write((const char*)data(), data_size());
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
                return Object::debug_string() + " path = " + fs::path::string();
            }

            // Move assignment operator.
            Path& operator = (Path&& p) noexcept { fs::path::operator = (p); return *this; }

            // Assignment operator for std::string.
            Path& operator = (const std::string& s) { fs::path::operator = (s); return *this; }

            // Assignment operator for const Path&.
            Path& operator = (const Path& p) { fs::path::operator = (p); return *this; }

            /// <summary>
            /// Creates the directory if it does not exist (including all parent directories).
            /// It assumes it is a directory if the path ends with a path separator. Otherwise it assumes it is a file.
            /// In the latter case it creates the parent directory.
            /// </summary>
            void create_dir () const noexcept
            {
                if (has_filename()) // Final slash indicates directory. If not present it assumes file.
					fs::create_directories (parent_path());
				else
					fs::create_directories (*this);
            }

            inline size_t size () const noexcept { return fs::path::string().size(); }



            inline fs::path std_path() const noexcept { return static_cast<const fs::path&>(*this); }

            // Implicit conversion to fs::path.
            operator fs::path() const noexcept { return std_path(); }

            // Implicit conversion to std::string.
            operator std::string() const noexcept { return fs::path::string(); }

            // Implicit conversion to std::wstring.
            operator std::wstring() const noexcept { return fs::path::wstring(); }

            template <typename C = char>
            std::basic_string<C> string () const
			{
				if constexpr (std::is_same_v<C, char>)
					return fs::path::string();
				else if constexpr (std::is_same_v<C, wchar_t>)
					return fs::path::wstring();
				else
					throw std::runtime_error("Unsupported character type.");
			}

            // Implicit conversion to const char* returning a value allocated in the heap using _strdup.
            operator const char* () const { return _strdup(fs::path::string().c_str()); }

            wchar_t * wstr() const { return _wcsdup(fs::path::wstring().c_str()); }
            char* cstr() const { return _strdup(fs::path::string().c_str()); }

            template <typename C = char>
            C* str() const
            {
                if constexpr (std::is_same_v<C, char>)
					return cstr();
				else if constexpr (std::is_same_v<C, wchar_t>)
					return wstr();
				else
				    throw std::runtime_error("Unsupported character type."); 
            }
           
            // Conversion from const char* operator.
            Path& operator = (const char* s) { fs::path::operator = (s); return *this; }

            // /= operator for std::string.
            Path& operator /= (const std::string& s) { fs::path::operator /= (s); return *this; }

            // /= operator for const Path&.
            Path& operator /= (const Path& p) { fs::path::operator /= (p); return *this; }

            // + operator for std::string.s
            Path operator + (const std::string& s) const { Path p = *this; p /= s; return p; }

            // == operator 
            bool operator == (const Path& apath) const { return apath.std_path () == this->std_path (); }

            // != operator for std::string.
            bool operator != (const std::string& s) const { return !(*this == s); }

            // == operator for const char*.
            bool operator == (const char* s) const { return *this == std::string(s); }

            // != operator for const char*.
            bool operator != (const char* s) const { return !(*this == s); }
            friend void to_json(Json& j, const Path& d);
            friend void from_json(const Json& j, Path& d);

            // Inherited via Object
            inline void set_id(const Id& value) { Object::set_id(value); }

            using fs::path::value_type;
        };  // class Path

        template <typename C = char>
        class CPath : public S<C>
        {
            public:
                CPath(const S<C>& path = CURRENT_DIR<C>) : S<C>(path) {}
                
                // Constructor from fs::path.
                //CPath (const fs::path& path) : Str (path.string()) {}

                inline fs::path to_fspath() const noexcept { return fs::path(S<C>::to_string()); }
                inline Path     to_path() const noexcept { return Path(S<C>::to_string()); }

                // Conversion to fs::path operator.
                operator fs::path() const noexcept { return to_fspath(); }
        };
         
        // path_to_cpath.
        template <typename C = char>
        inline CPath<C> path_to_cpath(const Path& path)
        {
            if constexpr (std::is_same_v<C, char>)
				return CPath<C>(path.string());
			else
				return CPath<C>(path.wstring());
        }

        // cpath_to_path.
        inline Path cpath_to_path(const CPath<char>& cpath)
        {
            return Path (cpath.to_path());
        }        

        inline Path cpath_to_path(const CPath<wchar_t>& cpath)
        {
            return Path (cpath.to_path());
        }

        // Json conversion.
        extern void to_json   (      Json& j, const Path& p);
        extern void from_json (const Json& j,       Path& p);

        // Sets TMP environment variable to a temporary directory.
        inline const Path& set_tmp_env_var(const Path& path = fs::temp_directory_path())
        {
            Path p = "TMP=";
            p += path;
            _putenv(p.cstr());
            return path;
        }
        static inline const Path TMP_PATH = set_tmp_env_var("c:\\tmp\\");
        template<typename C = char>
        struct TmpDirString
        {
            inline static const C* value = TMP_PATH.cstr ();
        };

        template<>
        struct TmpDirString<wchar_t>
        {
            inline static const wchar_t* value = TMP_PATH.wstr ();
        };

        template <typename C = char>
        inline static const C* TMP_DIR = TmpDirString<C>::value;
    } // namespace cpplib
} // namespace pensar_digital
#endif  // CPPLIB_FS_PATH_H