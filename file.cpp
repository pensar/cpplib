#include "file.hpp"
#include "path.hpp"


namespace pensar_digital
{
    namespace cpplib
    { 
        InStream&  operator >> (InStream&  is,       File& file) { return file.read  (is); }
        OutStream& operator << (OutStream& os, const File& file) { return file.write (os); }

        void to_json(Json& j, const File& f)
         {
             j[W("class")] = f.class_name();
             j[W("id")] = f.id();
             j[W("mpublic")] = f.VERSION->get_public();
             j[W("mprotected")] = f.VERSION->get_protected();
             j[W("mprivate")] = f.VERSION->get_private();
             j[W("path")] = f.fullpath ();
             j[W("mode")] = f.is_binary() ? W("BINARY") : W("TEXT");
         }

         void from_json(const Json& j, File& f)
         {
             S class_name = f.class_name();
             S json_class = j.at(W("class"));
             if (class_name == json_class)
             {
                 f.set_id(j.at(W("id")));
                 f.mfullpath = j.at(W("mfullpath"));
                 if (j.at(W("mode")) == W("BINARY"))
                 {
                     f.set_binary_mode ();
                 }
                 else
                 {
                    f.set_text_mode ();
                 }
             }
             else throw new std::runtime_error(W("Object expected class = ") + class_name + W(" but json has ") + json_class);
         }

    }   // namespace cpplib
} // namespace pensar_digital
