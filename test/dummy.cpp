#include "dummy.hpp"
#include "s.hpp" 

namespace pensar_digital
{
    namespace cpplib
    {
        /// Makes Dummy Streamable.
        OutStream& operator << (OutStream& os, const Dummy& d) { return d.write(os); }
        InStream& operator  >> ( InStream& is,       Dummy& d) { return d.read (is); }

       /*
        void to_json(Json& j, const Dummy& d)
        {
            j["class"     ] = d.class_name();
            j["id"        ] = d.id();
            j["mpublic"] = d.VERSION->get_public();
            j["mprotected"] = d.VERSION->get_protected();
            j["mprivate"] = d.VERSION->get_private();
            j["name"      ] = d.get_name();
        }

        void from_json(const Json& j, Dummy& d)
        {
            S class_name = d.class_name();
            S json_class = j.at("class");
            if (class_name == json_class)
            {
                d.Object::set_id(j.at("id"));
                d.name = j.at("name");
            }
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
        */
    }
}