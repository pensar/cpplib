// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "io_util.hpp"
#include "path.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>


namespace pensar_digital
{
    namespace cpplib
    {
        InStream&  operator >> (InStream&  is,       Path& path) { return path.read  (is); }
        OutStream& operator << (OutStream& os, const Path& path) { return path.write (os); }

        void to_json(Json& j, const Path& p)
        {
            j[W("class")] = p.class_name();
            j[W("id")] = p.id();
            j[W("VERSION")] = *p.VERSION;
            j[W("path")] = p;
        }

        void from_json(const Json& j, Path& p)
        {
            S class_name = p.class_name();
            S json_class = j.at(W("class"));
            if (class_name == json_class)
            {
                p.set_id(j.at(W("id")));
                p = j.at(W("path"));
            }
            else throw new std::runtime_error(W("Object expected class = ") + class_name + W(" but json has ") + json_class);
        }
        
    }   // namespace cpplib
} // namespace pensar_digital