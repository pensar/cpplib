// license: MIT (https://opensource.org/licenses/MIT)

#ifndef PATH_HPP
#define PATH_HPP

#include "object.hpp"
#include "s.hpp"
#include "system.hpp"
#include "s.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "version.hpp"
#include "concept.hpp"
#include "io_util.hpp"
#include "clone_util.hpp"
#include "error.hpp"

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
                inline static PathFactory mfactory = { 3, 10, W("."), null_value<Id>()};
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

            Path(const fs::path& p = W("."), const Id& id = null_value<Id>()) : Object(id), fs::path(p) {}
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

                    S s = str ();
                    if (s.back() == System::path_separator())
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

             virtual InStream& read (InStream& is, const std::endian& byte_order = std::endian::native)
            {
                return is;
            };

            virtual OutStream& write(OutStream& os, const std::endian& byte_order = std::endian::native) const
            {
                Object::write(os, byte_order);
                VERSION->write(os, byte_order);
                os.write((const C*)data(), data_size());
                return os;
            };

            virtual S debug_string() const noexcept
            {
                return Object::debug_string() + W(" path = ") + s ();
            }

            // Move assignment operator.
            Path& operator = (Path&& p) noexcept { fs::path::operator = (p); return *this; }

            // Assignment operator for S.
            Path& operator = (const S & s) { fs::path::operator = (s); return *this; }

            // Assignment operator for const Path&.
            Path& operator = (const Path& p) { fs::path::operator = (p); return *this; }

            /// <summary>
            /// Creates the directory if it does not exist (including all parent directories).
            /// It assumes it is a directory if the path ends with a path separator. Otherwise it assumes it is a file.
            /// In the latter case it creates the parent directory.
            /// </summary>
            void create_dir () const noexcept
            {
                if (has_filename() && has_parent_path ()) // Final slash indicates directory. If not present it assumes file.
					fs::create_directories (parent_path());
				else
					fs::create_directories (*this);
            }

            inline size_t size () const noexcept { return s ().size(); }



            inline fs::path std_path() const noexcept { return static_cast<const fs::path&>(*this); }

            // Implicit conversion to fs::path.
            operator fs::path() const noexcept { return std_path(); }

            // Implicit conversion to std::string.
            operator std::string() const noexcept { return fs::path::string(); }

            // Implicit conversion to std::wstring.
            operator std::wstring() const noexcept { return fs::path::wstring(); }

            S s () const
			{
                #ifdef WIDE_CHAR 
					return fs::path::wstring();
                #else
					return fs::path::string();
				#endif
			}

            // Implicit conversion to const char* returning a value allocated in the heap using _strdup.
            operator const char* () const { return _strdup(fs::path::string().c_str()); }

            wchar_t * wstr() const { return _wcsdup(fs::path::wstring().c_str()); }
            char* cstr() const { return _strdup(fs::path::string().c_str()); }

            C* str() const
            {
                #ifdef WIDE_CHAR 
                    return wstr();
                #else
                    return cstr();
                #endif  
            }
           
            // Conversion from const char* operator.
            Path& operator = (const char* s) { fs::path::operator = (s); return *this; }

            // /= operator for S.
            Path& operator /= (const S& s) { fs::path::operator /= (s); return *this; }

            // /= operator for const Path&.
            Path& operator /= (const Path& p) { fs::path::operator /= (p); return *this; }

            // + operator for S.
            Path operator + (const S& s) const { Path p = *this; p /= s; return p; }

            // == operator 
            bool operator == (const Path& apath) const { return apath.std_path () == this->std_path (); }

            // != operator for std::string.
            bool operator != (const std::string& s) const { return !(*this == s); }

            // == operator for const char*.
            bool operator == (const char* s) const { return *this == std::string(s); }

            // != operator for const char*.
            bool operator != (const char* s) const { return !(*this == s); }

            // Inherited via Object
            inline void set_id(const Id& value) { Object::set_id(value); }

            using fs::path::value_type;
        };  // class Path

        class CPath : public CS<0, MAX_PATH>
        {
            public:
                using P = CS<0, MAX_PATH>;
                CPath(const P& path = CURRENT_DIR) : P(path) {}
                
                // Constructor from fs::path.
                //CPath (const fs::path& path) : Str (path.string()) {}

                inline fs::path to_fspath() const noexcept { return fs::path(P::to_string()); }
                inline Path     to_path() const noexcept { return Path(P::to_string()); }

                // Conversion to fs::path operator.
                operator fs::path() const noexcept { return to_fspath(); }
        };
         
        // path_to_cpath.
        inline CPath path_to_cpath (const Path& path)
        {
			return CPath(path.s());
        }

        // cpath_to_path.
        inline Path cpath_to_path(const CPath& cpath)
        {
            return Path (cpath.to_path());
        }        


        // Sets TMP environment variable to a temporary directory.
        inline const Path& set_tmp_env_var(const Path& path = fs::temp_directory_path())
        {
            Path p = W("TMP=");
            p += path;
            int r = _putenv(p.cstr());
            if (r != 0)
				runtime_error(W("Error setting TMP environment variable."));
            return path;
        }
        inline static const C* TMP_DIR = W("c:\\tmp\\");
        inline static const Path TMP_PATH = set_tmp_env_var (TMP_DIR);
    } // namespace cpplib
} // namespace pensar_digital
#endif  // PATH_HPP